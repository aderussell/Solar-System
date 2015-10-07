#include "PhobosMesh.h"
#include "ObjLoader.h"

static Mesh *_phobosMesh;

void PhobosMesh::initialisePhobosMesh(ID3D11Device* device)
{
	Mesh *sphere = ObjLoader::LoadSingleMesh("Media\\Phobos\\Phobos.obj");
	sphere->createBuffers(device);
	_phobosMesh = sphere;
}


void PhobosMesh::destroyPhobosMesh()
{
	if (_phobosMesh) {
		delete _phobosMesh;
		_phobosMesh = NULL;
	}
}

Mesh *PhobosMesh::phobosMesh()
{
	return _phobosMesh;
}