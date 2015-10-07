#pragma once

#include <d3d11.h>
#include <vector>
#include <d3dx11.h>


class ShaderManager
{
public:

	ShaderManager(ID3D11Device *renderingDevice);

	static ShaderManager& sharedShaderManager();

	HRESULT createTexture(unsigned int *index, LPWSTR filename, ID3D11Device *device, ID3D11DeviceContext *immediateContext);

	int nextIndex() { return _nextIndex; };

	ID3D11ShaderResourceView *shaderForIndex(unsigned int index);

	ID3D11PixelShader* pixelShaderForTextureAndLightingAndDark(ID3D11Device *renderingDevice);

	ID3D11PixelShader* pixelShaderForTextureAndLightingAndBumpmap(ID3D11Device *renderingDevice);

	ID3D11PixelShader* pixelShaderForTextureAndLighting(ID3D11Device *renderingDevice);

	ID3D11PixelShader* pixelShaderForTexture(ID3D11Device *renderingDevice);

	ID3D11PixelShader *pixelShaderForTextureWithSpotlight(ID3D11Device *renderingDevice);

	ID3D11PixelShader* pixelShaderForNoTexture(ID3D11Device *renderingDevice);
	

	ID3D11PixelShader* pixelShaderForTexture();

	ID3D11PixelShader* pixelShaderForTextureAndBumpMap();

	ID3D11PixelShader* pixelShaderForNoTexture();

	//~ShaderManager();

private:

	ShaderManager();

	ID3D11Device *_device;

	//static ShaderManager *_sharedInstance;

	std::vector<ID3D11ShaderResourceView *> shaders;
	int _nextIndex;

	ID3D11PixelShader* _pixelShaderForTexture;
	ID3D11PixelShader* _pixelShaderForNoTexture;
	ID3D11PixelShader* _pixelShaderForTextureWithSpotlight;
	ID3D11PixelShader * _pixelShaderForTextureAndLighting;
	ID3D11PixelShader * _pixelShaderForTextureAndLightingAndDark;
	ID3D11PixelShader * _pixelShaderForTextureAndLightingAndBumpmap;
};

HRESULT CompileShaderFromFile( WCHAR* szFileName,		// File Name
							  LPCSTR szEntryPoint,		// Namee of shader
							  LPCSTR szShaderModel,		// Shader model
							  ID3DBlob** ppBlobOut );	// Blob returned