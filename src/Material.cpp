#include "Material.h"


MeshMaterial::MeshMaterial()
{ 
	this->hasTexture = false;
	pixelShader = NULL;
	texture = NULL;
	bumpMapTexture = NULL;
}


MeshMaterial::~MeshMaterial()
{
	texture = NULL;
	bumpMapTexture = NULL;
	//if (texture) texture->Release();
	//if (bumpMapTexture) bumpMapTexture->Release();
}