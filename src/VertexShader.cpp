#include "VertexShader.h"
#include "DXUT.h"
#include "ShaderManager.h"

VertexShader::VertexShader()
{
	_vertexShaderByteCode = NULL;
	_vertexShader         = NULL;
	_inputLayout          = NULL;
}

VertexShader::~VertexShader()
{
	destroyVertexShader();
	SAFE_RELEASE(_vertexShaderByteCode);
}

ID3D11VertexShader *VertexShader::vertexShader()
{
	return _vertexShader;
}

ID3D11InputLayout  *VertexShader::inputLayout()
{
	return _inputLayout;
}

void VertexShader::destroyVertexShader()
{
	SAFE_RELEASE(_vertexShader);
	SAFE_RELEASE(_inputLayout);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

#define VERTEX_SHADER_FILENAME      L"Start of OBJ loader VS.hlsl"
#define VERTEX_SHADER_FUNCTION_NAME "VS_obj"

HRESULT SimpleVertexShader::compileShader()
{
	HRESULT hr = S_OK;
	hr = CompileShaderFromFile( VERTEX_SHADER_FILENAME, VERTEX_SHADER_FUNCTION_NAME, "vs_5_0", &_vertexShaderByteCode );
	if(FAILED( hr )) {
		MessageBox( NULL, L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK );
	}
	return hr;
}

HRESULT SimpleVertexShader::createVertexShader(ID3D11Device *device)
{
	HRESULT hr = S_OK;
	ID3DBlob* pVSBlob = _vertexShaderByteCode;

	//**********************************************************************//
    // Create the vertex shader.											//
	//**********************************************************************//
		hr = device->CreateVertexShader( pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &_vertexShader );
		if(FAILED( hr )) {    
			pVSBlob->Release();
			return hr;
		}

		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,                            D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD",  0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
    
		UINT numElements = ARRAYSIZE( layout );

    // Create the input layout
		hr = device->CreateInputLayout( layout, numElements, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &_inputLayout );
		pVSBlob->Release();
		if( FAILED( hr ) )
			return hr;

	return hr;
}

void SimpleVertexShader::setVertexShaderForDeviceContext(ID3D11DeviceContext *deviceContext)
{
	deviceContext->VSSetShader( _vertexShader, NULL, 0 );
	deviceContext->IASetInputLayout( _inputLayout );
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

#define VERTEX_SHADER_FILENAME_2      L"Solar_System_VS.hlsl"
#define VERTEX_SHADER_FUNCTION_NAME "VS_obj"

HRESULT StandardVertexShader::compileShader()
{
	HRESULT hr = S_OK;
	hr = CompileShaderFromFile( VERTEX_SHADER_FILENAME_2, VERTEX_SHADER_FUNCTION_NAME, "vs_5_0", &_vertexShaderByteCode );
	if(FAILED( hr )) {
		MessageBox( NULL, L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK );
	}
	return hr;
}

HRESULT StandardVertexShader::createVertexShader(ID3D11Device *device)
{
	HRESULT hr = S_OK;
	
	ID3DBlob* pVSBlob = _vertexShaderByteCode;

	//**********************************************************************//
    // Create the vertex shader.											//
	//**********************************************************************//
		hr = device->CreateVertexShader( pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &_vertexShader );
		if(FAILED( hr )) {    
			pVSBlob->Release();
			return hr;
		}

		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,						    D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD",  0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BINORMAL",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
    
		UINT numElements = 5;

    // Create the input layout
		hr = device->CreateInputLayout( layout, numElements, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &_inputLayout );
		pVSBlob->Release();
		if( FAILED( hr ) )
			return hr;

		return hr;
}

void StandardVertexShader::setVertexShaderForDeviceContext(ID3D11DeviceContext *deviceContext)
{
	deviceContext->VSSetShader( _vertexShader, NULL, 0 );
	deviceContext->IASetInputLayout( _inputLayout );
}

