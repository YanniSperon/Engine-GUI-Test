#version 460

layout (location = 0) in vec3 in_VertexPosition;
layout (location = 1) in vec3 in_VertexNormal;
layout (location = 2) in vec2 in_VertexTexCoord;

uniform mat4 u_P;
uniform mat4 u_V;
uniform mat4 u_M;

out vec2 s_TexCoord;
out vec3 s_WorldPos;
out vec3 s_Normal;

void main()
{
	s_TexCoord = in_VertexTexCoord;
	s_WorldPos = vec3(u_M * vec4(in_VertexPosition, 1.0));
	s_Normal = mat3(u_M) * in_VertexNormal;

	gl_Position = u_P * u_V * vec4(s_WorldPos, 1.0);
}