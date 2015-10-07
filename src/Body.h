#pragma once
#include <d3d11.h>
#include <d3dx11.h>
#include <xnamath.h>
#include "MeshGroup.h"


class Body
{
public:
	Body(void);
	~Body(void);

	enum RenderWinding
	{
		RenderWindingCW,
		RenderWindingCCW,
		RenderWindingCWThenCCW,
		RenderWindingCCWThenCw,
	};


	void applyTranslation(XMVECTOR translation);
	void applyScale(float scale);
	void applyRotation(float x, float y, float z);
	void applyRotation(XMVECTOR rotation);

	void setPosition(float x, float y, float z);
	void setPosition(XMVECTOR translation);
	void setScale(float scale);
	void setRotation(XMVECTOR rotation);


	virtual XMMATRIX positionMatrix();
	virtual XMMATRIX rotationMatrix();
	virtual XMMATRIX scaleMatrix();
	virtual XMMATRIX worldMatrix();

	float scale() { return _scale; }
	virtual XMVECTOR position() { return _position; }
	virtual XMVECTOR rotation() { return _rotation; }

	// where to put some sort of behaviour, e.g. updating position
	//virtual void update(float dt, float wt) {};

	// TODO: make planet : body class

	//virtual XMMATRIX worldMatrixInTime(float dt) = 0;
	

	//virtual void startBodyAudio() {};

	virtual void render(ID3D11Device *renderingDevice, ID3D11DeviceContext *immediateContext) = 0;

protected:
	unsigned int textureIndex;

	float    _scale;

	XMVECTOR _position;
	XMVECTOR _rotation;

	MeshGroup *_meshGroup;

};

