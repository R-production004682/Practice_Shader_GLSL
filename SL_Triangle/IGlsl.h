#include <GL/glew.h>
#include <GL/glut.h>

GLuint vertexShader, fragmentShader;

// shaderFileを読み込みコンパイルする
void readShaderCompile(GLuint shader, const char* file) {
	FILE* fp;
	GLchar* buf;
	GLsizei size, len;
	GLint compiled;

	// ファイルを開く
	if (fopen_s(&fp, file, "rb") != 0) {
		printf("ファイルを開くことが出来ません: %s\n", file);
		return;
	}

	// ファイルの未尾に移動し現在位置を得る
	fseek(fp, 0, SEEK_END);
	size = ftell(fp); // ファイルサイズを取得
	fseek(fp, 0, SEEK_SET); // 先頭に戻る

	// ファイルサイズのメモリの確保
	buf = (GLchar*)malloc(size + 1); // NULL終端のために+1
	if (buf == NULL) {
		printf("メモリの確保が出来ませんでした。\n");
		fclose(fp);
		return;
	}

	// ファイルを読み込む
	fread(buf, 1, size, fp);
	buf[size] = '\0'; // NULL終端
	fclose(fp);

	// シェーダ・オブジェクトにプログラムをセット
	const GLchar* source = buf;
	glShaderSource(shader, 1, &source, NULL);

	// シェーダの読み込み領域の解放
	free(buf);

	// シェーダのコンパイル
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

	if (compiled == GL_FALSE) {
		printf("コンパイルできませんでした: %s\n", file);
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &size);
		if (size > 0) {
			buf = (GLchar*)malloc(size);
			glGetShaderInfoLog(shader, size, &len, buf);
			printf("エラー詳細:\n%s\n", buf);
			free(buf);
		}
	}
}

void link(GLuint prog) {
	GLsizei size, len;
	GLint linked;
	char* infoLog;

	glLinkProgram(prog);
	glGetProgramiv(prog , GL_LINK_STATUS, &linked);
	if (linked == GL_FALSE) {
		printf("リンクできません。\n");
		glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &size);
		if (size > 0) {
			infoLog = (char*)malloc(size);
			glGetProgramInfoLog(prog, size, &len, infoLog);
			printf(infoLog);

			free(infoLog);
		}

	}
}

void initGlsl(GLuint* program, const char* vertexFile) {
	// glewの初期化
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		printf("ERROR : %s\n", glewGetErrorString(err));
	}

	// GPU, OpenGL情報
	printf("VENDOR = %s \n", glewGetString(GL_VENDOR));
	printf("GPU = %s \n", glGetString(GL_RENDERER));
	printf("OpenGL = %s \n", glGetString(GL_VERSION));
	printf("GLSL = %s \n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	// シェーダ・オブジェクトの作成
	vertexShader = glCreateShader(GL_VERTEX_SHADER); 
	
	// shaderの読み込みとコンパイル
	readShaderCompile(vertexShader, vertexFile); 

	// シェーダ・プログラムの作成
	*program = glCreateProgram();
	
	// シェーダ・オブジェクトを、シェーダ・プログラムに関連付けする
	glAttachShader(*program , vertexShader);
	
	// シェーダ・オブジェクトの削除
	glDeleteShader(vertexShader);
	
	// シェーダ・プログラムのリンク
	link(*program);
}

void initGlsl(GLuint* program, const char* vertexFile, const char* fragmentFile) {
	// glewの初期化
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		printf("ERROR : %s\n", glewGetErrorString(err));
	}

	// GPU, OpenGL情報
	printf("VENDOR = %s \n", glGetString(GL_VENDOR));
	printf("GPU = %s \n", glGetString(GL_RENDERER));
	printf("OpenGL = %s \n", glGetString(GL_VERSION));
	printf("GLSL = %s \n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	// シェーダ・オブジェクトの作成
	vertexShader = glCreateShader(GL_VERTEX_SHADER); 
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// shaderの読み込みとコンパイル
	readShaderCompile(vertexShader, vertexFile); 
	readShaderCompile(fragmentShader, fragmentFile);

	// プログラム・オブジェクトの作成
	*program = glCreateProgram();

	// シェーダ・オブジェクトを、シェーダープログラムに関連付ける
	glAttachShader(*program, vertexShader);
	glAttachShader(*program, fragmentShader);

	// シェーダ・オブジェクトの削除
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// シェーダプログラムのリンク
	link(*program);
}

