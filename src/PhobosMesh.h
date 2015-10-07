#pragma once

#include "Mesh.h"

class PhobosMesh
{
public:
	static void initialisePhobosMesh(ID3D11Device* device);


	static void destroyPhobosMesh();

	static Mesh *phobosMesh();
};

