#include "texture.h"

GlTexture::GlTexture() {}
GlTexture::~GlTexture() {}

void GlTexture::load(unsigned char* data, unsigned int width, unsigned height, GLint internalFormat, GLint format, GLint type) {
	destroy();
	glGenTextures(1, &glID);
	glBindTexture(GL_TEXTURE_2D, glID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, data);
}

void GlTexture::destroy(){
	if(glID){
		glDeleteTextures(1, &glID);
		glID = 0;
	}
}

void GlTexture::use(GLint texTarget){
	glActiveTexture(texTarget);
	glBindTexture(GL_TEXTURE_2D, glID);
}

GLuint GlTexture::GetID(){
	return glID;
}

unsigned int GlTexture::GetWidth(){
	return width;
}

unsigned int GlTexture::GetHeight(){
	return height;
}

unsigned int GlTexture::GetChannels(){
	return channels;
}