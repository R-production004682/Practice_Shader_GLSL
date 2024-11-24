#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "IGlsl.h"

// 初期設定
void init();
// 描画処理
void display();
// 三角形を描画する処理
void drawTriangle();

GLuint shaderProg;

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA);   
    glutInitWindowSize(500, 500);     
    glutInitWindowPosition(100, 100); 
    glutCreateWindow("GL_Triangle");  
    glutDisplayFunc(display);         
    init();
    initGlsl(&shaderProg, "triangle.vert"); // 頂点シェーダ
    glutMainLoop();
    glDeleteProgram(shaderProg); // リソース解放
    return 0;
}

// 背景色などの初期設定
void init() {
    glClearColor(0.2, 0.2, 0.2, 1.0); // 背景色（ダークグレー）を設定
}

// 描画処理
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProg); // シェーダ・プログラムの適応
    drawTriangle(); 
    glUseProgram(0); // シェーダ・プログラムの適応の解除

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

