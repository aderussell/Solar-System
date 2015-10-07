#include "Planet.h"
#include "Sphere.h"


XMMATRIX _positionMatrix(float orbitRadius, XMVECTOR orbitRotation, XMMATRIX &ownerPositionMatrix)
{
	XMMATRIX matA = XMMatrixTranslation(orbitRadius, 0.0, 0.0);
	XMMATRIX matB = XMMatrixRotationRollPitchYawFromVector(orbitRotation);
	XMMATRIX matC = XMMatrixRotationRollPitchYaw(0.0, 0.0, 0.0);
	XMMATRIX matD = ownerPositionMatrix;

	return matA * matB * matD;
}


Planet::Planet(Planet *owner)
{
	_owner = owner;
	_orbitRotation = XMVectorSet(0.0, 0.0, 0.0, 0.0);
	_name = L"Unnamed Planet";
}

Planet::~Planet(void)
{
	if (_owner) {
		_owner = NULL;
	}
}

void Planet::setOrbitRotation(XMVECTOR rotation)
{
	_orbitRotation = rotation;
}

void Planet::setOrbitRotation(float x, float y, float z)
{
	XMVECTOR rot = XMVectorSet(x, y, z, 0.0);
	setOrbitRotation(rot);
}

void Planet::applyOrbitRotation(XMVECTOR rotation)
{
	_orbitRotation += rotation;
}

void Planet::applyOrbitRotation(float x, float y, float z)
{
	XMVECTOR rot = XMVectorSet(x, y, z, 0.0);
	applyOrbitRotation(rot);
}

void Planet::update(float dt, float wt)
{
	float t = dt / _spinDuration;
	applyRotation(0.0, t, 0.0);

	applyOrbitRotation(0.0, dt / _orbitDuration, 0.0);
}

XMMATRIX Planet::orbitRotationMatrix()
{
	return XMMatrixRotationRollPitchYawFromVector(_orbitRotation);
}

XMMATRIX Planet::positionMatrix()
{
	if (!_owner) {
		return Body::positionMatrix();
	}

	//return _positionMatrix(_orbitRadius, _orbitRotation, _owner->positionMatrix());
	return positionMatrixInTime(0.0, 0.0);
}


XMVECTOR Planet::position()
{
	if (!_owner) {
		return Body::position();
	}

	return positionInTime(0.0, 0.0);
}


XMMATRIX Planet::positionMatrixInTime(float dt, float wt)
{
	if (!_owner) {
		return Body::positionMatrix();
	}

	XMMATRIX ownerPositionMatrix = _owner->positionMatrixInTime(dt, wt);

	XMVECTOR orbitRotation = _orbitRotation + XMVectorSet(0.0, dt / _orbitDuration, 0.0, 0.0);
	XMMATRIX timePositionMatrix = _positionMatrix(_orbitRadius, orbitRotation, ownerPositionMatrix);
	return timePositionMatrix;
}

XMVECTOR Planet::positionInTime(float dt, float wt)
{
	if (!_owner) {
		return Body::position();
	}

	XMMATRIX timePositionMatrix = positionMatrixInTime(dt, wt);
	XMVECTOR timePos =  XMVector4Transform(_position, timePositionMatrix);
	return timePos;
}


//-------------------------
	
XMVECTOR Planet::rotationInTime(float dt, float wt)
{
	if (!_owner) {
		return Body::rotation();
	}

	XMMATRIX timePositionMatrix = _positionMatrix(_orbitRadius, _orbitRotation, _owner->positionMatrix());
	return XMVector4Transform(_position, timePositionMatrix);
}

XMMATRIX Planet::rotationMatrixInTime(float dt, float wt)
{
	float t = dt / _spinDuration;
	XMVECTOR rotation = _rotation + XMVectorSet(0.0, t, 0.0, 0.0);
	return XMMatrixRotationRollPitchYawFromVector(rotation);
}



//----------------------------------------------------------------------------------------------------------------------------------------//


SingleMeshPlanet::SingleMeshPlanet(Planet *owner) : Planet(owner)
{
	mesh = Sphere::sphereMesh();
	material = NULL;
}

SingleMeshPlanet::~SingleMeshPlanet(void)
{
	if (material) delete material; material = NULL;
}

void SingleMeshPlanet::render(ID3D11Device *renderingDevice, ID3D11DeviceContext *immediateContext)
{
	// Set vertex buffer
	UINT stride = this->mesh->buffers->vertexTypeSize();
	UINT offset = 0;
	ID3D11Buffer *vertexBuffer = this->mesh->buffers->vertexBuffer();
	immediateContext->IASetVertexBuffers( 0, 1, &(vertexBuffer), &stride, &offset );

	// Set index buffer
	ID3D11Buffer *indexBuffer = this->mesh->buffers->indexBuffer();
	immediateContext->IASetIndexBuffer( indexBuffer, DXGI_FORMAT_R16_UINT, 0 );

	

	//immediateContext->VSSetShader(material->vertexShader, NULL, 0);

	// setup the pixel shader
	immediateContext->PSSetShader(material->pixelShader, NULL, 0);
	immediateContext->PSSetShaderResources(0, 1, &(material->texture));
	//immediateContext->PSSetShaderResources(1, 1, &material->bumpMapTexture);

	// draw it out
	immediateContext->DrawIndexed(this->mesh->indices.size(), 0, 0 );
}