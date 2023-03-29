#version 460 core

//Vertex Shader Inputs
in vec2 FragTexCoords;
in vec3 FragColor;

//Uniform Inputs
uniform sampler2D ImageTexture;
uniform vec2 TexOffset;

//Output
out vec4 FinalColor;

void main()
{
	FinalColor = texture(ImageTexture, FragTexCoords + TexOffset);
}