uniform float factor;
void main(void) {
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_FrontColor = mix(gl_Color, (vec4(1.0) - gl_Color) , factor);
}