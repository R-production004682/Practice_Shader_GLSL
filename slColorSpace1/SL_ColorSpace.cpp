#include <stdio.h>
#include <math.h>
#include "IGlsl.h"
#define M_PI 3.141592

// 関数のプロトタイプを宣言
void init();
void idle();
void display();
void draw();
void drawColorSpace();
void resize(int width, int height);
void keyboard(unsigned char keyCode, int vertical, int horizontal);
void special(int key, int vertical, int horizontal);

// affine 変換用変数
float position[] = { 0.0f,  0.0f,  0.0f }; // 図形中心のxyz座標
float scale[] = { 1.0f,  1.0f,  1.0f }; // 大きさ（倍率）
float angle[] = { 0.0f, -30.0f, 0.0f }; // 回転角度

// 初期値
float originPosition[] = { .0f,  .0f,  .0f };
float origineScale[] = { 1.0f, 1.0f, 1.0f };
float origineAngle[] = { .0f, -30.f, .0f };

// カメラと視体積
struct View {
    float pos[3]; // 位置（始点）
    float cnt[3]; // 注視点
    float dist;   // 注視点から始点までの距離
    float theta;  // 仰角
    float phi;    // 方位角

    //視体積
    float fovY;   // 視野角
    float nearZ;  // 前歩クリップ面（近平面）
    float farZ;   // 後方クリップ面（遠平面）
};

// 初期値
View view = {
     0.0f,  2.0f,  4.0f, // position
     0.0f,  0.0f, 0.0f, // cnt
     4.0f, 30.0f, 20.0f, // dist, theta, phi
    30.0f,  1.0f, 100.0f // fovY, nearZ, farZ 
};

// windowのサイズ
int  width = 1000;
int height = 1000;

//アフィン変換
enum  SELECT_KEY { ROTATE, SCALE, TRANSLATE };
SELECT_KEY selectKey = TRANSLATE;

// Helpキーフラグ
bool flagHelp = false;

// 色の分布率（ユニフォーム変数の例）
float factor0 = 0.5;
float delta = 0.01;
GLuint shaderProg;


int main(int argc, char** argv)
{
    glutInit(&argc, argv);                        // GLUTの基本設定
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE); // 表示モード
    glutInitWindowSize(width, height);            // 表示ウィンドウのサイズ
    glutInitWindowPosition(100, 100);            // 左上の位置
    glutCreateWindow("色空間(GL_ColorSpace)");    // ウィンドウ作成
    glutReshapeFunc(resize);                      // ウィンドウのサイズ変更
    glutDisplayFunc(display);                     // 表示
    glutKeyboardFunc(keyboard);                   // キーボードの利用
    glutSpecialFunc(special);                     // 矢印キーなどの特殊キーの利用
    glutIdleFunc(idle);                           // 再描画
    init();                                       // 初期設定
    // initGlsl(&shaderProg, "ColorSpace1.vert", "ColorSpace1.flag"); // RGB色空間とCMY色空間を反転
    initGlsl(&shaderProg, "ColorSpace2.vert"); // RGB色空間とCMY色空間の中間色を含め交互に繰り返すアニメーション。
    glutMainLoop();                               // イベント処理ループに入る
    glDeleteProgram(shaderProg);
    return 0;
}

void idle() {
    // 再描画
    glutPostRedisplay();
}

void init(void) {
    // 背景色
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void display(void) {

    /* RGB色空間とCMY色空間を反転させたい場合に有効にする
        glClear(GL_COLOR_BUFFER_BIT); // カラーバッファのクリア
        draw();
        glutSwapBuffers(); // 終了
    */


    // RGB色空間とCMY色空間の中間色を含め交互に繰り返すアニメーションを有効にしたい場合は下記の処理を用いる。
    glClear(GL_COLOR_BUFFER_BIT); // カラーバッファのクリア
    if (factor0 <= 0.0) {
        delta = 0.01;
    }
    if (factor0 >= 1.0) {
        delta = -0.01;
    }
    factor0 += delta;
    glUseProgram(shaderProg); // シェーダ・プログラムの適用
    GLuint factorLoc = glGetUniformLocation(shaderProg, "factor");
    glUniform1f(factorLoc, factor0);
    draw();
    glUseProgram(0); // シェーダ・プログラムの適応を解除
    glutSwapBuffers(); // 終了
}

void draw() {

    glPolygonMode(GL_FRONT, GL_FILL); // モード

    glPolygonMode(GL_BACK, GL_POINT);
    glPushMatrix();
    glTranslatef(position[0], position[1], position[2]);
    glRotatef(angle[2], 0.0f, 0.0f, 1.0f); // z軸回転
    glRotatef(angle[1], 0.0f, 1.0f, 0.0f); // z軸回転
    glRotatef(angle[0], 1.0f, 0.0f, 0.0f); // z軸回転
    glScalef(scale[0], scale[1], scale[2]);
    drawColorSpace(); // 描画オブジェクト
    glPopMatrix();
}

void drawColorSpace() {
    float p[8][3] =
    {
        {0.5f, 0.5f, 0.5f},{-0.5f, 0.5f, 0.5f},{-0.5f, -0.5f, 0.5f},{0.5f, -0.5f, 0.5f},
        {0.5f, 0.5f, -0.5f},{-0.5f, 0.5f, -0.5f},{-0.5f, -0.5f, -0.5f},{0.5f, -0.5f, -0.5f}
    };

    // 各頂点の色
    float c[8][3] =
    {
        {1.0f, 1.0f, 1.0f},{1.0f, 0.0f, 1.0f},{1.0f, 0.0f, 0.0f},{1.0f, 1.0f, 0.0f},
        {0.0f, 1.0f, 1.0f},{0.0f, 0.0f, 1.0f},{0.0f, 0.0f, 0.0f},{0.0f, 1.0f, 0.0f}
    };

    // 立方体の描画
    glBegin(GL_QUADS);
    glColor3fv(c[0]); glVertex3fv(p[0]);
    glColor3fv(c[1]); glVertex3fv(p[1]);
    glColor3fv(c[2]); glVertex3fv(p[2]);
    glColor3fv(c[3]); glVertex3fv(p[3]);

    glColor3fv(c[0]); glVertex3fv(p[0]);
    glColor3fv(c[3]); glVertex3fv(p[3]);
    glColor3fv(c[7]); glVertex3fv(p[7]);
    glColor3fv(c[4]); glVertex3fv(p[4]);

    glColor3fv(c[0]); glVertex3fv(p[0]);
    glColor3fv(c[4]); glVertex3fv(p[4]);
    glColor3fv(c[5]); glVertex3fv(p[5]);
    glColor3fv(c[1]); glVertex3fv(p[1]);

    glColor3fv(c[1]); glVertex3fv(p[1]);
    glColor3fv(c[5]); glVertex3fv(p[5]);
    glColor3fv(c[6]); glVertex3fv(p[6]);
    glColor3fv(c[2]); glVertex3fv(p[2]);

    glColor3fv(c[2]); glVertex3fv(p[2]);
    glColor3fv(c[6]); glVertex3fv(p[6]);
    glColor3fv(c[7]); glVertex3fv(p[7]);
    glColor3fv(c[3]); glVertex3fv(p[3]);

    glColor3fv(c[4]); glVertex3fv(p[4]);
    glColor3fv(c[7]); glVertex3fv(p[7]);
    glColor3fv(c[6]); glVertex3fv(p[6]);
    glColor3fv(c[5]); glVertex3fv(p[5]);

    glEnd();
}

void resize(int w, int h) {
    glViewport(.0f, .0f, w, h); // ビューポート変換
    glMatrixMode(GL_PROJECTION); // 投影行列の指定
    glLoadIdentity(); // モデルビュー行列の初期化

    // 透視投影行列の設定（投影変換）
    gluPerspective(view.fovY, (double)w / (double)h, view.nearZ, view.farZ);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // 視点を設定（視野変換）
    gluLookAt
    (
        view.pos[0], view.pos[1], view.pos[2],
        view.cnt[0], view.cnt[1], view.cnt[2],
        .0f, 1.f, .0f
    );

    ::width = w; ::height = h; // 表示ウィンドウのサイズ
}

void keyboard(unsigned char keyCode, int x, int y) {
    switch (keyCode) {
    case 'e': // 回転モードに切り替え
        selectKey = ROTATE;
        break;
    case 'r': // スケールモードに切り替え
        selectKey = SCALE;
        break;
    case 'w': // 移動モードに切り替え
        selectKey = TRANSLATE;
        break;
    case 'q': // 終了
    case 27:  // ESCキー
        exit(0);
        break;
    case 'x': // 初期状態にリセット
        for (int i = 0; i < 3; ++i) {
            position[i] = originPosition[i];
            scale[i] = origineScale[i];
            angle[i] = origineAngle[i];
        }
        break;
    }
    glutPostRedisplay(); // 再描画
}

void special(int key, int x, int y) {
    float delta = 0.1f; // 変更量
    switch (selectKey) {
    case TRANSLATE: // 移動
        switch (key) {
        case GLUT_KEY_UP:
            position[1] += delta; // Y軸方向に移動
            break;
        case GLUT_KEY_DOWN:
            position[1] -= delta;
            break;
        case GLUT_KEY_LEFT:
            position[0] -= delta; // X軸方向に移動
            break;
        case GLUT_KEY_RIGHT:
            position[0] += delta;
            break;
        }
        break;

    case SCALE: // スケール変更
        switch (key) {
        case GLUT_KEY_UP:
            scale[1] += delta; // Y軸方向のスケール増加
            break;
        case GLUT_KEY_DOWN:
            scale[1] -= delta;
            break;
        case GLUT_KEY_LEFT:
            scale[0] -= delta; // X軸方向のスケール減少
            break;
        case GLUT_KEY_RIGHT:
            scale[0] += delta;
            break;
        }
        break;

    case ROTATE: // 回転
        switch (key) {
        case GLUT_KEY_UP:
            angle[0] += 5.0f; // X軸回転
            break;
        case GLUT_KEY_DOWN:
            angle[0] -= 5.0f;
            break;
        case GLUT_KEY_LEFT:
            angle[1] += 5.0f; // Y軸回転
            break;
        case GLUT_KEY_RIGHT:
            angle[1] -= 5.0f;
            break;
        }
        break;
    }
    glutPostRedisplay(); // 再描画
}