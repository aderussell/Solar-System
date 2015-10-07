#pragma once
#include "Planet.h"
#include "TextureManager.h"
#include "Mesh.h"

class Mars : public SingleMeshPlanet
{
public:
	Mars(Planet *owner);
	~Mars(void);




	static MeshMaterial *createMarsMaterial(TextureManager *textureManager);
	//static MeshMaterial *createEarthNightMaterial(TextureManager *textureManager);

	//void update(float dt, float wt);
	//void render(ID3D11Device *renderingDevice, ID3D11DeviceContext *immediateContext);

	void startBodyAudio();

	//Mesh *mesh;
	//MeshMaterial *material;

};

