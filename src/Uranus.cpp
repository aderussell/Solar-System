#include "Uranus.h"
#include "PlanetConstants.h"

#define URANUS_TEXTURE_FILE          L"Media\\Uranus\\Uranus_Texture.jpg"
#define URANUS_AUDIO_FILENAME        L"Media\\Uranus\\Uranus_Audio.wav"

Uranus::Uranus(Planet *owner) : SingleMeshPlanet(owner)
{
	_orbitDuration = URANUS_ORBIT_DURATION;
	_spinDuration  = URANUS_SPIN_DURATION;
	_orbitRadius   = URANUS_DISTANCE_FROM_SUN * SCALE_SIZES_AMMOUNT;
	setScale(URANUS_RADIUS * SCALE_SIZES_AMMOUNT);
	_name = L"Uranus";
}


Uranus::~Uranus(void)
{
}

void Uranus::startBodyAudio()
{
	PlaySound(URANUS_AUDIO_FILENAME, NULL, SND_ASYNC);
}

MeshMaterial *Uranus::createUranusMaterial(TextureManager *textureManager)
{
	MeshMaterial *earthMaterial = new MeshMaterial();

	ID3D11ShaderResourceView *earthTexture;
	HRESULT hr = textureManager->createTexture(URANUS_TEXTURE_FILE, &earthTexture);
	if (FAILED(hr)) exit(-1);
	earthMaterial->texture = earthTexture;

	return earthMaterial;
}

