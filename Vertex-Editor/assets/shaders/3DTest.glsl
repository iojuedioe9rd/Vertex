#type vertex
#version 330 core

// Positions/Coordinates
layout (location = 0) in vec3 a_Pos;
// Normals (not necessarily normalized)
layout (location = 1) in vec3 a_Normal;
// Texture Coordinates
layout (location = 2) in vec2 a_Tex;
// Colors
layout (location = 3) in vec4 a_Color;



// Outputs the current position for the Fragment Shader
out vec3 crntPos;
// Outputs the normal for the Fragment Shader
out vec3 Normal;
// Outputs the color for the Fragment Shader
out vec4 color;
// Outputs the texture coordinates to the Fragment Shader
out vec2 texCoord;



// Imports the camera matrix from the main function
uniform mat4 camMatrix;
// Imports the model matrix from the main function
uniform mat4 model;


void main()
{
	// calculates current position
	crntPos = vec3(model * vec4(a_Pos, 1.0f));
	// Assigns the normal from the Vertex Data to "Normal"
	Normal = a_Normal;
	// Assigns the colors from the Vertex Data to "color"
	color = a_Color;
	// Assigns the texture coordinates from the Vertex Data to "texCoord"
	texCoord = a_Tex;
	
	// Outputs the positions/coordinates of all vertices
	gl_Position = camMatrix * vec4(crntPos, 1.0);
}

#type fragment
#version 330 core

out vec4 FragColor;

uniform vec4 lightColor;

void main()
{
	FragColor = lightColor;
}