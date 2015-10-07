#include "Jupiter.h"
#include "PlanetConstants.h"

#define JUPITER_TEXTURE_FILE          L"Media\\Jupiter\\Jupiter_Texture.jpg"
#define JUPITER_AUDIO_FILENAME        L"Media\\Jupiter\\Jupiter_Audio.wav"

Jupiter::Jupiter(Planet *owner) : SingleMeshPlanet(owner)
{
	_orbitDuration = JUPITER_ORBIT_DURATION;
	_spinDuration  = JUPITER_SPIN_DURATION;
	_orbitRadius   = JUPITER_DISTANCE_FROM_SUN * SCALE_SIZES_AMMOUNT;
	setScale(JUPITER_RADIUS * SCALE_SIZES_AMMOUNT);
	_name = L"Jupiter";
}


Jupiter::~Jupiter(void)
{
}

void Jupiter::startBodyAudio()
{
	PlaySound(JUPITER_AUDIO_FILENAME, NULL, SND_ASYNC);
}

MeshMaterial *Jupiter::createJupiterMaterial(TextureManager *textureManager)
{
	MeshMaterial *earthMaterial = new MeshMaterial();

	ID3D11ShaderResourceView *earthTexture;
	HRESULT hr = textureManager->createTexture(JUPITER_TEXTURE_FILE, &earthTexture);
	if (FAILED(hr)) exit(-1);
	earthMaterial->texture = earthTexture;
	
	return earthMaterial;
}