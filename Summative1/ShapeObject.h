#pragma once
#ifndef __SHAPEOBJECT_H__
#define __SHAPEOBJECT_H__

#include <glew.h>
#include <glfw3.h>
#include <iostream>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#pragma region DefaultArrays
/*
* This region contains static array variables containing the default
* vertex and index arrays for the shapes that I have implemented so far.
* Unless specific coordinates are required, default values can be used and
* modified using object matrices later.
*/
static const GLfloat DefaultTriVertices[] = {
	//Index		//Position				//Colour				//Texture Coords
	/* 0 */		 0.0f,  0.5f, 0.0f,		1.0f, 0.0f, 0.0f,		0.5f, 1.0f, 	//Top - Middle
	/* 1 */		-0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,		0.0f, 0.0f, 	//Bot - Left
	/* 2 */		 0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f,		1.0f, 0.0f, 	//Bot - Right
};
static const GLuint DefaultTriIndices[] = {
	0, 1, 2,	//First Triangle	(TM -> BL -> BR)
};
static const GLfloat DefaultQuadVertices[] = {
	//Index		//Position				//Colour				//Texture Coords
	/* 0 */		-0.5f,  0.5f, 0.0f,		1.0f, 0.0f, 0.0f,		0.0f, 1.0f, 	//Top - Left
	/* 1 */		-0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,		0.0f, 0.0f, 	//Bot - Left
	/* 2 */		 0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 1.0f,		1.0f, 0.0f, 	//Bot - Right
	/* 3 */		 0.5f,  0.5f, 0.0f,		0.0f, 1.0f, 1.0f,		1.0f, 1.0f, 	//Top - Right
};
static const GLuint DefaultQuadIndices[] = {
	0, 1, 2,	//First Triangle	(TL -> BL -> BR)
	0, 2, 3,	//Second Triangle	(TL -> BR -> TR)
};
static const GLfloat DefaultHexVertices[] = {
	//Index		//Position										//Colour				//Texture Coords
	/* 0 */		-(sqrt(3.0f) *0.5f) /2,		 0.5f / 2, 0.0f,		0.5f, 0.5f, 0.5f,		0.5f - sqrt(3.0f) / 4,	0.75f, 	//Top - Left
	/* 1 */		-(sqrt(3.0f) *0.5f) /2,		-0.5f / 2, 0.0f,		0.0f, 1.0f, 0.0f,		0.5f - sqrt(3.0f) / 4,	0.25f, 	//Bot - Left
	/* 2 */		 0.0f,						-0.5f,	   0.0f,		0.5f, 0.5f, 0.5f,		0.5f,	0.0f,					//Bot - Middle
	/* 3 */		 (sqrt(3.0f) *0.5f) / 2,	-0.5f / 2, 0.0f,		0.0f, 0.0f, 1.0f,		0.5f + sqrt(3.0f) / 4,	0.25f, 	//Bot - Right
	/* 4 */		 (sqrt(3.0f) *0.5f) / 2,	 0.5f / 2, 0.0f,		0.5f, 0.5f, 0.5f,		0.5f + sqrt(3.0f) / 4,	0.75f, 	//Top - Right
	/* 5 */		 0.0f,						 0.5f,     0.0f,		1.0f, 0.0f, 0.0f,		0.5f,	1.0f, 					//Top - Middle
};
static const GLuint DefaultHexIndices[] = {
	0, 1, 2,		//First:	TL -> BL -> BM
	2, 3, 4,		//Second:	BM -> BR -> TR
	4, 5, 0,		//Third:	TR -> TM -> TL
	0, 2, 4,		//Middle:	TL -> BM -> TR
};
#pragma endregion

//Enum value assigned as the number of vertices the relevant shape holds
enum class EShapeType {
	S_TRIANGLE = 3,
	S_QUAD = 4,
	S_HEX = 6,
};

//Object class for textures or other programs to be rendered on.
class cShapeObject
{
protected:
	GLfloat* mpVertices = nullptr;
	GLuint* mpIndices = nullptr;
	int miIndexArraySize = 0;
	int miVertexArraySize = 0;
	EShapeType mShapeType;

	/* Suitable for Orthographic projection
	float ObjRotationAngle = 0.0f;
	glm::vec3 ObjPosition = glm::vec3(0.0f, -100.0f, 0.0f);
	glm::vec3 ObjScale = glm::vec3(400.0f, 400.0f, 1.0f);
	*/

	//* Suitable for Perspective projection
	float mObjRotationAngle = 0.0f;
	glm::vec3 mObjPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 mObjScale = glm::vec3(1.0f, 1.0f, 1.0f);

	glm::mat4 mRotationMat;
	glm::mat4 mScaleMat;
	glm::mat4 mTranslationMat;
	glm::mat4 mObjectModelMat;

public:
	cShapeObject(EShapeType _eShapeChoice);
	~cShapeObject();

	//GET accessors
	GLfloat* GetVertices() { return mpVertices; };
	GLuint* GetIndices() { return mpIndices; };

	glm::vec3 GetObjPosition() { return mObjPosition; };
	glm::vec3 GetObjScale() { return mObjScale; };
	glm::mat4 GetModelMat() { return mObjectModelMat; };

	EShapeType GetShapeType() { return mShapeType; };

	float GetObjRotation() { return mObjRotationAngle; };
	int GetIndexArraySize() { return miIndexArraySize; };

	//SET accessors
	void SetObjPosition(glm::vec3 _NewPosition) { mObjPosition = _NewPosition; };
	void SetObjScale(glm::vec3 _NewScale) { mObjScale = _NewScale; };
	void SetObjRotation(float _NewAngle) { mObjRotationAngle = _NewAngle; };
	void SetVertexAttributes(GLuint _VAO, GLuint _EBO, GLuint _VBO, GLenum _StaticOrDynamic);

	void ChangeVertexArray(GLfloat* _NewVertArray, int _NewArrSize);
	void UpdateObjectModelMat(bool _Translate = false, bool _Scale = false, bool _Rotate = true);
};

#endif // !__SHAPEOBJECT_H__

