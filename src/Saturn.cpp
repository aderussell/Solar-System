#include "Saturn.h"
#include "PlanetConstants.h"

#define SATURN_TEXTURE_FILE          L"Media\\Saturn\\Saturn_Texture.jpg"
#define SATURN_AUDIO_FILENAME        L"Media\\Saturn\\Saturn_Audio.wav"

Saturn::Saturn(Planet *owner) : SingleMeshPlanet(owner)
{
	_orbitDuration = SATURN_ORBIT_DURATION;
	_spinDuration  = SATURN_SPIN_DURATION;
	_orbitRadius   = SATURN_DISTANCE_FROM_SUN * SCALE_SIZES_AMMOUNT;
	setScale(SATURN_RADIUS * SCALE_SIZES_AMMOUNT);
	_name = L"Saturn";
	ringMesh = NULL;
	ringMaterial = NULL;
}


Saturn::~Saturn(void)
{
}

void Saturn::startBodyAudio()
{
	PlaySound(SATURN_AUDIO_FILENAME, NULL, SND_ASYNC);
}

MeshMaterial *Saturn::createSaturnMaterial(TextureManager *textureManager)
{
		MeshMaterial *earthMaterial = new MeshMaterial();

		ID3D11ShaderResourceView *earthTexture;
	    HRESULT hr = textureManager->createTexture(SATURN_TEXTURE_FILE, &earthTexture);
		if (FAILED(hr)) exit(-1);
		earthMaterial->texture = earthTexture;
	
	return earthMaterial;
}


void Saturn::render(ID3D11Device *renderingDevice, ID3D11DeviceContext *immediateContext)
{
	// Set vertex buffer
	UINT stride = this->mesh->buffers->vertexTypeSize();
	UINT offset = 0;
	ID3D11Buffer *vertexBuffer = this->mesh->buffers->vertexBuffer();
	immediateContext->IASetVertexBuffers( 0, 1, &(vertexBuffer), &stride, &offset );

	// Set index buffer
	ID3D11Buffer *indexBuffer = this->mesh->buffers->indexBuffer();
	immediateContext->IASetIndexBuffer( indexBuffer, DXGI_FORMAT_R16_UINT, 0 );

	// setup the pixel shader
	immediateContext->PSSetShader(material->pixelShader, NULL, 0);
	immediateContext->PSSetShaderResources(0, 1, &(material->texture));
	//immediateContext->PSSetShaderResources(1, 1, &material->bumpMapTexture);

	// draw it out
	immediateContext->DrawIndexed(this->mesh->indices.size(), 0, 0 );


}