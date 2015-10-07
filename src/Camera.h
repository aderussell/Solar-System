#pragma once

#include <xnamath.h>
#include "Body.h"
#include "Planet.h"

//void (*animationCompletion)(bool);


class Camera
{
public:
	Camera(void);
	~Camera(void);

	enum CameraType
	{
		CameraTypeSteady,
		CameraTypeFollowBody,
		CameraTypeWatchBody,
		CameraTypeAnimateToPoint,
		CameraTypeAnimateThenFollowBody,

	};
	CameraType cameraType()			{ return _cameraType; }
	wchar_t *cameraTypeString();  

	XMVECTOR position() { return _position; }

	void setPosition(XMVECTOR position);
	void setPosition(float x, float y, float z);

	// if this is the same as position, a normalized position is added to make it slightly further waya
	void setFocusPosition(XMVECTOR position);
	void setFocusPosition(float x, float y, float z);

	void setUpDirection(XMVECTOR direction);
	void setUpDirection(float x, float y, float z);

	void applyPosition(XMVECTOR position, bool applyAlsoToFocusPosition = true);
	void applyPosition(float x, float y, float z, bool applyAlsoToFocusPosition = true);

	void moveRelativeToFocus(float x, float y, float z)
	{
		XMVECTOR looky = _focusPosition - _position;
		looky = XMVector4Normalize(looky);
		XMVECTOR forward = looky * z;
		XMVECTOR sideways = XMVectorSet(-XMVectorGetZ(looky), XMVectorGetY(looky), XMVectorGetX(looky), 1.0) * x;
		_position += forward + sideways;
		_focusPosition += forward + sideways;
	}

	void rotateAroundPosition(float roll, float pitch, float yaw)
	{
		XMVECTOR looky = _focusPosition - _position;
		XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(roll, pitch, yaw);
		XMVECTOR adjust = XMVector4Transform(looky, rotationMatrix);
		_focusPosition = _position + adjust;
	}




	// projection matrix stuff
	void setViewSize(float width, float height);
	void setNearZClipping(float nearZ);
	void setFarZClipping(float farZ);
	void setFOVAngle(float angle);

	float viewWidth();
	float viewHeight();


	// view matrix
	XMMATRIX viewMatrix();

	XMMATRIX projectionMatrix();

	//void animateToPosition(XMVECTOR position, XMVECTOR focusPosition, float duration);
	void animateToPosition(XMVECTOR position, XMVECTOR focusPosition, float duration, void (*animationCompletion)(bool) = NULL);

	void animateToPlanetThenFollow(Planet *planet, float duration, float simulationMultiplyFactor = 0.0, float focusDuration = 0.0, float distance = -1.0, float rotation = 0.0, void (*animationCompletion)(Planet*, bool) = NULL);

	void cancelAnimation(bool returnToStart = false, bool moveToEnd = false);
	bool isAnimating() { return _isAnimating; }


	void update(float dt, float wt);

	void setSteadyCamera();

	void watchBody(Body *body);
	void followBody(Body *body, float distance);

	void setFollowDistance(float distance);
	void appendFollowDistance(float distance);
	void setFollowRotation(XMVECTOR rotation);
	void setFollowRotation(float x, float y, float z);
	void appendFollowRotation(XMVECTOR rotation);
	void appendFollowRotation(float x, float y, float z);


protected:

	// view matrix parameters
	XMVECTOR _position;
	XMVECTOR _focusPosition;
	XMVECTOR _upDirection;

	// projection matrix paramaters;
	float _viewWidth;
	float _viewHeight;
	float _nearClippingZ;
	float _farClippingZ;
	float _fovAngle;

	void planetAnimationCompletionInternal(bool finished);
	void updateWatchCamera(float dt);
	void updateFollowCamera(float dt);
	void updateMoveToPosition(float dt);
	void updateMoveToPlanetAndFollow(float dt);

	CameraType _cameraType;

	bool _isLeftHanded;

	

	// body to follow if camera is so
	Body *_body;
	float _distance;
	XMVECTOR _rotation;

	// animation stuff
	bool _isAnimating;
	float _animDuration;
	float _animProgress;
	XMVECTOR _animFrom;
	XMVECTOR _animTo;
	XMVECTOR _focusFrom;
	XMVECTOR _focusTo;
	void (*_animCompletion)(bool);
	void (*_planetAnimCompletion)(Planet*, bool);
};

