#pragma once
#include "Planet.h"
#include "TextureManager.h"
#include "Mesh.h"

class Venus : public SingleMeshPlanet
{
public:
	Venus(Planet *owner);
	~Venus(void);


	static MeshMaterial *createVenusMaterial(TextureManager *textureManager);
	//static MeshMaterial *createEarthNightMaterial(TextureManager *textureManager);

	//void update(float dt, float wt);
	//void render(ID3D11Device *renderingDevice, ID3D11DeviceContext *immediateContext);

	void startBodyAudio();

	//Mesh *mesh;
	//MeshMaterial *material;
};

