#include "Moon.h"
#include "PlanetConstants.h"



#define MOON_TEXTURE_FILE		   L"Media\\Moon\\Moon_Texture.jpg"
#define MOON_BUMP_MAP_TEXTURE_FILE L"Media\\Moon\\Moon_Bumpmap.jpg"
#define MOON_AUDIO_FILENAME        L"Media\\Moon\\Moon_Audio.wav"

Moon::Moon(Planet *owner) : SingleMeshPlanet(owner)
{
	setScale(MOON_RADIUS * SCALE_SIZES_AMMOUNT);
	_orbitDuration = MOON_ORBIT_DURATION;
	_spinDuration  = MOON_SPIN_DURATION;
	_orbitRadius   = MOON_DISTANCE_FROM_EARTH * SCALE_SIZES_AMMOUNT;
	_name = L"Moon (Earth)";
}


Moon::~Moon(void)
{
}


MeshMaterial *Moon::createMoonMaterial(TextureManager *textureManager)
{
	MeshMaterial *moonMaterial = new MeshMaterial();

	ID3D11ShaderResourceView *moonTexture;
	HRESULT hr = textureManager->createTexture(MOON_TEXTURE_FILE, &moonTexture);
	if (FAILED(hr)) exit(-1);
	moonMaterial->texture = moonTexture;

	ID3D11ShaderResourceView *earthTextureBumpMap;
	hr = textureManager->createTexture(MOON_BUMP_MAP_TEXTURE_FILE, &earthTextureBumpMap);
	if (FAILED(hr)) return NULL;
	moonMaterial->bumpMapTexture = earthTextureBumpMap;

	return moonMaterial;
}

void Moon::render(ID3D11Device *renderingDevice, ID3D11DeviceContext *immediateContext)
{
	// Set vertex buffer
	UINT stride = (UINT)this->mesh->buffers->vertexTypeSize();
	UINT offset = 0;
	immediateContext->IASetVertexBuffers( 0, 1, &(this->mesh->vertexBuffer), &stride, &offset );

	// Set index buffer
	immediateContext->IASetIndexBuffer( this->mesh->indexBuffer, DXGI_FORMAT_R16_UINT, 0 );

	

	//immediateContext->VSSetShader(material->vertexShader, NULL, 0);

	// setup the pixel shader
	immediateContext->PSSetShader(material->pixelShader, NULL, 0);
	immediateContext->PSSetShaderResources(0, 1, &(material->texture));
	immediateContext->PSSetShaderResources(1, 1, &(material->bumpMapTexture));

	// draw it out
	immediateContext->DrawIndexed(this->mesh->indices.size(), 0, 0 );
}

void Moon::startBodyAudio()
{
	PlaySound(MOON_AUDIO_FILENAME, NULL, SND_ASYNC);
}