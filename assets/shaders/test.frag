#version 450 core

in TESE_OUT {
	vec2 uv;
	vec3 wpos;
	vec3 vpos;
} tese_out;

out vec4 FragColor;

uniform vec3 uColor;
uniform sampler2D uTex;
uniform sampler2D uTex2;

void main()
{
	FragColor =  texture(uTex, tese_out.uv);
}