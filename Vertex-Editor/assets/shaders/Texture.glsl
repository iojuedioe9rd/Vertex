// Basic Texture Shader

#type vertex
#version 450

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexIndex;
layout(location = 4) in float a_TilingFactor;
layout(location = 5) in int a_EntityID;

uniform mat4 u_ViewProjection;

out vec4 v_Color;
out vec2 v_TexCoord;
out flat int v_TexIndex;
out float v_TilingFactor;
out flat int v_EntityID;

void main()
{
	v_Color = a_Color;
	v_TexCoord = a_TexCoord;
	v_TexIndex = int(a_TexIndex);
	v_TilingFactor = a_TilingFactor;
	v_EntityID = a_EntityID;
	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

#type fragment
#version 450

layout(location = 0) out vec4 color;
layout(location = 1) out int color2;

in vec4 v_Color;
in vec2 v_TexCoord;
in flat int v_TexIndex;
in float v_TilingFactor;
in flat int v_EntityID;

uniform sampler2D u_Textures[32];

void main()
{
	color = texture(u_Textures[v_TexIndex], v_TexCoord * v_TilingFactor) * v_Color;

	if (color.a < 0.1)
		discard;
	
	color2 = v_EntityID;
}