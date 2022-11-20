#pragma once

#include <string>

#include "texture.h"

std::string loadFileAsString(const std::string& path);
bool loadFileAsTexture(const std::string &path, GlTexture &target);