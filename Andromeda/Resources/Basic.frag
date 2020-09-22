#version 460

out vec4 out_Color;
in vec2 s_TexCoord;
uniform sampler2D in_Albedo;

void main()
{
	out_Color = texture(in_Albedo, s_TexCoord);
}