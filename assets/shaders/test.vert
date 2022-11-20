#version 450 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;

out VERT_OUT {
	vec2 uv;
} vert_out;

uniform mat4 objectTransform;
uniform mat4 perspective;
uniform mat4 viewTransform;
uniform mat3 normMatrix;

void main()
{
	vec3 pos = aPos;
	gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);
	vert_out.uv = aUV;
}