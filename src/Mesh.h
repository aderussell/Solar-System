#pragma once

#include <unordered_map>
#include "Material.h"

struct Vertex
{
    XMFLOAT3 position;		// the position of the vertex
    XMFLOAT2 textureUV;		// the texture co-ord
	XMFLOAT3 normal;
	XMFLOAT3 tangent;
	XMFLOAT3 binormal;
};





template<class Tv, class Ti>
class DeviceMeshBuffers
{
public:
	DeviceMeshBuffers(std::vector<Tv> &vertices, std::vector<Ti> &indices, ID3D11Device* device);
	~DeviceMeshBuffers();

	ID3D11Buffer *indexBuffer();
	ID3D11Buffer *vertexBuffer();

	size_t vertexTypeSize() { return sizeof(Tv);  }
	size_t indexTypeSize()  { return sizeof (Ti); }

protected:
	ID3D11Buffer *_indexBuffer;
	ID3D11Buffer *_vertexBuffer;

	HRESULT createIndexBuffer(ID3D11Device* device, std::vector<Ti> &indices, ID3D11Buffer **buffer);
	HRESULT createVertexBuffer(ID3D11Device* device, std::vector<Tv> &vertices, ID3D11Buffer **buffer);
};


class Mesh
{
public:
	Mesh(void);
	~Mesh(void);

	std::vector<SimpleVertex> vertices;
	std::vector<USHORT> indices;

	

	std::vector<Vertex> vertices_NEW;

	void calculateTangentsAndBinormals();


	ID3D11Buffer *indexBuffer;
	ID3D11Buffer *vertexBuffer;

	HRESULT createBuffers(ID3D11Device* device);

	DeviceMeshBuffers<Vertex, USHORT> *buffers;
};

