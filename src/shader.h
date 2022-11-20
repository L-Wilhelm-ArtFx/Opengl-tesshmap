#pragma once
#include <GL/glew.h>
#include <string>

class ShaderProgram
{
private:
	GLuint progId = 0;

public:
	ShaderProgram();

	bool load(
		const std::string& vertexSrc,
		const std::string& fragmentSrc,
		const std::string& tessControlSrc = "",
		const std::string& tessEvalSrc = "",
		const std::string& geometrySrc = "");
	void use();
	void destroy();
	GLuint getProgramId();

private:
	GLuint createShader(const std::string& src, GLenum shaderType);
};