#version 410 core

#extension GL_GOOGLE_include_directive : enable

#pragma inject_attributes

precision mediump float;

#include "snippets/frag_global_params.glsl"
#include "snippets/frag_global_fog.glsl"

struct PhongMaterial {
    vec3 DiffuseColor;
    vec3 SpecularColor;
    float Shininess;
};

uniform PhongMaterial u_Material;
uniform vec3 u_AmbientLight;

vec3 phongShading(
    const in vec3 light_dir,
    const in vec3 light_color,
    const in vec3 normal,
    const in vec3 diffuse_color
) {
    float diffuse_factor = max(dot(light_dir, normal), 0.0);
    vec3 diffuse = light_color * diffuse_color * diffuse_factor;

    // If the diffuse factor is zero, the light is facing away from the surface
    // and no light contribution should be calculated, so we skip specular calculation.
    vec3 specular = vec3(0.0);
    if (diffuse_factor > 0.0) {
        vec3 halfway = normalize(light_dir + v_ViewDir);
        specular = light_color * u_Material.SpecularColor *
            pow(max(dot(halfway, normal), 0.0), max(u_Material.Shininess, 1.0));
    }

    return diffuse + specular;
}

#if NUM_LIGHTS > 0

struct Light {
    int Type; // 1 = directional, 2 = point, 3 = spot
    vec3 Color;
    vec3 Position;
    vec3 Direction;
    float ConeCos;
    float PenumbraCos;
    float Base;
    float Linear;
    float Quadratic;
};

layout(std140) uniform ub_Lights {
    Light u_Lights[NUM_LIGHTS];
};

float attenuation(in float dist, in Light light) {
    float denominator = light.Base +
                        light.Linear * dist +
                        light.Quadratic * (dist * dist);
    return clamp(1.0 / max(denominator, 0.01), 0.02, 1.0);
}

vec3 processLights(const in vec3 normal, const in vec3 diffuse_color) {
    vec3 output_color = vec3(0.0);
    for (int i = 0; i < NUM_LIGHTS; i++) {
        Light light = u_Lights[i];

        if (light.Type == 1 /* directional light */) {
            output_color += phongShading(light.Direction, light.Color, normal, diffuse_color);
        }

        if (light.Type == 2 /* point light */) {
            vec3 light_dir = normalize(light.Position - v_Position.xyz);
            float dist = length(light.Position - v_Position.xyz);
            output_color += attenuation(dist, light) * phongShading(light_dir, light.Color, normal, diffuse_color);
        }

        if (light.Type == 3 /* spot light */) {
            vec3 light_dir = normalize(light.Position - v_Position.xyz);
            float dist = length(light.Position - v_Position.xyz);
            float angle_cos = dot(light_dir, light.Direction);
            if (angle_cos > light.ConeCos) {
                vec3 spot_color = light.Color * smoothstep(light.ConeCos, light.PenumbraCos, angle_cos);
                output_color += attenuation(dist, light) * phongShading(light_dir, spot_color, normal, diffuse_color);
            }
        }
    }
    return output_color;
}

#endif

void main() {
    #include "snippets/frag_main_normal.glsl"

    vec3 diffuse_color = u_Material.DiffuseColor;
    float opacity = u_Opacity;

    #ifdef USE_INSTANCING
        diffuse_color *= v_InstanceColor;
    #endif

    #ifdef USE_VERTEX_COLOR
        diffuse_color *= v_Color;
    #endif

    #ifdef USE_ALBEDO_MAP
        vec4 texture_sample = texture(u_AlbedoMap, v_TexCoord);
        diffuse_color *= texture_sample.rgb;
        opacity *= texture_sample.a;
    #endif

    #ifdef USE_ALPHA_MAP
        vec4 alpha_sample = texture(u_AlphaMap, v_TexCoord);
        opacity *= alpha_sample.r;
    #endif

    vec3 output_color = diffuse_color * u_AmbientLight;
    #if NUM_LIGHTS > 0
        output_color += processLights(normal, diffuse_color);
        output_color = clamp(output_color, 0.0, 1.0);
    #endif

    #ifdef USE_FOG
        applyFog(output_color, v_ViewDepth);
    #endif

    v_FragColor = vec4(output_color, opacity);
}