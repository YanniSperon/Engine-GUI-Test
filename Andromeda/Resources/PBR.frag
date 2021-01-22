#version 460

// constants
const float c_Pi = 3.14159265359;
const float c_Gamma = 2.2;

// io
out vec4 out_Color;
in vec2 s_TexCoord;
in vec3 s_WorldPos;
in vec3 s_Normal;

// camera
uniform vec3 u_CamPos;

// material
uniform sampler2D u_Albedo;
uniform sampler2D u_Diffuse;
uniform sampler2D u_Metallic;
uniform sampler2D u_Roughness;
uniform sampler2D u_AO;

// lights
struct Light {
	vec3 position;
	vec3 color;
};
#define NUM_LIGHTS 10
uniform Light u_Lights[NUM_LIGHTS];








//---------------Helper---------------//
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = c_Pi * denom * denom;

    return nom / max(denom, 0.001);
}
//---------------Helper---------------//
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
//---------------Helper---------------//
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
//---------------Helper---------------//
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}
//---------------Helper---------------//
vec3 getNormalFromNormalMap()
{
    return normalize(s_Normal * (texture(u_Diffuse, s_TexCoord).rgb * 2.0 - 1.0));
}







void main()
{
    vec3 albedo = pow(texture(u_Albedo, s_TexCoord).rgb, vec3(c_Gamma));
    vec3 normal = getNormalFromNormalMap();
    float metallic = texture(u_Metallic, s_TexCoord).r;
    float roughness = texture(u_Roughness, s_TexCoord).r;
    float ao = texture(u_AO, s_TexCoord).r;

    vec3 V = normalize(u_CamPos - s_WorldPos);

    // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)    
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

    // reflectance equation
    vec3 Lo = vec3(0.0);
    for(int i = 0; i < NUM_LIGHTS; ++i) 
    {
        // calculate per-light radiance
        vec3 L = normalize(u_Lights[i].position - s_WorldPos);
        vec3 H = normalize(V + L);
        float distance = length(u_Lights[i].position - s_WorldPos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = u_Lights[i].color * attenuation;

        // Cook-Torrance BRDF
        float NDF = DistributionGGX(normal, H, roughness);   
        float G   = GeometrySmith(normal, V, L, roughness);      
        vec3 F    = fresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);
        
        vec3 nominator    = NDF * G * F; 
        float denominator = 4 * max(dot(normal, V), 0.0) * max(dot(normal, L), 0.0);
        vec3 specular = nominator / max(denominator, 0.001); // prevent divide by zero for NdotV=0.0 or NdotL=0.0
        
        // kS is equal to Fresnel
        vec3 kS = F;
        // for energy conservation, the diffuse and specular light can't
        // be above 1.0 (unless the surface emits light); to preserve this
        // relationship the diffuse component (kD) should equal 1.0 - kS.
        vec3 kD = vec3(1.0) - kS;
        // multiply kD by the inverse metalness such that only non-metals 
        // have diffuse lighting, or a linear blend if partly metal (pure metals
        // have no diffuse light).
        kD *= 1.0 - metallic;	  

        // scale light by NdotL
        float NdotL = max(dot(normal, L), 0.0);        

        // add to outgoing radiance Lo
        Lo += (kD * albedo / c_Pi + specular) * radiance * NdotL;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
    }   
    
    // ambient lighting (note that the next IBL tutorial will replace 
    // this ambient lighting with environment lighting).
    vec3 ambient = vec3(0.03) * albedo * ao;

    vec3 color = ambient + Lo;

    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamma correct
    color = pow(color, vec3(1.0/2.2)); 

    out_Color = vec4(color, 1.0);
}