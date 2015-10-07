#include "SolarSystem.h"

#include "MeshGroup.h"
#include "ObjLoader.h"
#include "ShaderManager.h"
#include "String_Additions.h"

#define PROJECTION_NEAR_CLIPPING_DISTANCE 0.01
#define PROJECTION_FAR_CLIPPING_DISTANCE  (((EDGE_OF_SOLAR_SYSTEM * 1.5) * SCALE_SIZES_AMMOUNT) * 2.0)

#define ROTATE_ADJUST_MAGNIFIER   0.05
#define DISTANCE_ADJUST_MAGNIFIER (1000.0 * SCALE_SIZES_AMMOUNT)

//---------------------------------------------------------------

#define NUMBER_OF_SPEEDS 8
static const float timeMultiplierFactors[NUMBER_OF_SPEEDS] = { 0.0, 0.2, 1.0, 2.0, 5.0, 10.0, 50.0, 250.0 };
int currentSpeedFactor = 2;
float timeMultiplyFactor = timeMultiplierFactors[currentSpeedFactor];

void increaseSpeed()
{
	if (currentSpeedFactor < NUMBER_OF_SPEEDS-1) {
		currentSpeedFactor++;
		timeMultiplyFactor = timeMultiplierFactors[currentSpeedFactor];
	}
}

void decreaseSpeed()
{
	if (currentSpeedFactor > 0) {
		currentSpeedFactor--;
		timeMultiplyFactor = timeMultiplierFactors[currentSpeedFactor];
	}
}



//--------------------------------------------------------------------------------------------------------------//

SolarSystem::SolarSystem(float width, float height)
{
	_textureManager = NULL;
	_camera = new Camera();
	XMVECTOR Eye = XMVectorSet( 0.0, 0.0, -15.0f, 0.0f );
    XMVECTOR At = XMVectorSet( 0.0f, 0.0f, 0.0f, 0.0f );
    XMVECTOR Up = XMVectorSet( 0.0f, -1.0f, 0.0f, 0.0f );
	_camera->setPosition(Eye);
	_camera->setFocusPosition(At);
	_camera->setUpDirection(Up);
	_camera->setViewSize(width, height);
	_camera->setFOVAngle(XM_PIDIV4);
	_camera->setNearZClipping(PROJECTION_NEAR_CLIPPING_DISTANCE);
	_camera->setFarZClipping(PROJECTION_FAR_CLIPPING_DISTANCE);

	currentDrawingMode = DrawingModeTriangles;

	vertexShader = new StandardVertexShader();
	HRESULT hr = vertexShader->compileShader();
	//if (FAILED(hr)) return hr;
}

SolarSystem::~SolarSystem()
{
	SAFE_DELETE(_camera);
}


void SolarSystem::update(float dt, float wt)
{
	_camera->update(dt, wt);


	// do behavious stuff here!!
	for (Planet *planet : planets) {
		planet->update(dt * timeMultiplyFactor, wt);
	}
}

void SolarSystem::render(ID3D11Device* device, ID3D11DeviceContext* immediateContext)
{
	//-------------------------------------------------------------------------------------------------------//
	
	XMMATRIX matView = _camera->viewMatrix();
	XMMATRIX g_MatProjection = _camera->projectionMatrix();


	immediateContext->PSSetConstantBuffers( 1, 1, &g_pCBNeverChanges );		//Note this one belongs to the pixel shader.
	immediateContext->PSSetConstantBuffers( 0, 1, &g_pCBChangesEveryFrame );
	immediateContext->PSSetSamplers( 0, 1, &g_pSamplerLinear );

	vertexShader->setVertexShaderForDeviceContext(immediateContext);
	//pd3dImmediateContext->VSSetConstantBuffers( 0, 1, &g_pCBChangeOnResize );	// Paremeter 1 relates to pisition in 
	immediateContext->VSSetConstantBuffers( 0, 1, &g_pCBChangesEveryFrame );	// constant buffers.

	

	//pd3dImmediateContext->PSSetShader( g_pPixelShader, NULL, 0 );
	
	immediateContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );


	XMMATRIX worldMatrixSky = skyBox->worldMatrix();
	XMMATRIX matWVPSky = worldMatrixSky * matView * g_MatProjection;
	CBChangesEveryFrame cb;
	cb.matWorld                = XMMatrixTranspose( worldMatrixSky );
	cb.matWorldViewProjection  = XMMatrixTranspose( matWVPSky);
	//cb.light = light;
	immediateContext->UpdateSubresource( g_pCBChangesEveryFrame, 0, NULL, &cb, 0, 0 );

	immediateContext->RSSetState(CCWcullMode);
	skyBox->render(device, immediateContext);


	immediateContext->IASetPrimitiveTopology( (D3D11_PRIMITIVE_TOPOLOGY)currentDrawingMode );


	immediateContext->RSSetState(CWcullMode);

	// render the planets
	for (Planet *planet : planets) {
		XMMATRIX worldMatrix = planet->worldMatrix();
		XMMATRIX matWVP = worldMatrix * matView * g_MatProjection;
		CBChangesEveryFrame cb;
		cb.matWorld                = XMMatrixTranspose( worldMatrix );
		cb.matWorldViewProjection  = XMMatrixTranspose( matWVP);
		immediateContext->UpdateSubresource( g_pCBChangesEveryFrame, 0, NULL, &cb, 0, 0 );



		CBNeverChanges cbNeverChanges;
		cbNeverChanges.materialColour = XMFLOAT4(1, 1, 1, 1);		//Alpha does nothing.
		XMVECTOR pos = planet->position() * -1;
		XMVECTOR pos2 = XMVector3Normalize(pos);
		cbNeverChanges.vecLight       = pos2;
		immediateContext->UpdateSubresource( g_pCBNeverChanges, 
												0, NULL, 
												&cbNeverChanges, 
												0, 0 );


		planet->render(device, immediateContext);
	}
}

void SolarSystem::deviceCreated(ID3D11Device* device, ID3D11DeviceContext* immediateContext)
{
	HRESULT hr = S_OK;

	// create the rastersizer things, one clockwise and other anti
	D3D11_RASTERIZER_DESC cmdesc;
	ZeroMemory(&cmdesc, sizeof(D3D11_RASTERIZER_DESC));
	cmdesc.FillMode = D3D11_FILL_SOLID;
	cmdesc.CullMode = D3D11_CULL_BACK;
    
	cmdesc.FrontCounterClockwise = true;
	hr = device->CreateRasterizerState(&cmdesc, &CCWcullMode);

	cmdesc.FrontCounterClockwise = false;
	hr = device->CreateRasterizerState(&cmdesc, &CWcullMode);


	
	// create the vertex sahder
	hr = vertexShader->createVertexShader(device);


	
	// create the pixel shader manager
	ShaderManager shaderManager = ShaderManager::sharedShaderManager();

	// create the pixel shaders
	g_pPixelShader		= shaderManager.pixelShaderForTextureAndLighting(device);
	g_pPixelShaderSun   = shaderManager.pixelShaderForTexture(device);
	g_pPixelShaderEarth = shaderManager.pixelShaderForTextureAndLightingAndDark(device);
	g_pPixelShaderMoon  = shaderManager.pixelShaderForTextureAndLightingAndBumpmap(device);
	


	Sphere::initialiseSphereMesh(device);
	PhobosMesh::initialisePhobosMesh(device);


	TextureManager *texMngr = new TextureManager(device);
	_textureManager = texMngr;

	sun = new Sun();
	sun->material = Sun::createSunMaterial(texMngr);
	sun->material->pixelShader = g_pPixelShaderSun;
	

	mercury = new Mercury(sun);
	mercury->material = Mercury::createMercuryMaterial(texMngr);
	mercury->material->pixelShader = g_pPixelShader;
	planets.push_back(mercury);


	venus = new Venus(sun);
	venus->material = Venus::createVenusMaterial(texMngr);
	venus->material->pixelShader = g_pPixelShader;
	planets.push_back(venus);

	earth = new Earth(sun);
	earth->material = Earth::createEarthMaterial(texMngr);
	earth->material->pixelShader = g_pPixelShaderEarth;
	planets.push_back(earth);

	mars = new Mars(sun);
	mars->material = Mars::createMarsMaterial(texMngr);
	mars->material->pixelShader = g_pPixelShader;
	planets.push_back(mars);

	jupiter = new Jupiter(sun);
	jupiter->material = Jupiter::createJupiterMaterial(texMngr);
	jupiter->material->pixelShader = g_pPixelShader;
	planets.push_back(jupiter);

	saturn = new Saturn(sun);
	saturn->material = Saturn::createSaturnMaterial(texMngr);
	saturn->material->pixelShader = g_pPixelShader;
	planets.push_back(saturn);

	uranus = new Uranus(sun);
	uranus->material = Uranus::createUranusMaterial(texMngr);
	uranus->material->pixelShader = g_pPixelShader;
	planets.push_back(uranus);

	neptune = new Neptune(sun);
	neptune->material = Neptune::createNeptuneMaterial(texMngr);
	neptune->material->pixelShader = g_pPixelShader;
	planets.push_back(neptune);


	

	moon = new Moon(earth);
	moon->material = Moon::createMoonMaterial(texMngr);
	moon->material->pixelShader = g_pPixelShaderMoon;
	planets.push_back(moon);
	

	phobos = new Phobos(mars);
	phobos->mesh = PhobosMesh::phobosMesh();
	phobos->material = Phobos::createPhobosMaterial(texMngr);
	phobos->material->pixelShader = g_pPixelShader;
	planets.push_back(phobos);


	planets.push_back(sun);
	

	skyBox = new SkyBox();
	skyBox->material = SkyBox::createSkyBoxMaterial(texMngr);
	skyBox->material->pixelShader = g_pPixelShaderSun;


	planetClicked(earth);

	//ObjLoader *loader = new ObjLoader("Media\\Cup\\Cup.obj");

	//MeshGroup *mesh = loader->meshGroup;
	//mesh->createDeviceStuffs(pd3dDevice);
	//meshes.push_back(mesh);

	//delete loader;


	//loader = new ObjLoader("Media\\sphere\\sphere.obj");

	//mesh = loader->meshGroup;
	//mesh->createDeviceStuffs(g_pd3dDevice);
	//meshes.push_back(mesh);

	//delete loader;

 	//**************************************************************************//
	// Create the 3 constant buffers.											//
	//**************************************************************************//

	D3D11_BUFFER_DESC bd;
    ZeroMemory( &bd, sizeof(bd) );

    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(CBNeverChanges);
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = 0;
    hr = device->CreateBuffer( &bd, NULL, &g_pCBNeverChanges );
    //if( FAILED( hr ) )
    //    return hr;
    
    //bd.ByteWidth = sizeof(CBChangeOnResize);
    //hr = pd3dDevice->CreateBuffer( &bd, NULL, &g_pCBChangeOnResize );
    //if( FAILED( hr ) )
    //    return hr;
    
    bd.ByteWidth = sizeof(CBChangesEveryFrame);
    hr = device->CreateBuffer( &bd, NULL, &g_pCBChangesEveryFrame );
    //if( FAILED( hr ) )
    //    return hr;



   	//**************************************************************************//
	// Create the texture								//
	//**************************************************************************//
	//ShaderManager manager = ShaderManager::sharedShaderManager();
	//unsigned int index;
	//hr = manager.createTexture(&index, L"Media\\Cup\\cup.jpg", pd3dDevice, pd3dImmediateContext);
    //if( FAILED( hr ) )
    //    return hr;

    // Create the sample state
    D3D11_SAMPLER_DESC sampDesc;
    ZeroMemory( &sampDesc, sizeof(sampDesc) );
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
    hr = device->CreateSamplerState( &sampDesc, &g_pSamplerLinear );
    //if( FAILED( hr ) )
    //    return hr;

 

    //**************************************************************************//
	// Update the constant buffer for stuff (the light vector and material		//
	// colour in this case) that never change.  This is faster; don't update	//
	// stuff if you don't have to.												//
	//**************************************************************************//
	CBNeverChanges cbNeverChanges;
	cbNeverChanges.materialColour = XMFLOAT4(1, 1, 1, 1);		//Alpha does nothing.
	cbNeverChanges.vecLight       = XMVectorSet(1, 1, -2, 0);	//4th value unused.
	cbNeverChanges.vecLight       = XMVector3Normalize(cbNeverChanges.vecLight);
    immediateContext->UpdateSubresource( g_pCBNeverChanges, 
											0, NULL, 
											&cbNeverChanges, 
											0, 0 );
}

void SolarSystem::deviceDestroyed()
{
	SAFE_RELEASE (g_pSamplerLinear) ; 


	planets.empty();

	delete moon;
	delete earth;
	delete sun;
	delete skyBox;
	delete mercury;
	delete venus;
	delete mars;
	delete jupiter;
	delete saturn;
	delete uranus;
	delete neptune;
	//delete phobos;

	vertexShader->destroyVertexShader();

	SAFE_RELEASE(g_pPixelShader);
	SAFE_RELEASE(g_pPixelShaderSun);
	SAFE_RELEASE(g_pPixelShaderEarth);
	SAFE_RELEASE(g_pPixelShaderMoon);

	// release the winding orders
	SAFE_RELEASE(CCWcullMode);
	SAFE_RELEASE(CWcullMode);


	// destroy the sphere mesh
	Sphere::destroySphereMesh();
	PhobosMesh::destroyPhobosMesh();

	
	if (_textureManager) delete _textureManager;

	SAFE_RELEASE(g_pCBChangesEveryFrame);
	SAFE_RELEASE(g_pCBNeverChanges);
}

void SolarSystem::viewResize(int width, int height)
{
	_camera->setViewSize(width, height);
}


void SolarSystem::renderText(CDXUTTextHelper *textHelper)
{
	// Print out the sim speed
	WCHAR message[80];
	swprintf(message, L"Speed of simulation : %2.1f", timeMultiplyFactor);
	textHelper->DrawTextLine(message);

	// PRINT out the current planet
	swprintf(message, L"Current Planet: %s", currentPlanet->name());
	textHelper->DrawTextLine(message);

	// PRINT out the current camera mode
	swprintf(message, L"Camera Mode: %s", _camera->cameraTypeString());
	textHelper->DrawTextLine(message);
}


void SolarSystem::onGamePadEvent(DXUT_GAMEPAD *pad)
{
	if (_camera->cameraType() == Camera::CameraTypeSteady) {
		_camera->rotateAroundPosition(0.0, pad->fThumbRX * -0.01,  0.0);
		_camera->moveRelativeToFocus(pad->fThumbLX * 0.1, 0.0, pad->fThumbLY * 0.1);
	}


	if (_camera->cameraType() == Camera::CameraTypeFollowBody) {
		_camera->appendFollowRotation(0.0, pad->fThumbRX * -0.01, 0.0);
		_camera->appendFollowDistance(pad->fThumbLY * -0.1);
	}


	if (_camera->cameraType() == Camera::CameraTypeWatchBody) {
		_camera->moveRelativeToFocus(pad->fThumbLX * 0.1, 0.0, pad->fThumbLY * 0.1);
	}

	if (pad->wPressedButtons & XINPUT_GAMEPAD_START) {
		toggleCameraType();
	}


}

void SolarSystem::onKeyboardEvent(int key, bool keyDown, bool shiftDown, bool ctrlDown, bool altDown)
{
	if (!keyDown) return;

	switch( key ) {

		//******************************************************************//
		// Nigel code to rotate and move the tiger.							//
		//******************************************************************//
		case VK_LEFT:
		case VK_RIGHT:
		case VK_UP:
		case VK_DOWN: 
			adjustCamera(key);
			break;
		case VK_TAB:
			toggleCameraType();
			break;
		case VK_SPACE:
			currentPlanet->startBodyAudio();
			break;
		case '[':
			break;
		case ']':
			break;
	
		// toggle the different views
		case 'Z': currentDrawingMode = DrawingModePoints;    break;
		case 'X': currentDrawingMode = DrawingModeLines;     break;
		case 'C': currentDrawingMode = DrawingModeTriangles; break;


		// affect the time pasing
		case 187:
			increaseSpeed();
			break;
		case 189:
			decreaseSpeed();  
			break;

		// do stuff with the planets
		case '1':	// mercury
		case '2':	// venus
		case '3':	// earth
		case '4':	// mars
		case '5':	// jupiter
		case '6':	// saturn
		case '7':	// uranus
		case '8':	// neptune
		case '9':	// moon
			planetClicked(planets[key - '1']);
			break;
		case '0':	// sun
			planetClicked(sun);
			break;
    }
}

void SolarSystem::onMouseWheelScrollEvent(float delta, int x, int y)
{
	_camera->appendFollowDistance(delta * DISTANCE_ADJUST_MAGNIFIER);
}

void SolarSystem::onMouseButtonEvent(int button, bool isPressed, int buttons, int x, int y)
{
	// order then by depth (closeest first) then hit test in that order

	// check for planet being clicked by mouse
	for (Planet *planet : planets) {
		bool hit = testIntersection(planet, x, y);
		if (hit) {
			planetClicked(planet);
			return;
		}
	}
}


void animEnd(Planet *planet, bool finished)
{
	if (finished) {
		//planet->startBodyAudio();
	}
}


void SolarSystem::planetClicked(Planet *planet)
{
	currentPlanet = planet;
	_camera->animateToPlanetThenFollow(planet, 15.0, timeMultiplyFactor, 5.0, planet->scale() * 6, XM_PI, animEnd);
}

bool RaySphereIntersect(XMVECTOR rayOrigin, XMVECTOR rayDirection, float radius);

bool SolarSystem::testIntersection(Body *b, int mouseX, int mouseY)
{
	float pointX, pointY;
	XMMATRIX projectionMatrix, _viewMatrix, inverseViewMatrix, worldMatrix, translateMatrix, inverseWorldMatrix;
	XMVECTOR direction, origin, rayOrigin, rayDirection;
	bool intersect, result;


	// Move the mouse cursor coordinates into the -1 to +1 range.
	pointX = ((2.0f * (float)mouseX) / (float)_camera->viewWidth()) - 1.0f;
	pointY = (((2.0f * (float)mouseY) / (float)_camera->viewHeight()) - 1.0f) * -1.0f;
		
	XMMATRIX g_MatProjection = _camera->projectionMatrix();

	// Adjust the points using the projection matrix to account for the aspect ratio of the viewport.
	projectionMatrix = g_MatProjection;
	pointX = pointX / projectionMatrix._11;
	pointY = pointY / projectionMatrix._22;

	//viewMatrix
	_viewMatrix = _camera->viewMatrix();
	XMVECTOR vDeterminant;
	inverseViewMatrix = XMMatrixInverse(&vDeterminant, _viewMatrix);


	// Calculate the direction of the picking ray in view space.
	float dirX = (pointX * inverseViewMatrix._11) + (pointY * inverseViewMatrix._21) + inverseViewMatrix._31;
	float dirY = (pointX * inverseViewMatrix._12) + (pointY * inverseViewMatrix._22) + inverseViewMatrix._32;
	float dirZ = (pointX * inverseViewMatrix._13) + (pointY * inverseViewMatrix._23) + inverseViewMatrix._33;
	direction = XMVectorSet(dirX, dirY, dirZ, 0.0);

	// Get the origin of the picking ray which is the position of the camera.
	origin = _camera->position();

	// Get the world matrix and translate to the location of the sphere.
	worldMatrix = b->positionMatrix();

	// Now get the inverse of the translated world matrix.
	inverseWorldMatrix = XMMatrixInverse(&vDeterminant, worldMatrix);

	// Now transform the ray origin and the ray direction from view space to world space.
	rayOrigin = XMVector3TransformCoord(origin, inverseWorldMatrix);
	rayDirection = XMVector3TransformNormal(direction, inverseWorldMatrix);

	// Normalize the ray direction.
	rayDirection = XMVector3Normalize(rayDirection);

	// Now perform the ray-sphere intersection test.
	intersect = RaySphereIntersect(rayOrigin, rayDirection, b->scale());
	

	return intersect;
}


// This function performs the math of a basic ray-sphere intersection test. 
bool RaySphereIntersect(XMVECTOR rayOrigin, XMVECTOR rayDirection, float radius)
{
	float a, b, c, discriminant;


	float rayDir_X = XMVectorGetByIndex(rayDirection, 0);
	float rayDir_Y = XMVectorGetByIndex(rayDirection, 1);
	float rayDir_Z = XMVectorGetByIndex(rayDirection, 2);

	float rayOgn_X = XMVectorGetByIndex(rayOrigin, 0);
	float rayOgn_Y = XMVectorGetByIndex(rayOrigin, 1);
	float rayOgn_Z = XMVectorGetByIndex(rayOrigin, 2);

	// Calculate the a, b, and c coefficients.
	a = ((rayDir_X * rayDir_X) + (rayDir_Y * rayDir_Y) + (rayDir_Z * rayDir_Z));
	b = ((rayDir_X * rayOgn_X) + (rayDir_Y * rayOgn_Y) + (rayDir_Z * rayOgn_Z)) * 2.0f;
	c = ((rayOgn_X * rayOgn_X) + (rayOgn_Y * rayOgn_Y) + (rayOgn_Z * rayOgn_Z)) - (radius * radius);

	// Find the discriminant.
	discriminant = (b * b) - (4 * a * c);

	// if discriminant is negative the picking ray missed the sphere, otherwise it intersected the sphere.
	if (discriminant < 0.0f)
	{
		return false;
	}

	return true;
}

void SolarSystem::toggleCameraType()
{
	if (_camera->cameraType() == Camera::CameraTypeFollowBody) {
		_camera->watchBody(currentPlanet);
	} else if (_camera->cameraType() == Camera::CameraTypeWatchBody) {
		_camera->setSteadyCamera();
	} else if (_camera->cameraType() == Camera::CameraTypeSteady) {
		_camera->animateToPlanetThenFollow(currentPlanet,15.0,timeMultiplyFactor, 5.0, currentPlanet->scale() * 6);
		//_camera->followBody(currentPlanet, currentPlanet->scale() * 6);
	}
}

void SolarSystem::adjustCamera(UINT key)
{
	if (_camera->cameraType() == Camera::CameraTypeFollowBody) {
		switch( key ) {
			case VK_LEFT:
				_camera->appendFollowRotation(0.0, -ROTATE_ADJUST_MAGNIFIER, 0.0);
				break;
			case VK_RIGHT:
				_camera->appendFollowRotation(0.0, +ROTATE_ADJUST_MAGNIFIER, 0.0);
				break;
			case VK_UP:
				_camera->appendFollowDistance(-DISTANCE_ADJUST_MAGNIFIER);
				break;
			case VK_DOWN: 
				_camera->appendFollowDistance(DISTANCE_ADJUST_MAGNIFIER);
				break;
		}
	}


	if (_camera->cameraType() == Camera::CameraTypeWatchBody) {
		switch( key ) {
			case VK_LEFT:
				//_camera->appendFollowRotation(0.0, -ROTATE_ADJUST_MAGNIFIER, 0.0);
				break;
			case VK_RIGHT:
				//_camera->appendFollowRotation(0.0, +ROTATE_ADJUST_MAGNIFIER, 0.0);
				break;
			case VK_UP:
				_camera->moveRelativeToFocus(0.0, 0.0, DISTANCE_ADJUST_MAGNIFIER);
				break;
			case VK_DOWN: 
				_camera->moveRelativeToFocus(0.0, 0.0, -DISTANCE_ADJUST_MAGNIFIER);
				break;
		}
	}


	if (_camera->cameraType() == Camera::CameraTypeSteady) {
		switch( key ) {
			case VK_LEFT:
				_camera->rotateAroundPosition(0.0, ROTATE_ADJUST_MAGNIFIER,  0.0);
				break;
			case VK_RIGHT:
				_camera->rotateAroundPosition(0.0, -ROTATE_ADJUST_MAGNIFIER, 0.0);
				break;
			case VK_UP:
				_camera->moveRelativeToFocus(0.0, 0.0, DISTANCE_ADJUST_MAGNIFIER);
				break;
			case VK_DOWN: 
				_camera->moveRelativeToFocus(0.0, 0.0, -DISTANCE_ADJUST_MAGNIFIER);
				break;
		}
	}

}
