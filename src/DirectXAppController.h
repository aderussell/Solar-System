#pragma once

#include "DXUT.h"
#include "DXUTgui.h"
#include "DXUTmisc.h"
#include "SDKmisc.h"

class DirectXAppController
{
public:

	enum DrawingMode
	{
		DrawingModePoints    = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST,
		DrawingModeLines     = D3D11_PRIMITIVE_TOPOLOGY_LINELIST,
		DrawingModeTriangles = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
	};


	enum MouseButtons
	{
		LeftButton   = (1 << 0),
		RightButton  = (1 << 1),
		MiddleButton = (1 << 2),
		SideButton1  = (1 << 3),
		SideButton2  = (1 << 5),
	};

	virtual void update(float dt, float wt) = 0;

	virtual void render(ID3D11Device* device, ID3D11DeviceContext* immediateContext) = 0;


	virtual void onKeyboardEvent(int key, bool keyDown, bool shiftDown, bool ctrlDown, bool altDown) = 0;

	virtual void onMouseWheelScrollEvent(float delta, int x, int y) = 0;

	virtual void onMouseButtonEvent(int button, bool isPressed, int buttons, int x, int y) = 0;

	virtual void onGamePadEvent(DXUT_GAMEPAD *pad) = 0;


	virtual void deviceCreated(ID3D11Device* device, ID3D11DeviceContext* immediateContext) = 0;
	virtual void deviceDestroyed() = 0;
	virtual void viewResize(int width, int height) = 0;

	virtual void renderText(CDXUTTextHelper *textHelper) = 0;

};

