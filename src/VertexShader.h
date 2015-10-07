#pragma once

#include <d3dcompiler.h>
#include <d3d11.h>
#include <d3dx11.h>

class VertexShader
{
public:
	VertexShader();
	~VertexShader();
	virtual HRESULT compileShader() = 0;
	virtual HRESULT createVertexShader(ID3D11Device *device) = 0;
	virtual void destroyVertexShader();

	virtual void setVertexShaderForDeviceContext(ID3D11DeviceContext *deviceContext) = 0;

	ID3D11VertexShader *vertexShader();
	ID3D11InputLayout  *inputLayout();

protected:
	ID3DBlob           *_vertexShaderByteCode;
	ID3D11VertexShader *_vertexShader;
	ID3D11InputLayout  *_inputLayout;
};

//-------------------------------------------------------------------------//
// a vertex shader from Nigel's stuff
class SimpleVertexShader : public VertexShader
{
public:
	HRESULT compileShader();
	HRESULT createVertexShader(ID3D11Device *device);

	void setVertexShaderForDeviceContext(ID3D11DeviceContext *deviceContext);
};

//-------------------------------------------------------------------------//
// a vertes shader for Adrian's stuff
class StandardVertexShader : public VertexShader
{
public:
	HRESULT compileShader();
	HRESULT createVertexShader(ID3D11Device *device);

	void setVertexShaderForDeviceContext(ID3D11DeviceContext *deviceContext);
};