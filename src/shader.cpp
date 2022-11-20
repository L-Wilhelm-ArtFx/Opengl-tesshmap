#include "shader.h"
#include <iostream>

ShaderProgram::ShaderProgram()
{
}

GLuint ShaderProgram::getProgramId()
{
	return progId;
}

bool ShaderProgram::load(
	const std::string& vertexSrc,
	const std::string& fragmentSrc,
	const std::string& tessControlSrc,
	const std::string& tessEvalSrc,
	const std::string& geometrySrc)
{
	GLuint shaderV = createShader(vertexSrc, GL_VERTEX_SHADER);
	GLuint shaderF = createShader(fragmentSrc, GL_FRAGMENT_SHADER);
	GLuint shaderTC = createShader(tessControlSrc, GL_TESS_CONTROL_SHADER);
	GLuint shaderTE = createShader(tessEvalSrc, GL_TESS_EVALUATION_SHADER);
	GLuint shaderG = createShader(geometrySrc, GL_GEOMETRY_SHADER);
	if (shaderV == 0 || shaderF == 0)
	{
		fprintf(stderr, "Aborting ShaderProgram linking.");
		return false;
	}

	progId = glCreateProgram();
	glAttachShader(progId, shaderV);
	glAttachShader(progId, shaderF);
	if (shaderTC != 0)
		glAttachShader(progId, shaderTC);
	if (shaderTE != 0)
		glAttachShader(progId, shaderTE);
	if (shaderG != 0)
		glAttachShader(progId, shaderG);

	glLinkProgram(progId);

	GLint linkStatus;
	glGetProgramiv(progId, GL_LINK_STATUS, &linkStatus);
	if (!linkStatus)
	{
		char infoLog[1024];
		glGetProgramInfoLog(progId, 1024, NULL, infoLog);
		fprintf(stderr, "Failed to link ShaderProgram:\n%s", infoLog);
		glDeleteProgram(progId);
		progId = 0;
	}

	glDeleteShader(shaderV);
	glDeleteShader(shaderF);
	if (shaderTC != 0)
		glDeleteShader(shaderTC);
	if (shaderTE != 0)
		glDeleteShader(shaderTE);
	if (shaderG != 0)
		glDeleteShader(shaderG);

	return progId != 0;
}

GLuint ShaderProgram::createShader(const std::string& src, GLenum shaderType)
{
	if(src.empty())
		return 0;

	GLuint shader = glCreateShader(shaderType);
	const char* cSrc = src.c_str();
	glShaderSource(shader, 1, &cSrc, NULL);
	glCompileShader(shader);

	GLint compileStatus;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);

	if (!compileStatus)
	{
		char infoLog[1024];
		glGetShaderInfoLog(shader, 1024, NULL, infoLog);
		std::string shaderTypeName = "Unknown Shader";
		switch (shaderType)
		{
		case GL_VERTEX_SHADER:
			shaderTypeName = "Vertex Shader";
			break;
		case GL_FRAGMENT_SHADER:
			shaderTypeName = "Fragment Shader";
			break;
		case GL_TESS_CONTROL_SHADER:
			shaderTypeName = "Tesselation Control Shader";
			break;
		case GL_TESS_EVALUATION_SHADER:
			shaderTypeName = "Tesselation Evaluation Shader";
			break;
		case GL_GEOMETRY_SHADER:
			shaderTypeName = "Geometry Shader";
			break;
		}
		fprintf(stderr, "%s compilation failed:\n%s", shaderTypeName.c_str(), infoLog);
		glDeleteShader(shader);
		return 0;
	}

	return shader;
}

void ShaderProgram::use()
{
	glUseProgram(progId);
}

void ShaderProgram::destroy()
{
	if (progId == 0)
		return;
	glDeleteProgram(progId);
}