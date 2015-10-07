#include "MeshGroup.h"
#include <algorithm>
#include "ShaderManager.h"



MeshGroup::MeshGroup()
{
	indexBuffer  = NULL;
	vertexBuffer = NULL;
	std::unordered_map<std::wstring, MeshMaterial *> mats(0);
	materials = mats;
}

void MeshGroup::addMaterials(std::unordered_map<std::wstring, MeshMaterial *>& materials)
{
	for (auto kv : materials) {
		this->materials.insert(kv);
	}
}

HRESULT MeshGroup::createDeviceStuffs(ID3D11Device* device)
{
	HRESULT hr = S_OK;
	hr = createBuffers(device);
	if (FAILED(hr)) return hr;

	ShaderManager shaderManager = ShaderManager::sharedShaderManager();

	for (auto kv : materials) {
		MeshMaterial *material = kv.second;
		if (material->hasTexture) {
			material->pixelShader = shaderManager.pixelShaderForTexture(device);
		} else {
			material->pixelShader = shaderManager.pixelShaderForNoTexture(device);
		}
	}
}


HRESULT MeshGroup::createIndexBuffer(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC bd;
    ZeroMemory( &bd, sizeof(bd) );
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof( USHORT ) * this->indices.size();   //From sortOfMesh
	
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
    ZeroMemory( &InitData, sizeof(InitData) );
	InitData.pSysMem = &(this->indices[0]);					//From sortOfMesh

    hr = device->CreateBuffer( &bd, &InitData, &(this->indexBuffer) );
    return hr;
}

HRESULT MeshGroup::createVertexBuffer(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC bd;
    ZeroMemory( &bd, sizeof(bd) );
    bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof( SimpleVertex ) * this->vertices.size();	//From sortOfMesh
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;
    D3D11_SUBRESOURCE_DATA InitData;
    ZeroMemory( &InitData, sizeof(InitData) );
	SimpleVertex *vertices = &(this->vertices[0]);
	InitData.pSysMem = vertices;						//From sortOfMesh

    hr = device->CreateBuffer( &bd, &InitData, &(this->vertexBuffer) );

	return hr;
}

HRESULT MeshGroup::createBuffers(ID3D11Device* device)
	{
		HRESULT hr = S_OK;
		hr = createIndexBuffer(device);

		if (FAILED(hr)) return hr;

	    hr = createVertexBuffer(device);

		return hr;
	}


void MeshGroup::render(ID3D11Device *renderingDevice, ID3D11DeviceContext *immediateContext)
	{
		// Set vertex buffer
		UINT stride = sizeof( SimpleVertex );
		UINT offset = 0;
		immediateContext->IASetVertexBuffers( 0, 1, &(this->vertexBuffer), &stride, &offset );

		// Set index buffer
		immediateContext->IASetIndexBuffer( this->indexBuffer, DXGI_FORMAT_R16_UINT, 0 );

		// Set primitive topology
		immediateContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );


		int start = 0;
		int length = 0;
		int numberOfSubsets = this->startOfRange.size();
		for (int i = 0; i < numberOfSubsets; i++) {

			start = this->startOfRange[i];

			int end = (i < (numberOfSubsets - 1)) ? this->startOfRange[i+1] : indices.size()-1;
			int numberToDraw = end - start + 1;
			
			std::wstring materialName = this->materialAtRange[i];
			MeshMaterial *material = NULL;
			std::unordered_map<std::wstring, MeshMaterial *>::const_iterator got = materials.find(materialName);
			if (got != materials.end()) {
				material = got->second;
			}

			if (material) {
				if (material->pixelShader) {
					immediateContext->PSSetShader( material->pixelShader, NULL, 0 );
				}
				if (material->hasTexture) {

				}
			}

			immediateContext->DrawIndexed(numberToDraw, start, 0 );
		}
	}


void MeshGroup::updatePosition(float dt, float wt)
{
	float t = wt / 1000.0;
	this->matrix = XMMatrixRotationRollPitchYaw(t, t, t);
}