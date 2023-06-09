#include "ShaderLoader.h" 

const std::string ShaderLoader::ResourceFilePath = "Resources/Shaders/";
std::map<std::string, GLuint> ShaderLoader::mShaderMap;

ShaderLoader::ShaderLoader(void) {}
ShaderLoader::~ShaderLoader(void) {}

//Checks the map to see if shader file has already been loaded.  Loads it if it hasn't already been loaded
GLuint ShaderLoader::CheckShader(GLenum shaderType, std::string _ShaderName)
{
	if (mShaderMap.count(_ShaderName) > 0)
	{
		return mShaderMap[_ShaderName];
	}
	else
	{
		std::string fullShaderName = ResourceFilePath + _ShaderName;
		GLuint newShader = CreateShader(shaderType, fullShaderName.c_str());
		mShaderMap.insert(std::pair<std::string, GLuint>(_ShaderName, newShader));
		return newShader;
	}
}
//Checks the map to see if the program has already been loaded.  Creates it if it doesn't already exist.
GLuint ShaderLoader::CheckProgram(std::string programName, GLuint vShader, GLuint fShader)
{
	if (mShaderMap.count(programName) > 0)
	{
		return mShaderMap[programName];
	}
	else
	{
		GLuint program = glCreateProgram();
		glAttachShader(program, fShader);
		glAttachShader(program, vShader);
		glLinkProgram(program);
		mShaderMap.insert(std::pair<std::string, GLuint>(programName, program));
		return program;
	}
}

//Creates a program to load the two given shaders into
GLuint ShaderLoader::CreateProgram(std::string vertexShaderFilename, std::string fragmentShaderFilename)
{
	// Create the shaders from the filepath
	GLuint vShader = CheckShader(GL_VERTEX_SHADER, vertexShaderFilename);
	GLuint fShader = CheckShader(GL_FRAGMENT_SHADER, fragmentShaderFilename);

	// Create the program handle, attach the shaders and link it
	std::string programName = vertexShaderFilename + fragmentShaderFilename;
	GLuint program = CheckProgram(programName, vShader, fShader);

	// Check for link errors
	int link_result = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &link_result);
	if (link_result == GL_FALSE)
	{
		PrintErrorDetails(false, program, programName.c_str());
		return 0;
	}
	return program;
}

//Creates a shader from given file name, to load into program being created
GLuint ShaderLoader::CreateShader(GLenum shaderType, const char* shaderName)
{
	// Read the shader files and save the source code as strings
	std::string shaderSourceCode = ReadShaderFile(shaderName);

	// Create the shader ID and create pointers for source code string and length
	GLuint shaderID = glCreateShader(shaderType);
	const GLchar* source_cstr = shaderSourceCode.c_str();
	const GLint sourceLen = shaderSourceCode.size();

	// Populate the Shader Object (ID) and compile
	glShaderSource(shaderID, 1, &source_cstr, &sourceLen);
	glCompileShader(shaderID);

	// Check for errors
	int compile_result = 0;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compile_result);
	if (compile_result == GL_FALSE)
	{
		PrintErrorDetails(true, shaderID, shaderName);
		return 0;
	}
	return shaderID;
}

//Reads the shader file into memory
std::string ShaderLoader::ReadShaderFile(const char* filename)
{
	// Open the file for reading
	std::ifstream file(filename, std::ios::in);
	std::string shaderCode;

	// Ensure the file is open and readable
	if (!file.good()) {
		std::cout << "Cannot read file:  " << filename << std::endl;
		return "";
	}

	// Determine the size of of the file in characters and resize the string variable to accomodate
	file.seekg(0, std::ios::end);
	shaderCode.resize((unsigned int)file.tellg());

	// Set the position of the next character to be read back to the beginning
	file.seekg(0, std::ios::beg);
	// Extract the contents of the file and store in the string variable
	file.read(&shaderCode[0], shaderCode.size());
	file.close();
	return shaderCode;
}

//Prints load error messages if system could not properly load the shaders
void ShaderLoader::PrintErrorDetails(bool isShader, GLuint id, const char* name)
{
	int infoLogLength = 0;
	// Retrieve the length of characters needed to contain the info log
	(isShader == true) ? glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infoLogLength) : glGetProgramiv(id, GL_INFO_LOG_LENGTH, &infoLogLength);
	std::vector<char> log(infoLogLength);

	// Retrieve the log info and populate log variable
	(isShader == true) ? glGetShaderInfoLog(id, infoLogLength, NULL, &log[0]) : glGetProgramInfoLog(id, infoLogLength, NULL, &log[0]);
	std::cout << "Error compiling " << ((isShader == true) ? "shader" : "program") << ": " << name << std::endl;
	std::cout << &log[0] << std::endl;
}