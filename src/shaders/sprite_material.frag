#version 410 core

#extension GL_GOOGLE_include_directive : enable

#pragma inject_attributes

precision mediump float;

#include "snippets/frag_global_params.glsl"
#include "snippets/frag_global_fog.glsl"

void main() {
    vec3 output_color = u_Color;
    float opacity = u_Opacity;

    #ifdef USE_INSTANCING
        output_color *= v_InstanceColor;
    #endif

    #ifdef USE_VERTEX_COLOR
        output_color *= v_Color;
    #endif

    #ifdef USE_ALBEDO_MAP
        output_color *= texture(u_AlbedoMap, v_TexCoord).rgb;
        opacity *= texture(u_AlbedoMap, v_TexCoord).a;
    #endif

    #ifdef USE_ALPHA_MAP
        vec4 alpha_sample = texture(u_AlphaMap, v_TexCoord);
        opacity *= alpha_sample.r;
    #endif

    #ifdef USE_FOG
        applyFog(output_color, v_ViewDepth);
    #endif

    v_FragColor = vec4(output_color, opacity);
    v_FragColor = clamp(v_FragColor, 0.0, 1.0);
}