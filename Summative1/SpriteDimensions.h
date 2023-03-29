#pragma once

#include <glew.h>

//Not a class file - simply a header to collate specific variables for sprite sheets for animation purposes, to keep other code a bit tidier.
//Only has one sprite in this iteration, but could hold as many sprites as required

//Most of these variables are not used in this iteration of the program, as I have used them as the default values in the shape renderer.
//If more sprites were needed then these values would be used to update the default variables.
bool BT_bLoopSprite = false;
int BT_iFramesInSprite = 11;
float BT_fSpriteWidth = 390.0f + 206.0f;
float BT_fTextureWidth = 7000.0f;
int BT_ArrSize = 32;

GLfloat BT_Vertices[] = {
	//Index		//Position				//Colour				//Texture Coords
	/* 0 */		-0.15f,  0.4f, 0.0f,		1.0f, 0.0f, 0.0f,		(221.0f / 7000.0f), (2226.0f / 2626.0f), 							//Top - Left
	/* 1 */		-0.15f, -0.4f, 0.0f,		0.0f, 1.0f, 0.0f,		(221.0f / 7000.0f),  (400.0f / 2626.0f), 							//Bot - Left
	/* 2 */		 0.15f, -0.4f, 0.0f,		1.0f, 0.0f, 1.0f,		((221.0f + BT_fSpriteWidth) / 7000.0f),  (400.0f / 2626.0f),		//Bot - Right
	/* 3 */		 0.15f,  0.4f, 0.0f,		0.0f, 1.0f, 1.0f,		((221.0f + BT_fSpriteWidth) / 7000.0f), (2226.0f / 2626.0f),		//Top - Right
};
