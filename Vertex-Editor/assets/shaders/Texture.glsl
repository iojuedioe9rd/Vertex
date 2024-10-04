#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;  // Vertex position
layout(location = 1) in vec4 a_Color;     // Vertex color
layout(location = 2) in vec2 a_TexCoord;  // Texture coordinates
layout(location = 3) in float a_TexIndex; // Texture index
layout(location = 4) in float a_TilingFactor; // Tiling factor

uniform mat4 u_ViewProjection;  // View-projection matrix

out vec4 v_Color;      // Pass color to fragment shader
out vec2 v_TexCoord;   // Pass texture coordinates
flat out int v_TexIndex; // Pass texture index as integer

void main()
{
    // Calculate the transformed vertex position
    gl_Position = u_ViewProjection * vec4(a_Position, 1.0);

    // Pass through data needed in the fragment shader
    v_Color = a_Color;
    v_TexCoord = a_TexCoord * a_TilingFactor;  // Apply tiling factor to texture coordinates
    v_TexIndex = int(a_TexIndex);  // Pass texture index as float
}
#type fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec4 v_Color;         // Vertex color from vertex shader
in vec2 v_TexCoord;      // Texture coordinates from vertex shader
flat in int v_TexIndex; // Texture index from vertex shader

uniform sampler2D u_Textures[32];  // Array of texture samplers

void main()
{
    // Pre-calculate the texture index lookup (cast to integer once)
    int texIndex = int(v_TexIndex);

    // Sample the texture using the calculated index and texture coordinates
    vec4 textureColor = texture(u_Textures[texIndex], v_TexCoord);

    // Apply the vertex color to the texture color
    vec4 finalColor = textureColor * v_Color;

    // If the final color is fully transparent or nearly invisible, discard the fragment
    if (finalColor.a < 0.01) {
        discard;
    }

    // Set the final color output
    color = finalColor;
}