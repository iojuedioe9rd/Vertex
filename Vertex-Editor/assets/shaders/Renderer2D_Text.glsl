// MSDF text shader

#type vertex
#version 450 core

// Ensure locations are correctly set
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in int a_EntityID;

layout(std140, binding = 0) uniform Camera
{
    mat4 u_ViewProjection;
};

// Explicitly declare the output variables with locations
layout(location = 0) out vec4 v_Color;
layout(location = 1) out vec2 v_TexCoord;
layout(location = 2) out flat int v_EntityID;

void main()
{
    v_Color = a_Color;
    v_TexCoord = a_TexCoord;
    v_EntityID = a_EntityID;

    gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}



#type fragment
#version 450 core

layout(location = 0) out vec4 o_Color;
layout(location = 1) out int o_EntityID;

layout(location = 0) in vec4 v_Color;
layout(location = 1) in vec2 v_TexCoord;
layout(location = 2) in flat int v_EntityID;

layout(binding = 0) uniform sampler2D u_FontAtlas;

layout(std140, binding = 1) uniform Data
{
    float time;
} u_Data;

float screenPxRange() {
    const float pxRange = 2.0; // set to distance field's pixel range
    vec2 unitRange = vec2(pxRange) / vec2(textureSize(u_FontAtlas, 0));
    vec2 screenTexSize = vec2(1.0) / fwidth(v_TexCoord);
    return max(0.5 * dot(unitRange, screenTexSize), 1.0);
}

float median(float r, float g, float b) {
    return max(min(r, g), min(max(r, g), b));
}

void main()
{
    float colorF = 0.5 + 0.5 * sin(u_Data.time) * (v_TexCoord.x / v_TexCoord.y);
    vec3 color = vec3(colorF, 1.0 - colorF, sin(u_Data.time * 2.0) * 0.5 + 0.5);
    
    // Option 1: Multiply only the RGB components
    // vec4 texColor = vec4(color * texture(u_FontAtlas, v_TexCoord).rgb, texture(u_FontAtlas, v_TexCoord).a);
    
    // Option 2: Convert 'color' to vec4 before multiplying
    vec4 texColor = vec4(color, 1.0) * texture(u_FontAtlas, v_TexCoord);

    vec3 msd = texture(u_FontAtlas, v_TexCoord).rgb;
    float sd = median(msd.r, msd.g, msd.b);
    float screenPxDistance = screenPxRange() * (sd - 0.5);
    float opacity = clamp(screenPxDistance + 0.5, 0.0, 1.0);
    if (opacity == 0.0)
        discard;

    vec4 bgColor = vec4(0.0);
    o_Color = mix(bgColor, vec4(color, 1.0), opacity);
    if (o_Color.a == 0.0)
        discard;

    o_EntityID = v_EntityID;
}
