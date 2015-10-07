#include "Mesh.h"
#include <cassert>


Mesh::Mesh(void)
{
	indexBuffer  = NULL;
	vertexBuffer = NULL;
	buffers = NULL;
}


Mesh::~Mesh(void)
{
	if (buffers) delete buffers; buffers = NULL;
}

HRESULT Mesh::createBuffers(ID3D11Device* device)
{
	calculateTangentsAndBinormals();
	buffers = new DeviceMeshBuffers<Vertex, USHORT>(vertices_NEW, indices, device);
	indexBuffer = buffers->indexBuffer();
	vertexBuffer = buffers->vertexBuffer();

	return S_OK;

		//HRESULT hr = S_OK;
		//hr = createIndexBuffer(device);

		//if (FAILED(hr)) return hr;

	 //   hr = createVertexBuffer(device);

		//return hr;
}

void calculateTangentAndBiNormal(SimpleVertex v1, SimpleVertex v2, SimpleVertex v3, XMFLOAT3 &tangent, XMFLOAT3 &binormal);

void Mesh::calculateTangentsAndBinormals()
{
	std::vector<Vertex> newVertices(0);

	int faceCount = vertices.size() / 3;
	int index = 0;
	for (int i = 0; i < faceCount; i++) {
		SimpleVertex v1 = vertices[index++];
		SimpleVertex v2 = vertices[index++];
		SimpleVertex v3 = vertices[index++];

		XMFLOAT3 tangent;
		XMFLOAT3 binormal;

		calculateTangentAndBiNormal(v1, v2, v3, tangent, binormal);

		// create the other type here
		Vertex vertex1;
		vertex1.position  = v1.Pos;
		vertex1.textureUV = v1.TexUV;
		vertex1.normal    = v1.VecNormal;
		vertex1.tangent   = tangent;
		vertex1.binormal  = binormal;

		Vertex vertex2;
		vertex2.position  = v2.Pos;
		vertex2.textureUV = v2.TexUV;
		vertex2.normal    = v2.VecNormal;
		vertex2.tangent   = tangent;
		vertex2.binormal  = binormal;

		Vertex vertex3;
		vertex3.position  = v3.Pos;
		vertex3.textureUV = v3.TexUV;
		vertex3.normal    = v3.VecNormal;
		vertex3.tangent   = tangent;
		vertex3.binormal  = binormal;

		newVertices.push_back(vertex1);
		newVertices.push_back(vertex2);
		newVertices.push_back(vertex3);
	}
	vertices_NEW = newVertices;
}

void calculateTangentAndBiNormal(SimpleVertex v1, SimpleVertex v2, SimpleVertex v3, XMFLOAT3 &tangent, XMFLOAT3 &binormal)
{
	float vector1[3], vector2[3];
	float tuVector[2], tvVector[2];
	float den;
	float length;


	// Calculate the two vectors for this face.
	vector1[0] = v2.Pos.x - v1.Pos.x;
	vector1[1] = v2.Pos.y - v1.Pos.y;
	vector1[2] = v2.Pos.z - v1.Pos.z;

	vector2[0] = v3.Pos.x - v1.Pos.x;
	vector2[1] = v3.Pos.y - v1.Pos.y;
	vector2[2] = v3.Pos.z - v1.Pos.z;

	// Calculate the tu and tv texture space vectors.
	tuVector[0] = v2.TexUV.x - v1.TexUV.x;
	tvVector[0] = v2.TexUV.y - v1.TexUV.y;

	tuVector[1] = v3.TexUV.x - v1.TexUV.x;
	tvVector[1] = v3.TexUV.y - v1.TexUV.y;

	// Calculate the denominator of the tangent/binormal equation.
	den = 1.0f / (tuVector[0] * tvVector[1] - tuVector[1] * tvVector[0]);

	// Calculate the cross products and multiply by the coefficient to get the tangent and binormal.
	tangent.x = (tvVector[1] * vector1[0] - tvVector[0] * vector2[0]) * den;
	tangent.y = (tvVector[1] * vector1[1] - tvVector[0] * vector2[1]) * den;
	tangent.z = (tvVector[1] * vector1[2] - tvVector[0] * vector2[2]) * den;

	binormal.x = (tuVector[0] * vector2[0] - tuVector[1] * vector1[0]) * den;
	binormal.y = (tuVector[0] * vector2[1] - tuVector[1] * vector1[1]) * den;
	binormal.z = (tuVector[0] * vector2[2] - tuVector[1] * vector1[2]) * den;

	// Calculate the length of this normal.
	length = sqrt((tangent.x * tangent.x) + (tangent.y * tangent.y) + (tangent.z * tangent.z));
			
	// Normalize the normal and then store it
	tangent.x = tangent.x / length;
	tangent.y = tangent.y / length;
	tangent.z = tangent.z / length;

	// Calculate the length of this normal.
	length = sqrt((binormal.x * binormal.x) + (binormal.y * binormal.y) + (binormal.z * binormal.z));
			
	// Normalize the normal and then store it
	binormal.x = binormal.x / length;
	binormal.y = binormal.y / length;
	binormal.z = binormal.z / length;

	return;
}




//-------------------------------------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------------------------------------//

template <class Tv, class Ti>
DeviceMeshBuffers<Tv, Ti>::DeviceMeshBuffers(std::vector<Tv> &vertices, std::vector<Ti> &indices, ID3D11Device* device)
{
	HRESULT hr = S_OK;
	_indexBuffer = _vertexBuffer = NULL;
	hr = createIndexBuffer(device, indices, &_indexBuffer);
	assert(SUCCEEDED(hr));
	hr = createVertexBuffer(device, vertices, &_vertexBuffer);
	assert(SUCCEEDED(hr));
}

template <class Tv, class Ti>
DeviceMeshBuffers<Tv, Ti>::~DeviceMeshBuffers()
{
	_indexBuffer->Release();
	_vertexBuffer->Release();
	_indexBuffer = _vertexBuffer = NULL;
}




template <class Tv, class Ti>
ID3D11Buffer *DeviceMeshBuffers<Tv, Ti>::indexBuffer()
{
	return _indexBuffer;
}

template <class Tv, class Ti>
ID3D11Buffer *DeviceMeshBuffers<Tv, Ti>::vertexBuffer()
{
	return _vertexBuffer;
}

template <class Tv, class Ti>
HRESULT DeviceMeshBuffers<Tv, Ti>::createIndexBuffer(ID3D11Device* device, std::vector<Ti> &indices, ID3D11Buffer **buffer)
{
	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC bd;
    ZeroMemory( &bd, sizeof(bd) );
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof( Ti ) * indices.size();   //From sortOfMesh
	
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
    ZeroMemory( &InitData, sizeof(InitData) );
	InitData.pSysMem = &(indices[0]);					//From sortOfMesh

    hr = device->CreateBuffer( &bd, &InitData, buffer );
    return hr;
}

template <class Tv, class Ti>
HRESULT DeviceMeshBuffers<Tv, Ti>::createVertexBuffer(ID3D11Device* device, std::vector<Tv> &vertices, ID3D11Buffer **buffer)
{
	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC bd;
    ZeroMemory( &bd, sizeof(bd) );
    bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof( Tv ) * vertices.size();	//From sortOfMesh
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;
    D3D11_SUBRESOURCE_DATA InitData;
    ZeroMemory( &InitData, sizeof(InitData) );
	Tv *firstVertex = &(vertices[0]);
	InitData.pSysMem = firstVertex;						//From sortOfMesh

    hr = device->CreateBuffer( &bd, &InitData, buffer );

	return hr;
}