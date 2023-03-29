#pragma once

#ifndef __SHAPERENDERER_H__
#define __SHAPERENDERER_H__
//that's shape renderer, not shaper enderer.

#include "ShapeObject.h"
#include "TextureLoader.h"
#include "ShaderLoader.h"
#include "Camera.h"

#pragma region StaticArraysAndEnums
/*
* This region contains static arrays and enum values
* to more easily create and use programs etc without
* having to remember which integer goes with program etc
*/
static GLuint Programs[16];

enum class EProgramNames {
	P_FIXED_TRI = 0,
	P_POSITION_ONLY = 1,
	P_VERTEX_COLOR = 2,
	P_COLOR_FADE = 3,
	P_TEXTURE = 4,
	P_TEXTURE_MIX = 5,
	P_ANIMATE_SPRITE = 6,
	P_TEXTURE_MIX_FADE = 7,
	P_WORLD_SPACE = 8,
	P_CLIP_SPACE = 9,
	P_WORLD_TEXTURE_FADE = 10,
	P_CLIP_TEXTURE_FADE = 11,
	P_WORLD_COLOR_FADE = 12,
	P_CLIP_COLOR_FADE = 13,
	P_WORLD_ANIMATE = 14,
	P_CLIP_ANIMATE = 15,
};

static string VShaderNames[] = {
	"FixedTriangle.vs",	//0
	"PositionOnly.vs",	//1
	"VertexColor.vs",	//2
	"NDC_Texture.vs",	//3
	"SpriteAnimate.vs",	//4
	"WorldSpace.vs",	//5
	"ClipSpace.vs",		//6
};

enum class EVShaderNames {
	VS_FIXED_TRIANGLE = 0,
	VS_POSITION_ONLY = 1,
	VS_VERTEX_COLOR = 2,
	VS_NDC_TEXTURE = 3,
	VS_SPRITE_ANIMATE = 4,
	VS_WORLD_SPACE = 5,
	VS_CLIP_SPACE = 6,
};

static string FShaderNames[] = {
	"FixedColor.fs",		//0
	"VertexColor.fs",		//1
	"VertexColorFade.fs",	//2
	"Texture.fs",			//3
	"TextureMix.fs",		//4
	"TextureMixFade.fs",	//5
	"Animated_Texture.fs"	//6
};

enum class EFShaderNames {
	FS_FIXED_COLOR = 0,
	FS_VERTEX_COLOR = 1,
	FS_VERTEX_COLOR_FADE = 2,
	FS_TEXTURE = 3,
	FS_TEXTURE_MIX = 4,
	FS_TEXTURE_MIX_FADE = 5,
	FS_ANIMATED_TEXTURE = 6,
};

static GLuint Textures[8];

static string ImageNames[] = {
	"Rayman.jpg",			//0
	"AwesomeFace.png",		//1
	"Capguy_Walk.png",		//2
	"BlueTorch.jpg",		//3
	"DunluceRuins.jpg",		//4
	"DunluceRebuilt.jpg",	//5
	"ElectronPad.png",		//6
	"ElectronFlight.png",	//7
};

enum class EImageNames {
	T_RAYMAN = 0,
	T_AWESOME_FACE = 1,
	T_CAPGUY_WALK = 2,
	T_BLUETORCH = 3,
	T_DUNLUCE_RUINS = 4,
	T_DUNLUCE_REBUILT = 5,
	T_ELECTRON_FLYING = 6,
	T_ELECTRON_PAD = 7,
};

#pragma endregion

class cShapeRenderer
{
protected:
	//Setup functions. Included in this class because it is controlling the use of the programs and textures.
	static void LoadPrograms();
	static void LoadTextures();
	
	//Graphics identifiers / setup parameters etc
	GLuint miProgramID;
	GLuint miVAO;
	GLuint miVBO;
	GLuint miEBO;

	cCamera* mpMyCamera = nullptr;
	cShapeObject* mpMyShape = nullptr;

	EShapeType meShapeType;
	EProgramNames meProgramType;

	int miNumVertices;

	//Needs to be initialised properly if relevant shapes or programs are used
	GLuint miTexture0 = 0;
	GLuint miTexture1 = 0;

	//Needed for any object that has any sort of animation
	float mfAccumulatedDelta = 0.0f;	
	float mfTargetFPS = 25.0f;

	//Needed for faders
	float mfTransparency = 0.5f;

	//Needed for rotating objects
	float mfRotationPerFrame = 0.0f;
	
	//Needed for sprite animation - defaults are set for blue flame as it's the only one I'm using for now
	bool mbTraverseRight = true;
	bool mbLoopSprite = false;
	int miFramesInSprite = 11;
	int miFrameCounter = 0;
	float mfSpriteWidth = 390.0f + 206.0f;
	float mfTextureWidth = 7000.0f;
	GLfloat TexOffset[8] = {
		0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f,
	};

	//Called from Update or Render to help perform their functions as necessary
	void BindAndUseTexture(int _TextureNum, GLuint _TextureID);
	bool CalculateNextFrame();

	//Called during updates to calculate or render various changes
	void UpdateTexOffset();
	void CalculateSpriteChanges();
	void UpdateRotMat();
	void UpdateFader();

public:
	//Called to load programs and textures.  Included in this class because it is controlling the use of the programs and textures.
	static void SetupRenderers();

	//Construct/Destruct
	cShapeRenderer(EShapeType _ShapeToMake, GLuint _VAO, GLuint _EBO, GLuint _VBO, GLenum _StaticOrDynamic, EProgramNames _ProgramToUse, cCamera* _Camera);
	~cShapeRenderer();
	
	//Called from main loop
	void Update(float _fDeltaTime);
	void RenderShape();

	/*
	* The following functions are for setting up the various variables for different operations
	* because it's impractical to obtain all these values at once by constructor
	*/
	//Wrapper functions to get values from main and update shape object
	void SetObjPosition(glm::vec3 _NewPos) { mpMyShape->SetObjPosition(_NewPos); };
	void SetObjRotation(float _NewRot) { mpMyShape->SetObjRotation(_NewRot); };
	void SetObjScale(glm::vec3 _NewScale) { mpMyShape->SetObjScale(_NewScale); };

	void SetTexture(bool _SecondTexture, EImageNames _DesiredTexture);
	void SetFPS(float _fFPS) { mfTargetFPS = _fFPS; };
	void SetTransparency(float _fTransparency) { mfTransparency = _fTransparency; };
	void SetFrameRotation(float _fRotPerFrame) { mfRotationPerFrame = _fRotPerFrame; };

	//Sprite animation-specific functions
	void SetFrames(int _iFrames) { miFramesInSprite = _iFrames; };
	void SetSpriteWidth(float _fNewWidth) { mfSpriteWidth = _fNewWidth; };
	void SetTextureWidth(float _fNewWidth) { mfTextureWidth = _fNewWidth; };
	void SetSpriteLoopMode(bool _bLoopFromBeginning) { mbLoopSprite = _bLoopFromBeginning; };
	void ChangeVertexArray(GLfloat* _NewVertArray, int _NewArrSize, GLenum _StaticOrDynamic);
};

#endif // !__SHAPERENDERER_H__

