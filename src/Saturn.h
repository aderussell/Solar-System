#pragma once
#include "Planet.h"
#include "TextureManager.h"
#include "Mesh.h"

class Saturn : public SingleMeshPlanet
{
public:
	Saturn(Planet *owner);
	~Saturn(void);


	static MeshMaterial *createSaturnMaterial(TextureManager *textureManager);

	void render(ID3D11Device *renderingDevice, ID3D11DeviceContext *immediateContext);

	void startBodyAudio();

	Mesh *ringMesh;
	MeshMaterial *ringMaterial;
};

