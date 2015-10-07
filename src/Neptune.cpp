#include "Neptune.h"
#include "PlanetConstants.h"

#define NEPTUNE_TEXTURE_FILE          L"Media\\Neptune\\Neptune_Texture.jpg"
#define NEPTUNE_AUDIO_FILENAME        L"Media\\Neptune\\Neptune_Audio.wav"

Neptune::Neptune(Planet *owner) : SingleMeshPlanet(owner)
{
	_orbitDuration = NEPTUNE_ORBIT_DURATION;
	_spinDuration  = NEPTUNE_SPIN_DURATION;
	_orbitRadius   = NEPTUNE_DISTANCE_FROM_SUN * SCALE_SIZES_AMMOUNT;
	setScale(NEPTUNE_RADIUS * SCALE_SIZES_AMMOUNT);
	_name = L"Neptune";
}


Neptune::~Neptune(void)
{
}

void Neptune::startBodyAudio()
{
	PlaySound(NEPTUNE_AUDIO_FILENAME, NULL, SND_ASYNC);
}

MeshMaterial *Neptune::createNeptuneMaterial(TextureManager *textureManager)
{
		MeshMaterial *earthMaterial = new MeshMaterial();

		ID3D11ShaderResourceView *earthTexture;
	    HRESULT hr = textureManager->createTexture(NEPTUNE_TEXTURE_FILE, &earthTexture);
		if (FAILED(hr)) exit(-1);
		earthMaterial->texture = earthTexture;
	
	return earthMaterial;
}
