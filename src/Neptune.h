#pragma once
#include "Planet.h"
#include "TextureManager.h"
#include "Mesh.h"

class Neptune : public SingleMeshPlanet
{
public:
	Neptune(Planet *owner);
	~Neptune(void);



	static MeshMaterial *createNeptuneMaterial(TextureManager *textureManager);
	//static MeshMaterial *createEarthNightMaterial(TextureManager *textureManager);

	//void update(float dt, float wt);
	//void render(ID3D11Device *renderingDevice, ID3D11DeviceContext *immediateContext);

	void startBodyAudio();
};

