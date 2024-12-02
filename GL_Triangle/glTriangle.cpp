#include <GL/glut.h>
#include <iostream>

// 初期設定
void init();
// 描画処理
void display();
// 三角形を描画する処理
void drawTriangle();
// 四角形を描画する処理
void drawSquare();
// 星形を描画する処理
void drawStar();

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA);    // 表示モードの設定
    glutInitWindowSize(500, 500);      // ウィンドウのサイズを指定
    glutInitWindowPosition(100, 100);  // ウィンドウの初期位置（左上からのピクセル数）
    glutCreateWindow("GL_Triangle");   // ウィンドウを作成
    glutDisplayFunc(display);          // 描画関数を登録
    init();                            // 初期設定
    glutMainLoop();                    // イベントループの開始
    return 0;
}

// 背景色などの初期設定
void init() {
    glClearColor(0.2, 0.2, 0.2, 1.0); // 背景色（ダークグレー）を設定
}

// 描画処理
void display() {
    glClear(GL_COLOR_BUFFER_BIT); // カラーバッファのクリア

   // drawTriangle(); // 三角形を描画
   // drawSquare();   // 四角形の描画
    drawStar();     // 星形の描画
   
    glFlush(); // 描画命令を即時実行
}

// 三角形を描画する処理
void drawTriangle() {
    glLineWidth(10.0);                         // 線の幅を設定
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // ワイヤーフレームモードを設定

    glBegin(GL_TRIANGLES);                     // 三角形の描画開始

    // 赤い頂点を指定
    glColor3f(1.0f, 0.0f, 0.0f); // 頂点色: 赤
    glVertex2f(-0.75f, -0.75f);  // 頂点位置

    // 緑の頂点を指定
    glColor3f(0.0f, 1.0f, 0.0f); // 頂点色: 緑
    glVertex2f(0.75f, -0.75f);   // 頂点位置

    // 青い頂点を指定
    glColor3f(0.0f, 0.0f, 1.0f); // 頂点色: 青
    glVertex2f(0.0f, 0.75f);    // 頂点位置

    glEnd(); // 描画終了
}

// 四角形の
// 描画
void drawSquare() {
    glLineWidth(10.0);                         // 線の幅を設定
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // ワイヤーフレームモードを設定

    glBegin(GL_QUADS);                         // 四角形の描画開始

    // 黄色い頂点を指定
    glColor3f(0.5f, 0.5f, 0.0f); // 頂点色 : 黄色
    glVertex2f(0.5f, 0.5f);    // 頂点位置

    // 青い頂点を指定
    glColor3f(0.0f, 0.0f, 1.0f); // 頂点色 : 青
    glVertex2f(-0.25f, 0.5f);     // 頂点位置

    // 赤い頂点を指定
    glColor3f(1.0f, 0.0f, 0.0f); // 頂点色 : 赤
    glVertex2f(-0.25f, -0.25f);    // 頂点位置

    // 緑の頂点を指定
    glColor3f(0.0f, 1.0f, 0.0f); // 頂点色 : 緑
    glVertex2f(0.5f, -0.25f);   // 頂点位置

    glEnd(); // 描画終了
}


// 星形の描画
void drawStar() {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // ワイヤーフレームモードを設定

    // 星形の頂点を指定
    glBegin(GL_POLYGON);

    // 頂点の色を設定（黄色）
    glColor3f(1.0f, 1.0f, 0.0f); // 黄色

    // 頂点座標を指定（時計回りで星形を作成）
    glVertex2f(0.0f, 0.5f);    // 上
    glVertex2f(-0.2f, 0.1f);   // 左上
    glVertex2f(-0.5f, 0.1f);   // 左
    glVertex2f(-0.3f, -0.1f);   // 左下
    glVertex2f(-0.4f, -0.5f);   // 下左
    glVertex2f(0.0f, -0.3f);   // 下
    glVertex2f(0.4f, -0.5f);   // 下右
    glVertex2f(0.3f, -0.1f);    // 右下
    glVertex2f(0.5f, 0.1f);    // 右
    glVertex2f(0.2f, 0.1f);    // 右上

    glEnd();
}
