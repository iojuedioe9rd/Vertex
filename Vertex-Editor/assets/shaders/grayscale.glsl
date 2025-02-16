#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;


struct VertexOutput
{
	vec3 Position;
	vec2 TexCoord;
};

layout (location = 0) out VertexOutput Output;

void main()
{
	Output.Position = a_Position;
	Output.TexCoord = a_TexCoord;

	gl_Position = vec4(a_Position, 1.0);


}

#type fragment
#version 450 core
layout(location = 0) out vec4 o_Colour;

struct VertexOutput
{
	vec3 Position;
	vec2 TexCoord;
};

layout (location = 0) in VertexOutput Input;

layout (binding = 0) uniform sampler2D u_Texture;



void main()
{
	vec4 texColour = texture(u_Texture, Input.TexCoord);
	float gray = dot(texColour.rgb, vec3(0.299, 0.587, 0.114));
	o_Colour = vec4(gray, gray, gray, 1.0);
}