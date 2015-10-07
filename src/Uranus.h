#pragma once
#include "Planet.h"
#include "TextureManager.h"
#include "Mesh.h"

class Uranus : public SingleMeshPlanet
{
public:
	Uranus(Planet *owner);
	~Uranus(void);


	static MeshMaterial *createUranusMaterial(TextureManager *textureManager);
	//static MeshMaterial *createEarthNightMaterial(TextureManager *textureManager);


	void startBodyAudio();

};

