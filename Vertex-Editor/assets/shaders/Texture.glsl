#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;  // Vertex position
layout(location = 1) in vec4 a_Color;     // Vertex color
layout(location = 2) in vec2 a_TexCoord;  // Texture coordinates
layout(location = 3) in float a_TexIndex; // Texture index
layout(location = 4) in float a_TilingFactor; // Tiling factor
layout(location = 5) in int a_IsKey; // "Boolean" flag

uniform mat4 u_ViewProjection;  // View-projection matrix

out vec4 v_Color;      // Pass color to fragment shader
out vec2 v_TexCoord;   // Pass texture coordinates
flat out int v_TexIndex; // Pass texture index as integer
out vec4 v_ScreenParams;
uniform vec2 u_Resolution;
flat out int v_IsKey;       // Pass "boolean" flag to fragment shader
flat out int v_IsCRT;      // "Boolean" flag passed from vertex shader

void main()
{
    v_IsCRT = 0;
    v_ScreenParams = vec4(u_Resolution.x, u_Resolution.y, 1.0 - u_Resolution.x, 1.0 - u_Resolution.y);
    // Calculate the transformed vertex position
    gl_Position = u_ViewProjection * vec4(a_Position, 1.0);

    // Pass through data needed in the fragment shader
    v_Color = a_Color;
    v_TexCoord = a_TexCoord * a_TilingFactor;  // Apply tiling factor to texture coordinates
    v_TexIndex = int(a_TexIndex);  // Pass texture index as float
    v_IsKey = a_IsKey;  // Pass the "boolean" flag as an integer

    if(a_Position.z == -2)
    {
        v_IsCRT = 1;
    }
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec4 v_Color;          // Vertex color from vertex shader
in vec2 v_TexCoord;       // Texture coordinates from vertex shader
flat in int v_TexIndex;   // Texture index from vertex shader
in vec4 v_ScreenParams;   // Screen parameters
flat in int v_IsKey;      // "Boolean" flag passed from vertex shader
flat in int v_IsCRT;      // "Boolean" flag passed from vertex shader

uniform sampler2D u_Textures[32];  // Array of texture samplers
uniform vec2 u_Resolution;          // Resolution of the screen
uniform float u_Time;               // Time uniform
uniform float u_SinTime;            // Sin of time

// Vignette parameters
uniform float u_VignetteIntensity; // Intensity of the vignette effect
uniform float u_VignetteRadius;    // Radius of the vignette effect

void Unity_Dither_float4(vec4 In, vec2 ScreenPosition, out vec4 Out)
{
    vec2 uv = ScreenPosition * v_ScreenParams.xy;
    const float DITHER_THRESHOLDS[16] = float[](
        1.0 / 17.0, 9.0 / 17.0, 3.0 / 17.0, 11.0 / 17.0,
        13.0 / 17.0, 5.0 / 17.0, 15.0 / 17.0, 7.0 / 17.0,
        4.0 / 17.0, 12.0 / 17.0, 2.0 / 17.0, 10.0 / 17.0,
        16.0 / 17.0, 8.0 / 17.0, 14.0 / 17.0, 6.0 / 17.0
    );

    // Ensure explicit casting
    uint index = (uint(uv.x) % 4u) * 4u + (uint(uv.y) % 4u);
    Out = In - DITHER_THRESHOLDS[index];
}

// Pixelation: Adjust UV coordinates by pixel size
vec2 PixelateUV(vec2 uv, float pixelSize) {
    return floor(uv * pixelSize) / pixelSize;
}

// Pixelated Vignette with Dithering
float PixelatedDitheredVignette(vec2 uv, vec2 screenPosition, float pixelSize) {
    vec2 center = vec2(0.5, 0.5);  // Center of the screen
    vec2 pixelatedUV = PixelateUV(uv, pixelSize);  // Apply pixelation to the vignette UV
    float dist = distance(pixelatedUV, center);
    
    // Use sine wave to dynamically change the vignette radius
    float dynamicRadius = u_VignetteRadius + 0.05 * (sin(u_Time * 2.0) * 2 + 1.3); // Pulse effect
    float vignette = smoothstep(dynamicRadius, dynamicRadius - 0.1, dist);

    // Apply dithering to the vignette effect
    vec4 vignetteColor = vec4(vignette, vignette, vignette, 1.0);
    vec4 ditheredVignette;
    Unity_Dither_float4(vignetteColor, screenPosition, ditheredVignette);

    return ditheredVignette.r; // Return the dithered vignette factor
}

// Function to create a color shift effect
vec4 ColorShift(vec4 color) {
    float timeFactor = u_Time * 0.1; // Speed of color shift
    color.r += sin(timeFactor + 0.0) * 0.1; // Red channel shift
    color.g += sin(timeFactor + 2.0) * 0.1; // Green channel shift
    color.b += sin(timeFactor + 4.0) * 0.1; // Blue channel shift
    return clamp(color, 0.0, 1.0); // Clamp to ensure color stays within valid range
}

// Function to simulate a flickering effect
float FlickerEffect(float baseAlpha) {
    float flicker = 0.05 * sin(u_Time * 10.0); // Adjust flicker speed
    return clamp(baseAlpha + flicker, 0.0, 1.0); // Clamp the flicker
}

// Function to add scanlines
float Scanlines(float y) {
    return 0.5 + 0.5 * sin(y * 30.0 + u_Time * 5.0); // Creates horizontal lines effect
}

void main()
{
    float u_PixelSize = 32.0;
    // Pixelate UV coordinates for texture sampling
    vec2 pixelatedUV = PixelateUV(v_TexCoord, u_PixelSize);

    // Sample texture with pixelated UV coordinates
    vec4 pixelatedColor = texture(u_Textures[int(v_TexIndex)], pixelatedUV);

    // Calculate screen position in normalized coordinates
    vec2 screenPosition = gl_FragCoord.xy / u_Resolution;

    vec2 center = vec2(0.5, 0.5);  // Center of the screen
    float dist = distance(PixelateUV(screenPosition, u_PixelSize * (u_Resolution.x / u_Resolution.y)), center);

    if (dist >= u_VignetteRadius + 0.25 && v_IsKey == 0 && v_IsCRT == 0) {
        discard;
    }

    // Apply dithering
    vec4 ditheredColor;

    if(v_IsCRT == 0)
    {
        Unity_Dither_float4(pixelatedColor * v_Color * (u_Resolution.x / u_Resolution.y), screenPosition, ditheredColor);
    }

    if(v_IsCRT == 1)
    {
        ditheredColor = pixelatedColor * v_Color * (u_Resolution.x / u_Resolution.y);
    }
    



    if (v_IsKey == 0)
    {
        // Apply pixelated and dithered vignette
        float vignetteFactor = PixelatedDitheredVignette(screenPosition, screenPosition, u_PixelSize * (u_Resolution.x / u_Resolution.y));

        // Apply vignette to the dithered color
        ditheredColor.rgb *= vignetteFactor * u_VignetteIntensity;

        // Apply color shift effect
        ditheredColor = ColorShift(ditheredColor);

        // Apply flickering effect
        ditheredColor.a = FlickerEffect(ditheredColor.a);

        // Apply scanlines
        float scanlineFactor = Scanlines(screenPosition.y);
        ditheredColor.rgb *= scanlineFactor; // Modulate color by scanline effect
    }

    // Discard fragments with low alpha
    if (ditheredColor.a < 0.01) {
        discard;
    }

    // Set the final color output
    color = ditheredColor;
}
