#version 460 core

in vec3 FragColor;
uniform float Transparency;

out vec4 FinalColor;

void main()
{
	FinalColor = vec4(FragColor, 1.0f) * (Transparency);
}