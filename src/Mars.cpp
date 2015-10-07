#include "Mars.h"
#include "PlanetConstants.h"

#define MARS_TEXTURE_FILE          L"Media\\Mars\\Mars_Texture.jpg"
#define MARS_AUDIO_FILENAME        L"Media\\Mars\\Mars_Audio.wav"

Mars::Mars(Planet *owner) : SingleMeshPlanet(owner)
{
	_orbitDuration = MARS_ORBIT_DURATION;
	_spinDuration  = MARS_SPIN_DURATION;
	_orbitRadius   = (MARS_DISTANCE_FROM_SUN * SCALE_SIZES_AMMOUNT);
	setScale(MARS_RADIUS * SCALE_SIZES_AMMOUNT);
	_name = L"Mars";
}


Mars::~Mars(void)
{
}

void Mars::startBodyAudio()
{
	PlaySound(MARS_AUDIO_FILENAME, NULL, SND_ASYNC);
}

MeshMaterial *Mars::createMarsMaterial(TextureManager *textureManager)
{
	MeshMaterial *earthMaterial = new MeshMaterial();

	ID3D11ShaderResourceView *earthTexture;
	HRESULT hr = textureManager->createTexture(MARS_TEXTURE_FILE, &earthTexture);
	if (FAILED(hr)) exit(-1);
	earthMaterial->texture = earthTexture;
	
	return earthMaterial;
}
