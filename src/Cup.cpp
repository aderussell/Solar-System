#include "Cup.h"
#include "ObjLoader.h"

#define CUP_MESH_FILENAME "Media\\Cup\\Cup.obj"

Cup::Cup(void)
{
	meshGroup = ObjLoader::LoadMesh(CUP_MESH_FILENAME, true);
}


Cup::~Cup(void)
{
	if (meshGroup) {
		delete meshGroup;
		meshGroup = NULL;
	}
}

void Cup::render(ID3D11Device *renderingDevice, ID3D11DeviceContext *immediateContext)
{
	if (meshGroup) {
		meshGroup->render(renderingDevice, immediateContext);
	}
}
