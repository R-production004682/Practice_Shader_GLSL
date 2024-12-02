#include <stdio.h>
#include <GL/glut.h>
#include <math.h>
#define M_PI 3.141592653

// 関数にプロトタイプを宣言
void init();
void idle();
void display();
void draw();
void resize(int w, int h);
void keyboard(unsigned char key, int x, int y);
void special(int key, int x, int y);
void setLight();
void setCamera();
void mouse(int button, int state, int x, int y);
void motion(int x, int y);

// アフィン変数
float pos[] = { 0.0, 0.0, 0.0 };
float scale[] = {1.0, 1.0, 1.0};
float angle[] = { 0.0, -30.0, 0.0 };

// 初期値
float originPosition[] = {0.0, 0.0, 0.0};
float orisinScale[] = {1.0, 1.0, 1.0 };
float orisinAngle[] = {0.0f, -30.0, 0.0};

// 光源
float lightPosition[] = { 10.0, 15.0, 10.0, 1.0 }; // 光源位置
float originLightPosition[] = {10.0, 15.0, 10.0, 1.0}; // 光源位置（初期位置）

struct View
{
    float pos[3]; // 位置（視点）
    float cnt[3]; // 注視点
    float dist;   // 注視点から始点までの距離
    float theta;  // 仰角（水平面との偏角）
    float phi;    // 方位角

    // 視体積
    float fovY;   // 視野角
    float nearZ;  // 前方グリップ面（近平面）
    float farZ;   // 後方クリップ面（遠平面）
};

// 初期値
View view = {
    0.0, 0.0, 5.0, // 位置（pos）、z = 5.0 とする
    0.0, 0.0, 0.0, // 注視点（cnt）
    5.0,           // 注視点からカメラの距離
    30.0, 45.0,    // 仰角（theta）、方位角（phi）
    30.0, 1.0, 100.0 // fovY, nearZ, farZ
};
View view0 = view;

// windowのサイズ
int width = 800;
int height = 600;

// アフィン変換
enum SELECT_KEY {ROTATE, SCALE, TRANSLATE, LIGHT};
SELECT_KEY sKey = TRANSLATE;

// マウス操作
int xStart, tStart;
bool flagMouse = false;

// ヘルプフラグ
bool flagHelp = false;

// 光源位置変更フラグ
bool flagLight = false;

// ワイヤーフレーム<--->ソリッドの切り替えフラグ
bool flagWireframe = false;

// フラット<--->スムーズシェーディング切り替えフラグ
bool flagFlat = false;

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE); // 表示モード
    glutInitWindowSize(width, height);
    glutInitWindowPosition(100,100);
    glutCreateWindow("グローディングシェーディング(GL_GOURAND)");
    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);

    // マウス操作
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutIdleFunc(idle);
    init();
    glutMainLoop();
    return 0;
}

void idle() {
    // 再描画
    glutPostRedisplay();
}

void init(void) {
    // 背景色
    glClearColor(0.0, 0.0, 0.0, 1.0);
    setCamera(); // 視点を決める
    setLight();  // 光源設定
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
}

void display(void) {
    // カラーバッファ, デプスバッファをクリア
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity(); // 視点を変えるときはこの位置に必要

    if (cos(M_PI * view.theta / 180.0) >= 0.0) {
        // カメラ仰角度でビューアップベクトル切り替え
        gluLookAt(
            view.pos[0], view.pos[1], view.pos[2],
            view.cnt[0], view.cnt[1], view.cnt[2],
            0.0, 1.0, 0.0
        );
    }
    else {
        // カメラ仰角度でビューバックベクトル切り替え
        gluLookAt(
            view.pos[0], view.pos[1], view.pos[2],
            view.cnt[0], view.cnt[1], view.cnt[2],
            0.0, -1.0, 0.0
        );
    }

    // 光源設定
    // '1'を押したら光源位置可変
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    if (flagWireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // ワイヤーフレームモード
    }
    else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // ソリッドモード
    }

    if (flagFlat) {
        // 'f'でflatとsmooth shading切り替え
        glShadeModel(GL_FLAT);
    }
    else {
        glShadeModel(GL_SMOOTH);
    }

    draw(); // 描画
    glutSwapBuffers();
}

void draw(void) {
    float ambient[]  = {0.1, 0.3, 0.3, 1.0};
    float diffuse[]  = {0.2, 0.7, 0.7, 1.0};
    float specular[] = {0.5, 0.5, 0.5, 1.0};

    glMaterialfv(GL_FRONT,GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT,GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT,GL_SPECULAR, specular);
    glMaterialf(GL_FRONT,GL_SHININESS, 100);

    glPushMatrix();
    glTranslatef(pos[0], pos[1], pos[2]);
    glRotatef(angle[2], 0.0, 0.0, 0.0); // z軸回転
    glRotatef(angle[1], 0.0, 1.0, 0.0); // y軸回転
    glRotatef(angle[0], 1.0, 0.0, 0.0); // x軸回転
    glScalef(scale[0], scale[1], scale[2]);

    // オブジェクトの形状
    glutSolidSphere(1.0, 50, 50);
    glPopMatrix();
}

void resize(int w, int h) {
    glViewport(0, 0, w, h); // ビューポート変換
    glMatrixMode(GL_PROJECTION); // プロジェクション行列の指定
    glLoadIdentity(); // プロジェクション行列の初期化

    // 透視投影行列の設定（投影変換）
    gluPerspective(view.fovY, (double)w/(double)h, view.nearZ, view.farZ);
    glMatrixMode(GL_MODELVIEW); // モデル・ビュー行列の宣言

    width  = w;
    height = h;
}

void setLight() {
    float lightAmbient0[] = {0.5, 0.5, 0.5, 1.0};  // 環境光
    float lightDiffuse0[] = {1.0, 1.0, 1.0, 1.0};  // 拡散光
    float lightSpecular0[] = {1.0, 1.0, 1.0, 1.0}; // 鏡面光
    glLightfv(GL_LIGHT0, GL_AMBIENT , lightAmbient0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE , lightDiffuse0);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'w': // ワイヤーフレーム切り替え
        flagWireframe = !flagWireframe;
        break;
    case 'f': // フラット/スムーズシェーディング切り替え
        flagFlat = !flagFlat;
        break;
    case '1': // 光源移動モード切り替え
        flagLight = !flagLight;
        break;
    case 27: // ESCキーで終了
        exit(0);
    default:
        break;
    }
    glutPostRedisplay(); // 再描画要求
}

void special(int key, int x, int y) {
    const float step = 5.0; // 移動/回転のステップ量

    switch (key) {
    case GLUT_KEY_UP:    // 上矢印キーでカメラの仰角を増加
        view.theta -= step;
        break;
    case GLUT_KEY_DOWN:  // 下矢印キーでカメラの仰角を減少
        view.theta += step;
        break;
    case GLUT_KEY_LEFT:  // 左矢印キーでカメラの方位角を減少
        view.phi -= step;
        break;
    case GLUT_KEY_RIGHT: // 右矢印キーでカメラの方位角を増加
        view.phi += step;
        break;
    default:
        break;
    }
    setCamera(); // カメラ更新
    glutPostRedisplay();
}

void setCamera() {
    view.pos[0] = view.cnt[0] + view.dist * sin(view.theta * M_PI / 180.0) * cos(view.phi * M_PI / 180.0);
    view.pos[1] = view.cnt[1] + view.dist * cos(view.theta * M_PI / 180.0);
    view.pos[2] = view.cnt[2] + view.dist * sin(view.theta * M_PI / 180.0) * sin(view.phi * M_PI / 180.0);
}

void mouse(int button, int state, int x, int y) {
    if (state == GLUT_DOWN) {
        xStart = x;
        tStart = y;
        flagMouse = true;

        if (button == GLUT_LEFT_BUTTON) {
            sKey = ROTATE; // 左クリックで回転モード
        }
        else if (button == GLUT_MIDDLE_BUTTON) {
            sKey = SCALE;  // 中クリックでスケールモード
        }
        else if (button == GLUT_RIGHT_BUTTON) {
            sKey = TRANSLATE; // 右クリックで移動モード
        }
    }
    else if (state == GLUT_UP) {
        flagMouse = false;
    }
}

void motion(int x, int y) {
    if (flagMouse) {
        int dx = x - xStart;
        int dy = y - tStart;

        switch (sKey) {
        case ROTATE:
            angle[0] += dy * 0.2; // X軸回転
            angle[1] += dx * 0.2; // Y軸回転
            break;
        case SCALE:
            scale[0] = fmax(0.1, scale[0] + dx * 0.01);
            scale[1] = fmax(0.1, scale[1] + dy * 0.01);
            scale[2] = fmax(0.1, scale[2] + (dx + dy) * 0.005);
            break;
        case TRANSLATE:
            pos[0] += dx * 0.01;
            pos[1] -= dy * 0.01;
            break;
        case LIGHT:
            lightPosition[0] += dx * 0.1;
            lightPosition[1] -= dy * 0.1;
            break;
        }

        xStart = x;
        tStart = y;
        glutPostRedisplay(); // 再描画要求
    }
}