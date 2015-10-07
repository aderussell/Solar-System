#include "Sun.h"
#include "PlanetConstants.h"

//#define SUN_REAL_SCALE 1
//#define SUN_REAL_SPIN  27.0		// this isn't true but whatever

#define SUN_TEXTURE_FILE	L"Media\\Sun\\Sun_Texture.png"
#define SUN_AUDIO_FILENAME  L"Media\\Sun\\Sun_Audio.wav"

Sun::Sun(Planet *owner) : SingleMeshPlanet(owner)
{
	setScale(SUN_RADIUS * SCALE_SIZES_AMMOUNT);
	setPosition(0.0, 0.0, 0.0);
	_spinDuration  = SUN_SPIN_DURATION;
}


Sun::~Sun(void)
{
}

void Sun::startBodyAudio()
{
	PlaySound(SUN_AUDIO_FILENAME, NULL, SND_ASYNC);
}


MeshMaterial *Sun::createSunMaterial(TextureManager *textureManager)
{
	MeshMaterial *moonMaterial = new MeshMaterial();

	ID3D11ShaderResourceView *moonTexture;
	HRESULT hr = textureManager->createTexture(SUN_TEXTURE_FILE, &moonTexture);
	if (FAILED(hr)) exit(-1);
	moonMaterial->texture = moonTexture;

	return moonMaterial;
}

void Sun::update(float dt, float wt)
{
	float t = dt / _spinDuration;
	applyRotation(0.0, t, 0.0);
}
