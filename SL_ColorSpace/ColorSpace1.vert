void main(void) {
	// ���_���W�̏o��
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	// �t���O�����g�E�V�F�[�_�֒��_�F���o��
	gl_FrontColor = gl_Color;
}