#version 460

layout (location = 0) in vec3 in_VertexPosition;
layout (location = 1) in vec3 in_VertexNormal;
layout (location = 2) in vec2 in_VertexTexCoord;

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

out vec2 s_TexCoord;

void main()
{
	gl_Position = P * V * M * vec4(in_VertexPosition, 1.0);
	s_TexCoord = in_VertexTexCoord;
}