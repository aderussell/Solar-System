#pragma once

#include <d3d11.h>
#include <xnamath.h>
#include <string>
#include <vector>
#include <unordered_map>
#include "Material.h"






class MeshGroup
{
public:

	MeshGroup();
	~MeshGroup() {};

	std::vector<SimpleVertex> vertices;
	std::vector<USHORT> indices;
	std::vector<int> startOfRange;
	std::vector<int> startOfGroup;
	std::vector<std::wstring> materialAtRange;

	ID3D11Buffer *indexBuffer;
	ID3D11Buffer *vertexBuffer;

	std::unordered_map<std::wstring, MeshMaterial *> materials;

	void addMaterials(std::unordered_map<std::wstring, MeshMaterial *>& materials);

	HRESULT createDeviceStuffs(ID3D11Device* device);

	HRESULT createBuffers(ID3D11Device* device);

	HRESULT createIndexBuffer(ID3D11Device* device);
	HRESULT createVertexBuffer(ID3D11Device* device);

	void render(ID3D11Device *renderingDevice, ID3D11DeviceContext *immediateContext);

	XMMATRIX matrix;

	void updatePosition(float dt, float wt);
};
