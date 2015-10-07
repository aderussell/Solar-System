#include "PlanetConstants.h"
#include "Phobos.h"

#define PHOBOS_BUMP_MAP_TEXTURE_FILE L"Media\\Phobos\\Phobos_Bumpmap.jpg"

Phobos::Phobos(Planet *owner) : SingleMeshPlanet(owner)
{
	setScale(MOON_RADIUS * SCALE_SIZES_AMMOUNT);
	_orbitDuration = MOON_ORBIT_DURATION;
	_spinDuration  = MOON_SPIN_DURATION;
	_orbitRadius   = MOON_DISTANCE_FROM_EARTH * SCALE_SIZES_AMMOUNT;
	_name = L"Phobos (Mars)";
}


Phobos::~Phobos(void)
{
}


MeshMaterial *Phobos::createPhobosMaterial(TextureManager *textureManager)
{
	MeshMaterial *moonMaterial = new MeshMaterial();

	ID3D11ShaderResourceView *moonTexture;
	HRESULT hr = textureManager->createTexture(PHOBOS_BUMP_MAP_TEXTURE_FILE, &moonTexture);
	if (FAILED(hr)) exit(-1);
	moonMaterial->texture = moonTexture;

	moonMaterial->bumpMapTexture = moonTexture;

	return moonMaterial;
}

void Phobos::render(ID3D11Device *renderingDevice, ID3D11DeviceContext *immediateContext)
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
	immediateContext->DrawIndexed( this->mesh->indices.size(), 0, 0 );
}
