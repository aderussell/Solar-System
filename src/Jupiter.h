#pragma once
#include "Planet.h"
#include "TextureManager.h"
#include "Mesh.h"

class Jupiter : public SingleMeshPlanet
{
public:
	Jupiter(Planet *owner);
	~Jupiter(void);


	//void setDay() { _isDay = true; };
	//void setNight() {_isDay = false; };
	//bool isDay() { return _isDay; };

	static MeshMaterial *createJupiterMaterial(TextureManager *textureManager);
	//static MeshMaterial *createEarthNightMaterial(TextureManager *textureManager);

	//void update(float dt, float wt);
	//void render(ID3D11Device *renderingDevice, ID3D11DeviceContext *immediateContext);

	void startBodyAudio();

	//Mesh *mesh;
	//MeshMaterial *material;

//private:
	//bool _isDay;
};

