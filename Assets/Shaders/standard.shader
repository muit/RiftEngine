## VERTEX ##

#version 330 core

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec2 v_uv;

uniform mat4 mvp;
uniform mat4 model;

out vec3 normal;
out vec3 world_position;
out vec2 uv;

void main()
{
	vec4 v_position4 = vec4(v_position, 1.0);

	world_position =  vec3(model * v_position4);
	gl_Position = mvp * v_position4;

    normal = mat3(model) * v_normal;
	uv = v_uv;
}


## FRAGMENT ##

#version 330 core

// Lighting Data
#define MAX_DIRLIGHTS 10
uniform int num_directional_lights;
struct DirectionalLight {
    vec3 direction;
    vec3 color;
};
#define MAX_POINTLIGHTS 10
uniform int num_point_lights;
struct PointLight {
    vec3 position;
    vec3 color;
};


in vec3 normal;
in vec3 world_position;
in vec2 uv;

// Material Parameters
//uniform sample2D baseColor;
//uniform sample2D rmao;
uniform float metallic = 0.04;
uniform float roughness = 0.85;
uniform float ao = 1.0;

uniform vec3 camera_position;
uniform DirectionalLight directional_lights[MAX_DIRLIGHTS];
uniform PointLight point_lights[MAX_POINTLIGHTS];



const float PI = 3.14159265359;

// ----------------------------------------------------------------------------
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / max(denom, 0.001); // prevent divide by zero for roughness=0.0 and NdotH=1.0
}
// ----------------------------------------------------------------------------
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
// ----------------------------------------------------------------------------
vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}
// ----------------------------------------------------------------------------


// ----------------------------------------------------------------------------
vec3 ApplyDirectionalLight(DirectionalLight light, vec3 baseColor, vec3 normal, float metallic, vec3 F0, vec3 N, vec3 V)
{    
    // Calculate per-light radiance
    vec3 L = normalize(light.direction);
    vec3 H = normalize(V + L);
    vec3 radiance = light.color;

    // Cook-Torrance BRDF
    float NDF = DistributionGGX(N, H, roughness);   
    float G   = GeometrySmith(N, V, L, roughness);      
    vec3  F   = FresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);
        
    vec3 nominator    = NDF * G * F; 
    float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
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
    float NdotL = max(dot(N, L), 0.0);        

    // Return outgoing radiance Lo
    return (kD * baseColor / PI + specular) * radiance * NdotL;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
}

vec3 ApplyPointLight(PointLight light, vec3 baseColor, vec3 normal, float metallic, vec3 F0, vec3 N, vec3 V)
{    
    // Calculate per-light radiance
    vec3 L = normalize(light.position - world_position);
    vec3 H = normalize(V + L);
    float distance = length(light.position - world_position);
    float attenuation = 1.0 / (distance * distance);
    vec3 radiance = light.color * attenuation;

    // Cook-Torrance BRDF
    float NDF = DistributionGGX(N, H, roughness);   
    float G   = GeometrySmith(N, V, L, roughness);      
    vec3  F   = FresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);
        
    vec3 nominator    = NDF * G * F; 
    float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
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
    float NdotL = max(dot(N, L), 0.0);        

    // Return outgoing radiance Lo
    return (kD * baseColor / PI + specular) * radiance * NdotL;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
}
// ----------------------------------------------------------------------------


void main()
{
    // #TODO: Replace with textures
    vec3 baseColor = vec3(0.9, 0.1, 0.1);


    vec3 N = normalize(normal);
    vec3 V = normalize(camera_position - world_position);

    // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)    
    vec3 F0 = mix(vec3(0.04), baseColor, metallic);
    
    vec3 Lo = vec3(0.0);
    for(int i = 0; i < num_directional_lights; ++i) 
    {
        Lo += ApplyDirectionalLight(directional_lights[i], baseColor, normal, metallic, F0, N, V);
    }
    for(int i = 0; i < num_point_lights; ++i) 
    {
        Lo += ApplyPointLight(point_lights[i], baseColor, normal, metallic, F0, N, V);
    }

    // ambient lighting (note that the next IBL tutorial will replace 
    // this ambient lighting with environment lighting).
    vec3 ambient = vec3(0.3) * baseColor * ao;


    vec3 color = ambient + Lo;

    color = color / (color + vec3(1.0)); // HDR tonemapping
    color = pow(color, vec3(1.0/2.2)); // Gamma correction

	gl_FragColor = vec4(color, 1.0);
}
