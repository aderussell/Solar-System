#include "Venus.h"
#include "PlanetConstants.h"

#define VENUS_TEXTURE_FILE          L"Media\\Venus\\Venus_Texture.jpg"
#define VENUS_AUDIO_FILENAME        L"Media\\Venus\\Venus_Audio.wav"

Venus::Venus(Planet *owner) : SingleMeshPlanet(owner)
{
	_orbitDuration = VENUS_ORBIT_DURATION;
	_spinDuration  = VENUS_SPIN_DURATION;
	_orbitRadius   = VENUS_DISTANCE_FROM_SUN * SCALE_SIZES_AMMOUNT;
	setScale(VENUS_RADIUS * SCALE_SIZES_AMMOUNT);
	_name = L"Venus";
}


Venus::~Venus(void)
{
	if (material) delete material; material = NULL;
}

void Venus::startBodyAudio()
{
	PlaySound(VENUS_AUDIO_FILENAME, NULL, SND_ASYNC);
}

MeshMaterial *Venus::createVenusMaterial(TextureManager *textureManager)
{
		MeshMaterial *earthMaterial = new MeshMaterial();

		ID3D11ShaderResourceView *earthTexture;
	    HRESULT hr = textureManager->createTexture(VENUS_TEXTURE_FILE, &earthTexture);
		if (FAILED(hr)) exit(-1);
		earthMaterial->texture = earthTexture;
	
	return earthMaterial;
}
