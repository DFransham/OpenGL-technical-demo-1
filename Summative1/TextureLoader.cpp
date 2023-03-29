#include "TextureLoader.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

const std::string TextureLoader::TextureFilePath = "Resources/Textures/";
std::map<std::string, GLuint> TextureLoader::mTextureMap;

TextureLoader::TextureLoader(void) {}
TextureLoader::~TextureLoader(void) {}

//Checks if texture is already loaded into map.  Creates new one if it doesn't already exist.
GLuint TextureLoader::CheckTexture(std::string _TextureName)
{
	if (mTextureMap.count(_TextureName) > 0)
	{
		return mTextureMap[_TextureName];
	}
	else
	{
		GLuint NewTexture = LoadTexture(_TextureName);
		mTextureMap.insert(std::pair<std::string, GLuint>(_TextureName, NewTexture));
		return NewTexture;
	}
}

//Reads the image file and loads to Texture information for use.
GLuint TextureLoader::LoadTexture(std::string _TextureName)
{
	//Load the Image data
	int ImageWidth;
	int ImageHeight;
	int ImageComponents;

	//check if map contains file name first
	std::string FullTextureName = TextureFilePath + _TextureName;
	unsigned char* ImageData = stbi_load(FullTextureName.c_str(), &ImageWidth, &ImageHeight, &ImageComponents, 0);

	GLuint NewTexture;
	glGenTextures(1, &NewTexture);
	glBindTexture(GL_TEXTURE_2D, NewTexture);

	//Check how many components the loaded image has (RGB or RGBA?)
	GLint LoadedComponents = (ImageComponents == 4) ? GL_RGBA : GL_RGB;

	//Populate the texture with the image data
	glTexImage2D(GL_TEXTURE_2D, 0, LoadedComponents, ImageWidth, ImageHeight, 0, LoadedComponents, GL_UNSIGNED_BYTE, ImageData);

	//Setting the filtering, mipmap and address mode parameters for this texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//Replace previous two lines with this if clamp to border is desired:
	//GLfloat BorderColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, BorderColor);

	//Generate the mipmaps, free the memory and unbind the texture
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(ImageData);
	glBindTexture(GL_TEXTURE_2D, 0);

	return NewTexture;
}

//Sets STBI flip mode to the desired mode.
void TextureLoader::SetStbiFlip(GLboolean _bFlipMode)
{
	stbi_set_flip_vertically_on_load(_bFlipMode);
}