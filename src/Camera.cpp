#include "DXUT.h"
#include "Camera.h"


Camera::Camera(void)
{
	_cameraType = CameraTypeSteady;
	_body = NULL;
	_isAnimating = false;
	_animCompletion = NULL;

	_isLeftHanded = true;
}


wchar_t *Camera::cameraTypeString()  
	{ 
		wchar_t *str;
		switch(_cameraType) {
		case CameraTypeSteady:
			str = L"Steady";
			break;
		case CameraTypeFollowBody:
			str = L"Follow Body";
			break;
		case CameraTypeWatchBody:
			str = L"Watch Body";
			break;
		case CameraTypeAnimateToPoint:
			str = L"Animating to Point";
			break;
		case CameraTypeAnimateThenFollowBody:
			str = L"Animate to follow body";
			break;
		default:
			str = L"Unknown";
		}
		return str;
	}


Camera::~Camera(void)
{
}

void Camera::setViewSize(float width, float height)
{
	_viewWidth  = width;
	_viewHeight = height;
}

void Camera::setNearZClipping(float nearZ)
{
	_nearClippingZ = nearZ;
}

void Camera::setFarZClipping(float farZ)
{
	_farClippingZ = farZ;
}

void Camera::setFOVAngle(float angle)
{
	_fovAngle = angle;
}

float Camera::viewWidth()
{
	return _viewWidth;
}

float Camera::viewHeight()
{
	return _viewHeight;
}



XMMATRIX Camera::viewMatrix()
{
	return XMMatrixLookAtLH(_position, _focusPosition, _upDirection);
}

XMMATRIX Camera::projectionMatrix()
{
	return XMMatrixPerspectiveFovLH(_fovAngle, (_viewWidth / _viewHeight), _nearClippingZ, _farClippingZ);
}

void Camera::setPosition(XMVECTOR position)
{
	_position = position;
}

void Camera::setPosition(float x, float y, float z)
{
	XMVECTOR position = XMVectorSet(x, y, z, 0.0);
	setPosition(position);
}

void Camera::setFocusPosition(XMVECTOR position)
{
	if (XMVector4Equal(position, _position)) {
		position += XMVector4Normalize(position);
	}
	_focusPosition = position;
}

void Camera::setFocusPosition(float x, float y, float z)
{
	XMVECTOR position = XMVectorSet(x, y, z, 0.0);
	setFocusPosition(position);
}

void Camera::setUpDirection(XMVECTOR direction)
{
	_upDirection = direction;
}

void Camera::setUpDirection(float x, float y, float z)
{
	XMVECTOR position = XMVectorSet(x, y, z, 0.0);
	setUpDirection(position);
}

void Camera::applyPosition(XMVECTOR position, bool applyAlsoToFocusPosition)
{
	_position += position;
	if (applyAlsoToFocusPosition) {
		_focusPosition += position;
	}
}

void Camera::applyPosition(float x, float y, float z, bool applyAlsoToFocusPosition)
{
	XMVECTOR position = XMVectorSet(x, y, z, 0.0);
	applyPosition(position, applyAlsoToFocusPosition);
}


void Camera::setSteadyCamera()
{
	_cameraType = CameraTypeSteady;
}


void Camera::animateToPosition(XMVECTOR position, XMVECTOR focusPosition, float duration, void (*animationCompletion)(bool))
{
	_animFrom = _position;
	_animTo   = position;
	_isAnimating = true;
	_animDuration = duration;
	_animProgress = 0.0;
	_animCompletion = animationCompletion;
	_cameraType = CameraTypeAnimateToPoint;

	setPosition(position);
	setFocusPosition(focusPosition);
}

//void Camera::planetAnimationCompletionInternal(bool finished)
//{
//	if (_animCompletion) {
//		_animCompletion(finished);
//	}
//}


void Camera::animateToPlanetThenFollow(Planet *planet, float duration, float simulationMultiplyFactor, float focusDuration, float distance, float rotation, void (*animationCompletion)(Planet*, bool))
{
	if (distance != -1.0) {
		_distance = distance;
	}
	//_planetAnimCompletion = &Camera::planetAnimationCompletionInternal;
	setFocusPosition(planet->positionInTime(duration * simulationMultiplyFactor, duration));
	_rotation = XMVectorSet(0.0, rotation, 0.0, 0.0);
	_planetAnimCompletion = animationCompletion;
	_animCompletion = NULL;
	_cameraType = CameraTypeAnimateThenFollowBody;
	_isAnimating = true;
	_animDuration = duration;
	_animFrom = _position;
	//_animTo   = position;		// WORK OUT THE POSITION!!
	_animProgress = 0.0;
	_body = planet;

	XMMATRIX translation = XMMatrixTranslation(_distance, 0.0, 0.0);
	XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYawFromVector(_rotation);
	XMMATRIX cameraMatrix = translation * rotationMatrix * planet->positionMatrixInTime(duration * simulationMultiplyFactor, duration);
	XMVECTOR newPosition = XMVector4Transform(XMVectorSet(0.0, 0.0, 0.0, 1.0), cameraMatrix);
	_animTo = newPosition;
}


void Camera::update(float dt, float wt)
{
	if (_isAnimating) {
		// move to next bit

		if (_cameraType == CameraTypeAnimateToPoint) {
			updateMoveToPosition(dt);
		} else if (_cameraType == CameraTypeAnimateThenFollowBody) {
			updateMoveToPlanetAndFollow(dt);
		}
		
	} else if (_cameraType == CameraTypeFollowBody) {
		// follow the body some how
		updateFollowCamera(0.0);
	} else if (_cameraType == CameraTypeWatchBody) {
		updateWatchCamera(0.0);
	}
}

void Camera::updateWatchCamera(float dt)
{
	setFocusPosition(_body->position());
}

void Camera::updateFollowCamera(float dt)
{
	setFocusPosition(_body->position());

		XMMATRIX translation = XMMatrixTranslation(_distance, 0.0, 0.0);
		XMMATRIX rotation    = XMMatrixRotationRollPitchYawFromVector(_rotation);
		XMMATRIX cameraMatrix = translation * rotation * _body->positionMatrix();
		XMVECTOR newPosition = XMVector4Transform(XMVectorSet(0.0, 0.0, 0.0, 1.0), cameraMatrix);
		setPosition(newPosition);
}

void Camera::updateMoveToPosition(float dt)
{
	_animProgress += dt;

		XMVECTOR diff = _animTo - _animFrom;
		XMVECTOR diffTime = diff * (_animProgress / _animDuration);

		setPosition(diffTime + _animFrom);


		
		if (_animProgress >= _animDuration) {
			_isAnimating = false;
			setPosition(_animTo);
			if (_animCompletion) {
				_animCompletion(true);
			}
		}
}

float easeInOutCurve(float t)
{
	return (t < 0.5) ? 4*t*t*t : (t-1)*(2*t-2)*(2*t-2)+1;
}


void Camera::updateMoveToPlanetAndFollow(float dt)
{
	_animProgress += dt;

		XMVECTOR diff = _animTo - _animFrom;
		XMVECTOR diffTime = diff * easeInOutCurve(_animProgress / _animDuration);

		setPosition(diffTime + _animFrom);


		
		if (_animProgress >= _animDuration) {
			_isAnimating = false;
			setPosition(_animTo);
			_cameraType = CameraTypeFollowBody;
			if (_planetAnimCompletion) {
				_planetAnimCompletion((Planet *)_body, true);
			}
		}
}



void Camera::cancelAnimation(bool returnToStart, bool moveToEnd)
{
	_isAnimating = false;
	if (returnToStart) {
		setPosition(_animFrom);
	} else if (moveToEnd) {
		setPosition(_animTo);
	}

	if (_animCompletion) {
		_animCompletion(false);
	}
}


void Camera::watchBody(Body *body)
{
	_cameraType = CameraTypeWatchBody;
	_body = body;
}

void Camera::followBody(Body *body, float distance)
{
	_cameraType = CameraTypeFollowBody;
	_body = body;
	_distance = distance;
	_rotation = XMVectorSet(0.0, 0.0, 0.0, 0.0);
}


void Camera::setFollowDistance(float distance)
{
	_distance = distance;
}

void Camera::appendFollowDistance(float distance)
{
	_distance += distance;
}

void Camera::setFollowRotation(XMVECTOR rotation)
{
	_rotation = rotation;
}

void Camera::setFollowRotation(float x, float y, float z)
{
	XMVECTOR rotation = XMVectorSet(x, y, z, 0.0);
	setFollowRotation(rotation);
}

void Camera::appendFollowRotation(XMVECTOR rotation)
{
	_rotation += rotation;
}

void Camera::appendFollowRotation(float x, float y, float z)
{
	XMVECTOR rotation = XMVectorSet(x, y, z, 0.0);
	appendFollowRotation(rotation);
}