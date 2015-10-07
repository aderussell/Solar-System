#include "Body.h"


Body::Body(void)
{
	_scale    = 1.0;
	_position = XMVectorSet(0.0, 0.0, 0.0, 1.0);
	_rotation = XMVectorSet(0.0, 0.0, 0.0, 1.0);
}


Body::~Body(void)
{
}


void Body::applyTranslation(XMVECTOR translation)
{
	_position += translation;
}

void Body::applyRotation(float x, float y, float z)
{
	XMVECTOR rot = XMVectorSet(x, y, z, 0);
	applyRotation(rot);
}

void Body::applyScale(float scale)
{
	_scale *= scale;
}

void Body::applyRotation(XMVECTOR rotation)
{
	_rotation += rotation;
}

void Body::setPosition(float x, float y, float z)
{
	XMVECTOR pos = XMVectorSet(x, y, z, 0);
	setPosition(pos);
}


void Body::setPosition(XMVECTOR position)
{
	_position = position;
}

void Body::setScale(float scale)
{
	_scale = scale;
}

void Body::setRotation(XMVECTOR rotation)
{
	_rotation = rotation;
}


XMMATRIX Body::positionMatrix()
{
	return XMMatrixTranslationFromVector(_position);
}

XMMATRIX Body::rotationMatrix()
{
	return XMMatrixRotationRollPitchYawFromVector(_rotation);
}

XMMATRIX Body::scaleMatrix()
{
	return XMMatrixScaling(_scale, _scale, _scale);
}

XMMATRIX Body::worldMatrix()
{
	return scaleMatrix() * rotationMatrix() *  positionMatrix();
}