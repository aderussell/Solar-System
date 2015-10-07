#include "SkyBox.h"
#include "Sphere.h"
#include "PlanetConstants.h"

#define SKYBOX_TEXTURE_FILE L"Media\\Star_skybox.png"

SkyBox::SkyBox(void)
{
	setScale(EDGE_OF_SOLAR_SYSTEM * SCALE_SIZES_AMMOUNT);
	mesh = Sphere::sphereMesh();
	material = NULL;
}


SkyBox::~SkyBox(void)
{
	if (material) delete material; material = NULL;
}


MeshMaterial *SkyBox::createSkyBoxMaterial(TextureManager *textureManager)
{
	MeshMaterial *moonMaterial = new MeshMaterial();

	ID3D11ShaderResourceView *moonTexture;
	HRESULT hr = textureManager->createTexture(SKYBOX_TEXTURE_FILE, &moonTexture);
	if (FAILED(hr)) exit(-1);
	moonMaterial->texture = moonTexture;

	return moonMaterial;
}

void SkyBox::render(ID3D11Device *renderingDevice, ID3D11DeviceContext *immediateContext)
{
	// Set vertex buffer
	UINT stride = this->mesh->buffers->vertexTypeSize();
	UINT offset = 0;
	immediateContext->IASetVertexBuffers( 0, 1, &(this->mesh->vertexBuffer), &stride, &offset );

	// Set index buffer
	immediateContext->IASetIndexBuffer( this->mesh->indexBuffer, DXGI_FORMAT_R16_UINT, 0 );

	

	//immediateContext->VSSetShader(material->vertexShader, NULL, 0);

	// setup the pixel shader
	immediateContext->PSSetShader(material->pixelShader, NULL, 0);
	immediateContext->PSSetShaderResources(0, 1, &(material->texture));
	//immediateContext->PSSetShaderResources(1, 1, &material->bumpMapTexture);

	// draw it out
	immediateContext->DrawIndexed(this->mesh->indices.size(), 0, 0 );
}

void SkyBox::update(float dt, float wt)
{

}