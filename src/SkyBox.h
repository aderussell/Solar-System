#pragma once
#include "Body.h"
#include "TextureManager.h"
#include "Mesh.h"


class SkyBox : public Body
{
public:
	SkyBox(void);
	~SkyBox(void);

	static MeshMaterial *createSkyBoxMaterial(TextureManager *textureManager);

	void render(ID3D11Device *renderingDevice, ID3D11DeviceContext *immediateContext);

	void update(float dt, float wt);

	Mesh *mesh;
	MeshMaterial *material;
};

