#pragma once

#ifndef __SHADERLOADER_H__
#define __SHADERLOADER_H__

// Library Includes
#include <glew.h>
#include <glfw3.h>
#include <iostream>
#include <map>

#include<iostream>
#include<fstream>
#include<vector>

//Static class for loading shaders and creating programs for GLSL use.
class ShaderLoader
{
public:
	static GLuint CreateProgram(std::string VertexShaderFilename, std::string FragmentShaderFilename);

protected:
	ShaderLoader(void);
	~ShaderLoader(void);

	static GLuint CheckShader(GLenum shaderType, std::string shaderName);
	static GLuint CheckProgram(std::string programName, GLuint vShader, GLuint fShader);

	static GLuint CreateShader(GLenum shaderType, const char* shaderName);
	static std::string ReadShaderFile(const char* filename);
	static void PrintErrorDetails(bool isShader, GLuint id, const char* name);

	static const std::string ResourceFilePath;
	static std::map<std::string, GLuint> mShaderMap;
};

#endif // !__SHADERLOADER_H__