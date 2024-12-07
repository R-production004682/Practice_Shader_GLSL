void main(void){
	vec3 P = vec3(gl_ModelViewMatrix * gl_Vertex); // ��������
	vec3 N = normalize(gl_NormalMatrix * gl_Normal); // �g�U���ˌ�����
	vec3 L = normalize(gl_LightSource[0].position.xyz - P); // �t�H���̔��˃��f���ɂ�鋾�ʔ��ˌ�����
	float dotNL = dot(N, L);

	vec4 ambient = gl_FrontLightProduct[0].ambient;
	vec4 diffuse = gl_FrontLightProduct[0].diffuse * max(0.0 , dotNL);

	vec3 V = normalize(-P); // �����˃x�N�g��
	vec3 H = normalize(L + V);
	float dotNH = pow(max(dot(N, H), 0.0), gl_FrontMaterial.shininess);

	if(dotNL <= 0.0) {
		dotNH = 0.0;
	}

	vec4 specular = gl_FrontLightProduct[0].specular * dotNH;

	// ����
	gl_FrontColor = ambient + diffuse + specular;
	gl_Position = ftransform(); // �N���b�v��Ԃ�
}