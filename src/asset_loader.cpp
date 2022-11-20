#include "asset_loader.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

#include "texture.h"
#include "stb_image.h"

std::string loadFileAsString(const std::string &path)
{
	std::ifstream stream(path);
	std::stringstream buff;
	buff << stream.rdbuf();

	return buff.str();
}

bool loadFileAsTexture(const std::string &path, GlTexture &target)
{
	int width, height, nrChannels;
	uint8_t *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
	
	if(data == nullptr){
		return false;
	}
	GLint format = GL_RGB;
	if (nrChannels == 1)
	{
		format = GL_RED;
	}
	else if (nrChannels == 2)
	{
		format = GL_RG;
	}
	else if (nrChannels == 3)
	{
		format = GL_RGB;
	}
	else
	{
		format = GL_RGBA;
	}
	target.load(data, width, height, GL_RGB, format);
	stbi_image_free(data);
	return true;
}