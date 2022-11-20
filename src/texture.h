#pragma once

#include <GL/glew.h>

class GlTexture {
private:
	GLuint glID;
	unsigned int width, height, channels;
public:

	GlTexture();
	~GlTexture();
	GlTexture(const GlTexture&) = delete;

	void load(unsigned char* data, unsigned int width, unsigned height, GLint internalFormat = GL_RGB, GLint format = GL_RGB, GLint type = GL_UNSIGNED_BYTE);

	void destroy();

	void use(GLint texTarget = GL_TEXTURE0);

	GLuint GetID();
	unsigned int GetWidth();
	unsigned int GetHeight();
	unsigned int GetChannels();
};