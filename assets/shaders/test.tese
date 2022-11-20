#version 450 core

layout (quads, fractional_odd_spacing) in;

in TESC_OUT
{
    vec2 uv;
} tesc_out[];

out TESE_OUT {
	vec2 uv;
	vec3 wpos;
	vec3 vpos;
} tese_out;

uniform mat4 objectTransform;
uniform mat4 perspective;
uniform mat4 viewTransform;
uniform mat3 normMatrix;

uniform sampler2D uTex;
uniform float uHeightAmount;

void main()
{
	float tcX = gl_TessCoord.x;
    float tcY = gl_TessCoord.y;

	vec4 p0 = gl_in[0].gl_Position;
    vec4 p1 = gl_in[1].gl_Position;
    vec4 p2 = gl_in[2].gl_Position;
    vec4 p3 = gl_in[3].gl_Position;

	vec4 pos = mix(
		mix(p0, p1, tcX),
		mix(p2, p3, tcX),
		tcY
	);

	vec2 uv0 = tesc_out[0].uv;
    vec2 uv1 = tesc_out[1].uv;
    vec2 uv2 = tesc_out[2].uv;
    vec2 uv3 = tesc_out[3].uv;

	vec2 uv = mix(
		mix(uv0, uv1, tcX),
		mix(uv2, uv3, tcX),
		tcY
	);

	pos.y += texture(uTex, uv).r * uHeightAmount;

	vec4 wpos = objectTransform * pos;
	tese_out.wpos = wpos.xyz;

	vec4 vpos = viewTransform * wpos;
	tese_out.vpos = vpos.xyz;

	gl_Position = perspective * vpos;

	tese_out.uv = uv;

}