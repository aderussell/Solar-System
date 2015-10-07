#include "Earth.h"
#include "Sphere.h"
#include "PlanetConstants.h"

#define EARTH_TEXTURE_FILE          L"Media\\Earth\\Earth_Texture.jpg"
#define EARTH_TEXTURE_FILE_NIGHT    L"Media\\Earth\\Earth_Texture_Night.jpg"
#define EARTH_BUMP_MAP_TEXTURE_FILE L"Media\\Earth\\Earth_Bump.jpg"
#define EARTH_AUDIO_FILENAME        L"Media\\Earth\\Earth_Audio.wav"


//#define EARTH_REAL_ORBIT_DISTANCE (23481.0/109.231)
//#define EARTH_REAL_ORBIT_DURATION 365.24;
//#define EARTH_REAL_SPIN_DURATION  1.0;
//#define EARTH_REAL_SCALE          (1.0/109.231)

Earth::Earth(Planet *owner) : Planet(owner)
{
	mesh = Sphere::sphereMesh();
	material = NULL;
	_orbitDuration = EARTH_ORBIT_DURATION;
	_spinDuration  = EARTH_SPIN_DURATION;
	_orbitRadius   = EARTH_DISTANCE_FROM_SUN * SCALE_SIZES_AMMOUNT;
	setScale(EARTH_RADIUS * SCALE_SIZES_AMMOUNT);
	_name = L"Earth";
}


Earth::~Earth(void)
{
	if (material) delete material; material = NULL;
}

void Earth::startBodyAudio()
{
	PlaySound(EARTH_AUDIO_FILENAME, NULL, SND_ASYNC);
}

MeshMaterial *Earth::createEarthMaterial(TextureManager *textureManager)
{
		MeshMaterial *earthMaterial = new MeshMaterial();

		ID3D11ShaderResourceView *earthTexture;
	    HRESULT hr = textureManager->createTexture(EARTH_TEXTURE_FILE, &earthTexture);
		if (FAILED(hr)) exit(-1);
		earthMaterial->texture = earthTexture;

		ID3D11ShaderResourceView *earthTextureBumpMap;
		hr = textureManager->createTexture(EARTH_TEXTURE_FILE_NIGHT, &earthTextureBumpMap);
		if (FAILED(hr)) return NULL;
		earthMaterial->bumpMapTexture = earthTextureBumpMap;
	
	return earthMaterial;
}

//void Earth::update(float dt, float wt)
//{
//	float t = dt;
//	applyRotation(0.0, t, 0.0);
//}

void Earth::render(ID3D11Device *renderingDevice, ID3D11DeviceContext *immediateContext)
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