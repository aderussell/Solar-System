#pragma once
#include "Body.h"


class Cup : public Body
{
public:
	Cup(void);
	~Cup(void);

	static void createCupMaterials(ID3D11Device *renderingDevice, ID3D11DeviceContext *immediateContext);


	void render(ID3D11Device *renderingDevice, ID3D11DeviceContext *immediateContext);

	void update(float dt, float wt);


	MeshGroup *meshGroup;
};

