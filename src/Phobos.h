#pragma once
#include "Planet.h"
#include "TextureManager.h"
#include "Mesh.h"

class Phobos : public SingleMeshPlanet
{
public:
	Phobos(Planet *owner);
	~Phobos(void);

	static MeshMaterial *createPhobosMaterial(TextureManager *textureManager);

	//void update(float dt, float wt);
	void render(ID3D11Device *renderingDevice, ID3D11DeviceContext *immediateContext);


};

