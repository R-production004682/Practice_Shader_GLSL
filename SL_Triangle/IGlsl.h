#include <GL/glew.h>
#include <GL/glut.h>

GLuint vertexShader, fragmentShader;

// shaderFile��ǂݍ��݃R���p�C������
void readShaderCompile(GLuint shader, const char* file) {
	FILE* fp;
	GLchar* buf;
	GLsizei size, len;
	GLint compiled;

	// �t�@�C�����J��
	if (fopen_s(&fp, file, "rb") != 0) {
		printf("�t�@�C�����J�����Ƃ��o���܂���: %s\n", file);
		return;
	}

	// �t�@�C���̖����Ɉړ������݈ʒu�𓾂�
	fseek(fp, 0, SEEK_END);
	size = ftell(fp); // �t�@�C���T�C�Y���擾
	fseek(fp, 0, SEEK_SET); // �擪�ɖ߂�

	// �t�@�C���T�C�Y�̃������̊m��
	buf = (GLchar*)malloc(size + 1); // NULL�I�[�̂��߂�+1
	if (buf == NULL) {
		printf("�������̊m�ۂ��o���܂���ł����B\n");
		fclose(fp);
		return;
	}

	// �t�@�C����ǂݍ���
	fread(buf, 1, size, fp);
	buf[size] = '\0'; // NULL�I�[
	fclose(fp);

	// �V�F�[�_�E�I�u�W�F�N�g�Ƀv���O�������Z�b�g
	const GLchar* source = buf;
	glShaderSource(shader, 1, &source, NULL);

	// �V�F�[�_�̓ǂݍ��ݗ̈�̉��
	free(buf);

	// �V�F�[�_�̃R���p�C��
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

	if (compiled == GL_FALSE) {
		printf("�R���p�C���ł��܂���ł���: %s\n", file);
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &size);
		if (size > 0) {
			buf = (GLchar*)malloc(size);
			glGetShaderInfoLog(shader, size, &len, buf);
			printf("�G���[�ڍ�:\n%s\n", buf);
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
		printf("�����N�ł��܂���B\n");
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
	// glew�̏�����
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		printf("ERROR : %s\n", glewGetErrorString(err));
	}

	// GPU, OpenGL���
	printf("VENDOR = %s \n", glewGetString(GL_VENDOR));
	printf("GPU = %s \n", glGetString(GL_RENDERER));
	printf("OpenGL = %s \n", glGetString(GL_VERSION));
	printf("GLSL = %s \n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	// �V�F�[�_�E�I�u�W�F�N�g�̍쐬
	vertexShader = glCreateShader(GL_VERTEX_SHADER); 
	
	// shader�̓ǂݍ��݂ƃR���p�C��
	readShaderCompile(vertexShader, vertexFile); 

	// �V�F�[�_�E�v���O�����̍쐬
	*program = glCreateProgram();
	
	// �V�F�[�_�E�I�u�W�F�N�g���A�V�F�[�_�E�v���O�����Ɋ֘A�t������
	glAttachShader(*program , vertexShader);
	
	// �V�F�[�_�E�I�u�W�F�N�g�̍폜
	glDeleteShader(vertexShader);
	
	// �V�F�[�_�E�v���O�����̃����N
	link(*program);
}

void initGlsl(GLuint* program, const char* vertexFile, const char* fragmentFile) {
	// glew�̏�����
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		printf("ERROR : %s\n", glewGetErrorString(err));
	}

	// GPU, OpenGL���
	printf("VENDOR = %s \n", glGetString(GL_VENDOR));
	printf("GPU = %s \n", glGetString(GL_RENDERER));
	printf("OpenGL = %s \n", glGetString(GL_VERSION));
	printf("GLSL = %s \n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	// �V�F�[�_�E�I�u�W�F�N�g�̍쐬
	vertexShader = glCreateShader(GL_VERTEX_SHADER); 
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// shader�̓ǂݍ��݂ƃR���p�C��
	readShaderCompile(vertexShader, vertexFile); 
	readShaderCompile(fragmentShader, fragmentFile);

	// �v���O�����E�I�u�W�F�N�g�̍쐬
	*program = glCreateProgram();

	// �V�F�[�_�E�I�u�W�F�N�g���A�V�F�[�_�[�v���O�����Ɋ֘A�t����
	glAttachShader(*program, vertexShader);
	glAttachShader(*program, fragmentShader);

	// �V�F�[�_�E�I�u�W�F�N�g�̍폜
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// �V�F�[�_�v���O�����̃����N
	link(*program);
}

