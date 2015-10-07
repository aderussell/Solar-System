#pragma once

#include <unordered_map>
#include <string>
#include "MeshGroup.h"
#include "Mesh.h"

class ObjLoader
{
	public:

		ObjLoader(LPSTR filename);
		~ObjLoader();		

		MeshGroup *meshGroup;

		// static methods
		static MeshGroup *LoadMesh(LPSTR filename, bool loadMaterials = true);
		static std::unordered_map<std::wstring, MeshMaterial *> loadMeshMaterial(LPSTR filename);

		//static MeshGroup *LoadMesh(LPSTR filename, bool loadMaterials = true);

		static Mesh *LoadSingleMesh(LPSTR filename, bool loadMaterials = true);



		// DEPRECATED
		ObjLoader(LPSTR filename, LPSTR mtlFilename);				// DEPREACTED!! DON't USE!!!
		MeshGroup *LoadMesh2(LPSTR filename, LPSTR mtlFilename);	// DEPREACTED!! DON't USE!!!
};
