#pragma once

#include "DirectXAppController.h"

#include "PlanetConstants.h"

#include "Planet.h"

// the skybox class
#include "SkyBox.h"

// the sun class
#include "Sun.h"

// the planets classes
#include "Mercury.h"
#include "Venus.h"
#include "Earth.h"
#include "Mars.h"
#include "Jupiter.h"
#include "Saturn.h"
#include "Uranus.h"
#include "Neptune.h"

// the moons classes
#include "Moon.h"
#include "Phobos.h"

// the mesh classes
#include "Sphere.h"
#include "PhobosMesh.h"


#include "Planet.h"
#include <vector>

#include "Camera.h"

#include "VertexShader.h"


//--------------------------------------------------------


//**************************************************************************//
// Light vector never moves; and colour never changes.  I have done it .	//
// this way to show how constant buffers can be used so that you don't		//
// upsate stuff you don't need to.											//
// Beware of constant buffers that aren't in multiples of 16 bytes..		//
//**************************************************************************//
struct CBNeverChanges
{
    XMFLOAT4 materialColour;
	XMVECTOR vecLight;			// Must be 4, we only use the first 3.
};

//**************************************************************************//
// Note we do it properly here and pass the WVP matrix, rather than world,	//
// view and projection matrices separately.									//
//																			//
// We still need the world matrix to transform the normal vectors.			//
//**************************************************************************//
struct CBChangesEveryFrame
{
    XMMATRIX matWorld;
	XMMATRIX matWorldViewProjection;
};

//----------------------------------------------------------------------------------------


class SolarSystem : public DirectXAppController
{
public:
	SolarSystem(float width, float height);
	~SolarSystem();

	void update(float dt, float wt);

	void render(ID3D11Device* device, ID3D11DeviceContext* immediateContext);


	void onKeyboardEvent(int key, bool keyDown, bool shiftDown, bool ctrlDown, bool altDown);

	void onMouseWheelScrollEvent(float delta, int x, int y);

	void onMouseButtonEvent(int button, bool isPressed, int buttons, int x, int y);

	void onGamePadEvent(DXUT_GAMEPAD *pad);

	void deviceCreated(ID3D11Device* device, ID3D11DeviceContext* immediateContext);
	void deviceDestroyed();
	void viewResize(int width, int height);

	void renderText(CDXUTTextHelper *textHelper);

protected:
	SkyBox *skyBox;
	Sun *sun;
	std::vector<Planet *>planets;

	Earth *earth;
	Moon *moon;

	Mercury *mercury;
	Venus   *venus;
	Mars    *mars;
	Jupiter *jupiter;
	Saturn  *saturn;
	Uranus  *uranus;
	Neptune *neptune;

	Phobos *phobos;

	Planet *currentPlanet;

	Camera *_camera;

	DrawingMode currentDrawingMode;

	VertexShader *vertexShader;

	TextureManager *_textureManager;

ID3D11PixelShader*                  g_pPixelShader;
ID3D11PixelShader*                  g_pPixelShaderSun;
ID3D11PixelShader*                  g_pPixelShaderEarth;
ID3D11PixelShader*                  g_pPixelShaderMoon;

ID3D11SamplerState*                 g_pSamplerLinear;

ID3D11RasterizerState* CCWcullMode;
ID3D11RasterizerState* CWcullMode;

//**************************************************************************//
// Now a global instance of each constant buffer.							//
//**************************************************************************//
ID3D11Buffer                       *g_pCBNeverChanges;
ID3D11Buffer                       *g_pCBChangesEveryFrame;


private:
	void toggleCameraType();
	void adjustCamera(UINT key);
	bool testIntersection(Body *b, int mouseX, int mouseY);
	void planetClicked(Planet *planet);
};

