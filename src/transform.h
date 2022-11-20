#pragma once

#include "glm_includes.h"

struct Transform
{
	glm::vec3 pos{0.0f};
	glm::vec3 scale{1.0f};
	glm::vec3 rotation{0.0f};

	Transform() = default;
	Transform(const glm::vec3& pos);
	Transform(const glm::vec3& pos, const glm::vec3& scale);
	Transform(const glm::vec3& pos, const glm::vec3& scale, const glm::vec3& rotation);
	
	~Transform() = default;

	glm::mat4 matrixObjTransform();
	glm::mat3 matrixNormal();
	glm::mat4 matrixView();
};