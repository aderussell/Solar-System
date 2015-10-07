#include "ShaderManager.h"
#include "ObjLoader.h"
#include <d3dcompiler.h>
#include "String_Additions.h"


//#define PIXEL_SHADER_FILENAME L"Start of OBJ loader PS.hlsl"
#define PIXEL_SHADER_FILENAME L"Solar_System_PS.hlsl"

ShaderManager& ShaderManager::sharedShaderManager()
{
	static ShaderManager instance;
	return instance;
}

ShaderManager::ShaderManager(ID3D11Device *renderingDevice)
{
	_device = renderingDevice;
}

ShaderManager::ShaderManager()
{
	_nextIndex = 0;
}

//ShaderManager::~ShaderManager()
//{
//	if (_pixelShaderForTexture)   delete _pixelShaderForTexture;
//	if (_pixelShaderForNoTexture) delete _pixelShaderForNoTexture;
//}

ID3D11PixelShader* ShaderManager::pixelShaderForTextureWithSpotlight(ID3D11Device *renderingDevice)
{
	if (!_pixelShaderForTextureWithSpotlight) {
		HRESULT hr = S_OK;
		ID3DBlob* pPSBlob = NULL;
		hr = CompileShaderFromFile( PIXEL_SHADER_FILENAME, "PS_TexturesWithSpotlight", "ps_4_0", &pPSBlob );

		if(FAILED( hr )) {
			MessageBox( NULL, L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK );
			return NULL;
		}

		hr = renderingDevice->CreatePixelShader( pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &_pixelShaderForTextureWithSpotlight );
		pPSBlob->Release();
		if( FAILED( hr ) )
			return NULL;
	}

	return _pixelShaderForTextureWithSpotlight;
}

ID3D11PixelShader* ShaderManager::pixelShaderForTexture(ID3D11Device *renderingDevice)
{
	if (!_pixelShaderForTexture) {
		HRESULT hr = S_OK;
		ID3DBlob* pPSBlob = NULL;
		hr = CompileShaderFromFile( PIXEL_SHADER_FILENAME, "PS_TexturesNoLighting", "ps_4_0", &pPSBlob );
		//hr = CompileShaderFromFile( L"Start of OBJ loader PS.hlsl", "PS_TexturesWithLighting", "ps_4_0", &pPSBlob );

		if(FAILED( hr )) {
			MessageBox( NULL, L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK );
			return NULL;
		}

		hr = renderingDevice->CreatePixelShader( pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &_pixelShaderForTexture );
		pPSBlob->Release();
		if( FAILED( hr ) )
			return NULL;
	}

	return _pixelShaderForTexture;
}

ID3D11PixelShader* ShaderManager::pixelShaderForTextureAndLighting(ID3D11Device *renderingDevice)
{
	if (!_pixelShaderForTextureAndLighting) {
		HRESULT hr = S_OK;
		ID3DBlob* pPSBlob = NULL;
		//hr = CompileShaderFromFile( PIXEL_SHADER_FILENAME, "PS_TexturesNoLighting", "ps_4_0", &pPSBlob );
		hr = CompileShaderFromFile( PIXEL_SHADER_FILENAME, "PS_TexturesWithLighting", "ps_4_0", &pPSBlob );

		if(FAILED( hr )) {
			MessageBox( NULL, L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK );
			return NULL;
		}

		hr = renderingDevice->CreatePixelShader( pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &_pixelShaderForTextureAndLighting );
		pPSBlob->Release();
		if( FAILED( hr ) )
			return NULL;
	}

	return _pixelShaderForTextureAndLighting;
}





ID3D11PixelShader* ShaderManager::pixelShaderForTextureAndLightingAndDark(ID3D11Device *renderingDevice)
{
	if (!_pixelShaderForTextureAndLightingAndDark) {
		HRESULT hr = S_OK;
		ID3DBlob* pPSBlob = NULL;
		//hr = CompileShaderFromFile(PIXEL_SHADER_FILENAME, "PS_TexturesNoLighting", "ps_4_0", &pPSBlob );
		hr = CompileShaderFromFile( PIXEL_SHADER_FILENAME, "PS_TexturesWithDarkTexture", "ps_4_0", &pPSBlob );

		if(FAILED( hr )) {
			MessageBox( NULL, L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK );
			return NULL;
		}

		hr = renderingDevice->CreatePixelShader( pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &_pixelShaderForTextureAndLightingAndDark );
		pPSBlob->Release();
		if( FAILED( hr ) )
			return NULL;
	}

	return _pixelShaderForTextureAndLightingAndDark;
}


ID3D11PixelShader* ShaderManager::pixelShaderForTextureAndLightingAndBumpmap(ID3D11Device *renderingDevice)
{
	if (!_pixelShaderForTextureAndLightingAndBumpmap) {
		HRESULT hr = S_OK;
		ID3DBlob* pPSBlob = NULL;
		//hr = CompileShaderFromFile(PIXEL_SHADER_FILENAME, "PS_TexturesNoLighting", "ps_4_0", &pPSBlob );
		hr = CompileShaderFromFile( PIXEL_SHADER_FILENAME, "PS_TextureWithBumpMap", "ps_4_0", &pPSBlob );

		if(FAILED( hr )) {
			MessageBox( NULL, L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK );
			return NULL;
		}

		hr = renderingDevice->CreatePixelShader( pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &_pixelShaderForTextureAndLightingAndBumpmap );
		pPSBlob->Release();
		if( FAILED( hr ) )
			return NULL;
	}

	return _pixelShaderForTextureAndLightingAndBumpmap;
}



ID3D11PixelShader* ShaderManager::pixelShaderForNoTexture(ID3D11Device *renderingDevice)
{
	if (!_pixelShaderForNoTexture) {
		HRESULT hr = S_OK;
		ID3DBlob* pPSBlob = NULL;
		hr = CompileShaderFromFile( PIXEL_SHADER_FILENAME, "PS_NoTexturesWithLighting", "ps_4_0", &pPSBlob );

		if(FAILED( hr )) {
			MessageBox( NULL, L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK );
			return NULL;
		}

		hr = renderingDevice->CreatePixelShader( pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &_pixelShaderForNoTexture );
		pPSBlob->Release();
		if( FAILED( hr ) )
			return NULL;
	}

	return _pixelShaderForNoTexture;
}


HRESULT ShaderManager::createTexture(unsigned int *index, LPWSTR filename, ID3D11Device *device, ID3D11DeviceContext *immediateContext)
	{
		HRESULT hr = S_OK;

		ID3D11ShaderResourceView           *textureResource   = NULL;

		hr = D3DX11CreateShaderResourceViewFromFile( device, 
												 filename, 
												 NULL, NULL, 
												 &textureResource,		// This is returned.
												 NULL );

		immediateContext->PSSetShaderResources( _nextIndex, 1, &textureResource );

		if (index != NULL) {
			*index = _nextIndex;
		}

		// if creating texture was successful then incrment the next texture index
		if (SUCCEEDED(hr)) _nextIndex++;

		return hr;
	}


HRESULT CompileShaderFromFile( WCHAR* szFileName,		// File Name
							  LPCSTR szEntryPoint,		// Namee of shader
							  LPCSTR szShaderModel,		// Shader model
							  ID3DBlob** ppBlobOut )	// Blob returned
{
    HRESULT hr = S_OK;

    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
    // Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
    // Setting this flag improves the shader debugging experience, but still allows 
    // the shaders to be optimized and to run exactly the way they will run in 
    // the release configuration of this program.
    dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

    ID3DBlob* pErrorBlob;
    hr = D3DX11CompileFromFile( szFileName, NULL, NULL, szEntryPoint, szShaderModel, dwShaderFlags, 0, NULL, ppBlobOut, &pErrorBlob, NULL );
   if( FAILED(hr) )
    {
		WCHAR errorCharsW[200];
        if( pErrorBlob != NULL )
		{
			charStrToWideChar(errorCharsW, (char *)pErrorBlob->GetBufferPointer());
            MessageBox( 0, errorCharsW, L"Error", 0 );
		}
    }
    if( pErrorBlob ) pErrorBlob->Release();

    return S_OK;
}