#include "TextureManager.h"


TextureManager::TextureManager(ID3D11Device *device)
{
	_device = device;
}


TextureManager::~TextureManager(void)
{
	removeAllTextures();
}




HRESULT TextureManager::createTexture(LPWSTR filename, ID3D11ShaderResourceView **texture)
{
	HRESULT hr = S_OK;

	// check it doesn't already exist on device
	ID3D11ShaderResourceView *textureResource = textureForFilename(filename);
	if (textureResource) {
		*texture = textureResource;
		return hr;
	}

	// creat the resource for the texture
	hr = D3DX11CreateShaderResourceViewFromFile(_device, filename, NULL, NULL, &textureResource, NULL);

	if (SUCCEEDED(hr)) {
		// add it to map
		std::pair<LPWSTR, ID3D11ShaderResourceView *> pair(filename, textureResource);
		_textures.insert(pair);

		// send out the address if a pointer has been provided
		//if (*texture) {
			*texture = textureResource;
		//}
	}

	return hr;
}

ID3D11ShaderResourceView *TextureManager::textureForFilename(LPWSTR filename)
{
	ID3D11ShaderResourceView *texture = NULL;
	std::unordered_map<LPWSTR, ID3D11ShaderResourceView *>::const_iterator got = _textures.find(filename);
	if (got != _textures.end()) {
		texture = got->second;
	}
	return texture;
}


void TextureManager::removeAllTextures()
{
	for (auto kv : _textures)
	{
		ID3D11ShaderResourceView *resource = kv.second;
		resource->Release();
	}
	_textures.empty();
}