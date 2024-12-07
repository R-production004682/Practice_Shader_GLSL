void main(void){
	vec3 P = vec3(gl_ModelViewMatrix * gl_Vertex); // 環境光成分
	vec3 N = normalize(gl_NormalMatrix * gl_Normal); // 拡散反射光成分
	vec3 L = normalize(gl_LightSource[0].position.xyz - P); // フォンの反射モデルによる鏡面反射光成分
	float dotNL = dot(N, L);

	vec4 ambient = gl_FrontLightProduct[0].ambient;
	vec4 diffuse = gl_FrontLightProduct[0].diffuse * max(0.0 , dotNL);

	vec3 V = normalize(-P); // 正反射ベクトル
	vec3 H = normalize(L + V);
	float dotNH = pow(max(dot(N, H), 0.0), gl_FrontMaterial.shininess);

	if(dotNL <= 0.0) {
		dotNH = 0.0;
	}

	vec4 specular = gl_FrontLightProduct[0].specular * dotNH;

	// 統合
	gl_FrontColor = ambient + diffuse + specular;
	gl_Position = ftransform(); // クリップ空間へ
}