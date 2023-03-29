#include "Camera.h"

//Constructor - creates the camera with the given variables.
cCamera::cCamera(GLfloat _fFOV, GLfloat _fWidth, GLfloat _fHeight, glm::vec3 _vCameraPos, ECameraProjectionMode ProjectionMode)
{
	mCameraPos = _vCameraPos;
	meProjectionMode = ProjectionMode;
	UpdateViewMat();
	switch (ProjectionMode)
	{
	case ECameraProjectionMode::ORTHOGRAPHIC_TOP:
		mProjectionMat = glm::ortho(0.0f, _fWidth, _fHeight, 0.0f, 0.1f, 100.0f);
		break;
	case ECameraProjectionMode::ORTHOGRAPHIC_CENTRE:
		mProjectionMat = glm::ortho(_fWidth * -0.5f, _fWidth * 0.5f, _fHeight * -0.5f, _fHeight * 0.5f, 0.1f, 100.0f);
		break;
	case ECameraProjectionMode::PERSPECTIVE_TARGET_POSITION:
		//fall through - perspective projection mat is the same in both cases
	case ECameraProjectionMode::PERSPECTIVE_LOOK_DIRECTION:
		mProjectionMat = glm::perspective(glm::radians(_fFOV), _fWidth / _fHeight, 0.1f, 100.0f);
		break;
	default:
		std::cout << "I'm not sure how we got here.  SetUpCameraProjection() switch case fail.";
		break;
	}
}

//Destructor - nothing needed as no memory allocated
cCamera::~cCamera() {}

//adds delta time to accumulated time, subtracts accumulated time after FPS threshold, calls functions to calculate panning and update view matrix
void cCamera::Update(float _fDeltaTime)
{
	mfAccumulatedDelta += _fDeltaTime;
	if (mfAccumulatedDelta >= (1 / mfTargetFPS))
	{
		mfAccumulatedDelta -= (1 / mfTargetFPS);
		PanCamera();
		UpdateViewMat();
	}
}

//Updates the view matrix now that camera position has changed
void cCamera::UpdateViewMat()
{
	switch (meProjectionMode)
	{
	case ECameraProjectionMode::ORTHOGRAPHIC_TOP:			//fall through - first three modes use same view matrix
	case ECameraProjectionMode::ORTHOGRAPHIC_CENTRE:
	case ECameraProjectionMode::PERSPECTIVE_LOOK_DIRECTION:
		mViewMat = glm::lookAt(mCameraPos, mCameraPos + mCameraLookDir, mCameraUpDir);
		break;
	case ECameraProjectionMode::PERSPECTIVE_TARGET_POSITION:
		mViewMat = glm::lookAt(mCameraPos, mCameraTargetPos, mCameraUpDir);
		break;
	default:
		std::cout << "I'm not sure how we got here. cCamera::UpdateViewMat switch case fail.";
		break;
	}
}

//changes camera position by PixelsPerFrame variable, adjusts boolean for left and right if camera is at edge of target zone
void cCamera::PanCamera()
{
	if (mbMoveRight)
	{
		SetCameraPos(glm::vec3(mCameraPos.x + mfPixelsPerFrame, mCameraPos.y, mCameraPos.z));
		if (mCameraPos.x >= mfRightEdge)
		{
			mbMoveRight = !mbMoveRight;
		}
	}
	else
	{
		SetCameraPos(glm::vec3(mCameraPos.x - mfPixelsPerFrame, mCameraPos.y, mCameraPos.z));
		if (mCameraPos.x <= mfLeftEdge)
		{
			mbMoveRight = !mbMoveRight;
		}
	}
}