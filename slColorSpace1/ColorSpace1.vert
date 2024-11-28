void main(void) {
	// 頂点座標の出力
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	// フラグメント・シェーダへ頂点色を出力
	gl_FrontColor = gl_Color;
}