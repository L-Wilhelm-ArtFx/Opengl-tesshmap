#version 450 core

layout (vertices = 4) out;

in VERT_OUT {
	vec2 uv;
} vert_out[];

out TESC_OUT
{
    vec2 uv;
} tesc_out[];

uniform vec2 tessInner;
uniform vec4 tessOuter;

void main(void)
{
    if (gl_InvocationID == 0)
    {
        gl_TessLevelInner[0] = tessInner.x;
        gl_TessLevelInner[1] = tessInner.y;
        gl_TessLevelOuter[0] = tessOuter.x;
        gl_TessLevelOuter[1] = tessOuter.y;
        gl_TessLevelOuter[2] = tessOuter.z;
        gl_TessLevelOuter[3] = tessOuter.w;
    }
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
    tesc_out[gl_InvocationID].uv = vert_out[gl_InvocationID].uv;
}