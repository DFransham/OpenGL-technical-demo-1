#pragma once
#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <iostream>

//Object Matrices and Components - examples
/* Suitable for Orthographic projection
glm::vec3 ObjPosition = glm::vec3(0.0f, -100.0f, 0.0f);
float ObjRotationAngle = 0.0f;
glm::vec3 ObjScale = glm::vec3(400.0f, 400.0f, 1.0f);
*/
/* Suitable for Perspective projection
glm::vec3 ObjPosition = glm::vec3(-0.5f, -0.5f, 0.0f);
float ObjRotationAngle = 180.0f;
glm::vec3 ObjScale = glm::vec3(0.4f, 0.4f, 1.0f);
*/

enum class ECameraProjectionMode
{
	ORTHOGRAPHIC_TOP,
	ORTHOGRAPHIC_CENTRE,
	PERSPECTIVE_LOOK_DIRECTION,
	PERSPECTIVE_TARGET_POSITION,
};

class cCamera
{
protected:
	glm::vec3 mCameraPos;
	glm::vec3 mCameraLookDir = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 mCameraTargetPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 mCameraUpDir = glm::vec3(0.0f, 1.0f, 0.0f);
	
	glm::mat4 mViewMat;
	glm::mat4 mProjectionMat;
	
	ECameraProjectionMode meProjectionMode;

	//Camera movement variables
	bool mbMoveRight = true;
	float mfTargetFPS = 25.0f;
	float mfPixelsPerFrame = 1.0f;
	float mfLeftEdge = -50.0f;
	float mfRightEdge = 50.0f;
	float mfAccumulatedDelta = 0.0f;

	void PanCamera();
	void UpdateViewMat();

public:
	cCamera(GLfloat _fFOV, GLfloat _fWidth, GLfloat _fHeight, glm::vec3 _vCameraPos, ECameraProjectionMode ProjectionMode);
	~cCamera();

	void Update(float _fDeltaTime);

	//GET accessors
	glm::vec3 GetCameraPos() { return mCameraPos; };
	glm::vec3 GetCameraLookDir() { return mCameraLookDir; };
	glm::vec3 GetCameraTargetPos() { return mCameraTargetPos; };
	glm::vec3 GetCameraUpDir() { return mCameraUpDir; };
	glm::mat4 GetViewMat() { return mViewMat; };
	glm::mat4 GetProjectionMat() { return mProjectionMat; };
	
	ECameraProjectionMode GetProjectionMode() { return meProjectionMode; };

	//SET accessors
	void SetCameraPos(glm::vec3 _NewCameraPos) { mCameraPos = _NewCameraPos; };
	void SetCameraLookDir(glm::vec3 _NewLookDir) { mCameraLookDir = _NewLookDir; };
	void SetCameraTargetPos(glm::vec3 _NewTargetPos) { mCameraTargetPos = _NewTargetPos; };
	void SetCameraUpDir(glm::vec3 _NewUpDir) { mCameraUpDir = _NewUpDir; };
	void SetViewMat(glm::mat4 _NewViewMat) { mViewMat = _NewViewMat; };
	void SetProjectionMat(glm::mat4 _NewProjectionMat) { mProjectionMat = _NewProjectionMat; };
	void ChangeProjectionMode(ECameraProjectionMode _NewProjectionMode) { meProjectionMode = _NewProjectionMode; };

};

#endif // !__CAMERA_H__