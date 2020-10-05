#version 460

out vec4 out_Color;
in vec2 s_TexCoord;
uniform sampler2D in_Albedo;

struct Light {
	vec3 position;
	vec3 color;
}

#define NUM_LIGHTS 10
uniform Light lights[NUM_LIGHTS];

void main()
{
	vec4 tempColor = texture(in_Albedo, s_TexCoord);
	float gamma = 1.0;

	vec3 normal = 

    out_Color.rgb = pow(tempColor.rgb, vec3(1.0/gamma));
}