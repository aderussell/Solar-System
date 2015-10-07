#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <d3d11.h>
#include <d3dx11.h>

/** Class for managing all of the textures for a device */
class TextureManager
{
public:

	TextureManager(ID3D11Device *device);
	~TextureManager(void);


	void removeAllTextures();
	

	HRESULT createTexture(LPWSTR filename, ID3D11ShaderResourceView **texture);

	// return NULL if there isn't one
	ID3D11ShaderResourceView *textureForFilename(LPWSTR filename);

private:
	ID3D11Device *_device;

	std::unordered_map<LPWSTR, ID3D11ShaderResourceView *> _textures;
};

