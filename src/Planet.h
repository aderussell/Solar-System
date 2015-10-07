#pragma once
#include "Body.h"
#include "Mesh.h"
#include "Material.h"
#include <string>

class Planet : public Body
{
public:
	Planet(Planet *owner = NULL);
	~Planet(void);

	//virtual XMMATRIX worldMatrix();

	virtual void update(float dt, float wt);

	virtual void startBodyAudio() {};
	virtual void stopBodyAudio() { PlaySound(NULL, NULL, SND_ASYNC); }

	virtual XMVECTOR positionInTime(float dt, float wt);
	virtual XMVECTOR rotationInTime(float dt, float wt);
	virtual XMMATRIX positionMatrixInTime(float dt, float wt);
	virtual XMMATRIX rotationMatrixInTime(float dt, float wt);

	void setOrbitRotation(XMVECTOR rotation);
	void setOrbitRotation(float x, float y, float z);

	void applyOrbitRotation(XMVECTOR rotation);
	void applyOrbitRotation(float x, float y, float z);

	XMMATRIX orbitRotationMatrix();

	XMMATRIX positionMatrix();

	//XMMATRIX rotationMatrix();

	virtual XMVECTOR position();
	//virtual XMVECTOR rotation();


	wchar_t *name() { return _name; }


protected:
	Planet *_owner;

	wchar_t *_name;

	float _orbitRadius;		// the orbit distance from centers of planet & owner
	float _orbitDuration;	// how it takes a planet to orbit its owner
	float _spinDuration;	// how long it takes aplanet to spin on its access

	XMVECTOR _orbitRotation;	// this thing
};



class SingleMeshPlanet : public Planet
{
public:
	SingleMeshPlanet(Planet *owner = NULL);
	~SingleMeshPlanet(void);

	virtual void render(ID3D11Device *renderingDevice, ID3D11DeviceContext *immediateContext);

	Mesh         *mesh;
	MeshMaterial *material;
};