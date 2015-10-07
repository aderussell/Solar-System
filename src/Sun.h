#pragma once
#include "Planet.h"
#include "TextureManager.h"




class Sun : public SingleMeshPlanet
{
public:
	Sun(Planet *owner = NULL);
	~Sun(void);

	static MeshMaterial *createSunMaterial(TextureManager *textureManager);

	void update(float dt, float wt);

	void startBodyAudio();
};

