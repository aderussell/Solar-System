#pragma once
#include "Planet.h"
#include "TextureManager.h"
#include "Mesh.h"

class Moon : public SingleMeshPlanet
{
public:
	Moon(Planet *owner);
	~Moon(void);

	static MeshMaterial *createMoonMaterial(TextureManager *textureManager);

	void render(ID3D11Device *renderingDevice, ID3D11DeviceContext *immediateContext);

	void startBodyAudio();
};

