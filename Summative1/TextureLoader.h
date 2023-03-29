#pragma once

#ifndef __TEXTURELOADER_H__
#define __TEXTURELOADER_H__

#include <glew.h>
#include <iostream>
#include <map>

using std::string;

//Static class used for loading textures using stbi.

class TextureLoader
{
public:
	static GLuint CheckTexture(std::string _sTextureName);
	static void SetStbiFlip(GLboolean _bFlipMode);

protected:
	TextureLoader(void);
	~TextureLoader(void);

	static const std::string TextureFilePath;
	static std::map<std::string, GLuint> mTextureMap;
	static GLuint LoadTexture( std::string _TextureName);
};


#endif // !__TEXTURELOADER_H__
