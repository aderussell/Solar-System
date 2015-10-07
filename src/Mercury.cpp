#include "Mercury.h"
#include "PlanetConstants.h"



#define EARTH_TEXTURE_FILE     L"Media\\Mercury\\Mercury_Texture.jpg"
#define EARTH_AUDIO_FILENAME   L"Media\\Mercury\\Mercury_Audio.wav"

Mercury::Mercury(Planet *owner) : SingleMeshPlanet(owner)
{
	//material = NULL;
	_orbitDuration = MERCURY_ORBIT_DURATION;
	_spinDuration  = MERCURY_SPIN_DURATION;
	_orbitRadius   = MERCURY_DISTANCE_FROM_SUN * SCALE_SIZES_AMMOUNT;
	setScale(MERCURY_RADIUS * SCALE_SIZES_AMMOUNT);
	_name = L"Mercury";
}


Mercury::~Mercury(void)
{
}

void Mercury::startBodyAudio()
{
	PlaySound(EARTH_AUDIO_FILENAME, NULL, SND_ASYNC);
}

MeshMaterial *Mercury::createMercuryMaterial(TextureManager *textureManager)
{
	MeshMaterial *earthMaterial = new MeshMaterial();

	ID3D11ShaderResourceView *earthTexture;
	HRESULT hr = textureManager->createTexture(EARTH_TEXTURE_FILE, &earthTexture);
	if (FAILED(hr)) exit(-1);
	earthMaterial->texture = earthTexture;
	
	return earthMaterial;
}
