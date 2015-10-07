#include "Sphere.h"
#include "ObjLoader.h"

static Mesh *_sphereMesh;

void Sphere::initialiseSphereMesh(ID3D11Device* device)
{
	Mesh *sphere = ObjLoader::LoadSingleMesh("Media\\sphere\\sphere.obj");
	sphere->createBuffers(device);
	_sphereMesh = sphere;
}


void Sphere::destroySphereMesh()
{
	if (_sphereMesh) {
		delete _sphereMesh;
		_sphereMesh = NULL;
	}
}

Mesh *Sphere::sphereMesh()
{
	return _sphereMesh;
}