#include "transform.h"

// Transform::Transform() {

// }

Transform::Transform(const glm::vec3& pos) : pos(pos) {

}

Transform::Transform(const glm::vec3& pos, const glm::vec3& scale) : pos(pos), scale(scale) {

}

Transform::Transform(const glm::vec3& pos, const glm::vec3& scale, const glm::vec3& rotation) : pos(pos), scale(scale), rotation(rotation) {

}

glm::mat4 Transform::matrixObjTransform(){
	auto transform = glm::identity<glm::mat4>();
	transform = glm::translate(transform, pos);
	transform = glm::scale(transform, scale);
	transform = transform * glm::yawPitchRoll(rotation[1], rotation[0], rotation[2]);
	return transform;
}

glm::mat3 Transform::matrixNormal(){
	return glm::identity<glm::mat4>() * glm::yawPitchRoll(rotation[1], rotation[0], rotation[2]);
}

glm::mat4 Transform::matrixView(){
	auto transform = glm::identity<glm::mat4>();
	transform = glm::translate(transform, pos);
	transform = transform * glm::yawPitchRoll(rotation[1], rotation[0], rotation[2]);
	return glm::inverse(transform);
}