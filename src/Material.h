#pragma once

#include <d3d11.h>
#include <xnamath.h>
#include <string>
#include <vector>
#include <unordered_map>
#include "ShaderManager.h"

struct SimpleVertex
{
    XMFLOAT3 Pos;	//Why not a float4?  See the shader strucrure.  Any thoughts?  Nigel
	XMFLOAT2 TexUV;
	XMFLOAT3 VecNormal;
};


typedef enum {
	IllumTypeNoSpecular = 1,
	IllumTypeSpecular = 2
} IllumType;

struct MeshMaterial
{
	MeshMaterial();
	~MeshMaterial();
	std::wstring name;

	XMFLOAT3 ka;
	XMFLOAT3 kd;
	XMFLOAT3 ks;

	IllumType illum;
	int ns;	//shininess

	std::wstring map_kd;	// texture file

	// texture
	bool hasTexture;
	unsigned int textureIndex;
	ID3D11ShaderResourceView *texture;

	// bump map
	bool hasBumpMap;
	ID3D11ShaderResourceView *bumpMapTexture;



	ID3D11PixelShader* pixelShader;
};