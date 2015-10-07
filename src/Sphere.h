#pragma once

#include "Mesh.h"

class Sphere
{
public:
	static void initialiseSphereMesh(ID3D11Device* device);


	static void destroySphereMesh();

	static Mesh *sphereMesh();
};

