//**************************************************************************//
// The DirectX9 Thing3D now ported to DirextX10.  This module is based on	//
// Microsoft’s “Simple Sample”, but I have done quite a lot of poggeling.   //
// Apart from using my own Thing3D.											//
//																			//
// This (poggled) code is copyright of Dr Nigel Barlow, lecturer in			//
// computing, University of Plymouth, UK.  email: nigel@soc.plymouth.ac.uk.	//
//																			//
// You may use, modify and distribute this (rather cack-handed in places)	//
// code subject to the following conditions:								//
//																			//
//	1:	You may not use it, or sell it, or use it in any adapted form for	//
//		financial gain, without my written premission.						//
//																			//
//	2:	You must not remove the copyright messages.							//
//																			//
//	3:	You should correct at least 10% of the typig abd spekking errirs.   //
//																			//
// Poggeling ©Nigel Barlow nigel@soc.plymouth.ac.uk.						//
// "Simple Sample" ©Microsoft Corporation. All rights reserved.				//
//**************************************************************************//

#include "DXUT.h"
#include "DXUTgui.h"
#include "DXUTmisc.h"
#include "resource.h"

#include "src/SolarSystem.h"

#define DIRECTX_FEATURE_LEVEL D3D_FEATURE_LEVEL_11_0

#define WINDOW_WIDTH  1024
#define WINDOW_HEIGHT 768
#define START_IN_FULLSCREEN false


//**************************************************************************//
// Global Variables.  There are many global variables here (we aren't OO	//
// yet.  I doubt  Roy Tucker (Comp Sci students will know him) will			//
// approve pf this either.  Sorry, Roy.										//
//**************************************************************************//
CDXUTDialogResourceManager  g_DialogResourceManager; // manager for shared resources of dialogs
CDXUTTextHelper*            g_pTxtHelper = NULL;

DirectXAppController *appController;

DXUT_GAMEPAD gamepad[DXUT_MAX_CONTROLLERS];

//--------------------------------------------------------------------------------------
// Forward declarations 
//--------------------------------------------------------------------------------------


LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void* pUserContext );
void CALLBACK OnKeyboard( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext );
void CALLBACK OnFrameMove( double fTime, float fElapsedTime, void* pUserContext );
bool CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext );
void CALLBACK OnMouse( bool bLeftButtonDown, bool bRightButtonDown, bool bMiddleButtonDown, bool bSideButton1Down, bool bSideButton2Down, int nMouseWheelDelta, int xPos, int yPos, void* pUserContext );


// render stuff
bool CALLBACK IsD3D11DeviceAcceptable( const CD3D11EnumAdapterInfo *AdapterInfo, UINT Output, const CD3D11EnumDeviceInfo *DeviceInfo, DXGI_FORMAT BackBufferFormat, bool bWindowed, void* pUserContext );
HRESULT CALLBACK OnD3D11CreateDevice( ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
HRESULT CALLBACK OnD3D11ResizedSwapChain( ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
void CALLBACK OnD3D11ReleasingSwapChain( void* pUserContext );
void CALLBACK OnD3D11DestroyDevice( void* pUserContext );
void CALLBACK OnD3D11FrameRender( ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext, double fTime, float fElapsedTime, void* pUserContext );

void InitApp();
void RenderText();


//**************************************************************************//
// A Windows program always kicks off in WinMain.							//
// Initializes everything and goes into a message processing				//
// loop.																	//
//																			//
// This version uses DXUT, and is much more complicated than previous		//
// versions you have seen.  This allows you to sync the frame rate to your  //
// monitor's vertical sync event.											//
//**************************************************************************//
int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
    // Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	// set up event DXUT callbacks
    DXUTSetCallbackMsgProc( MsgProc );
    DXUTSetCallbackKeyboard( OnKeyboard );
    DXUTSetCallbackFrameMove( OnFrameMove );
    DXUTSetCallbackDeviceChanging( ModifyDeviceSettings );
	DXUTSetCallbackMouse(OnMouse);

	// set up DirectX11 DXUT callbacks
    DXUTSetCallbackD3D11DeviceAcceptable( IsD3D11DeviceAcceptable );
    DXUTSetCallbackD3D11DeviceCreated( OnD3D11CreateDevice );
    DXUTSetCallbackD3D11SwapChainResized( OnD3D11ResizedSwapChain );
    DXUTSetCallbackD3D11SwapChainReleasing( OnD3D11ReleasingSwapChain );
    DXUTSetCallbackD3D11DeviceDestroyed( OnD3D11DestroyDevice );
    DXUTSetCallbackD3D11FrameRender( OnD3D11FrameRender );

    DXUTInit( true, true, NULL, true );
    DXUTSetCursorSettings( true, true );
    DXUTCreateWindow( L"Adrian's Solar System" );

	appController = new SolarSystem(WINDOW_WIDTH, WINDOW_HEIGHT);


    // Only require 10-level hardware, change to D3D_FEATURE_LEVEL_11_0 to require 11-class hardware
	DXUTCreateDevice( DIRECTX_FEATURE_LEVEL, !START_IN_FULLSCREEN, WINDOW_WIDTH, WINDOW_HEIGHT );

    DXUTMainLoop(); // Enter into the DXUT render loop

    return DXUTGetExitCode();
}



//--------------------------------------------------------------------------------------
// Render the help and statistics text. This function uses the ID3DXFont interface for 
// efficient text rendering.
//--------------------------------------------------------------------------------------
void RenderText()
{
    g_pTxtHelper->Begin();
    g_pTxtHelper->SetInsertionPos( 5, 5 );
    g_pTxtHelper->SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f ) );
    g_pTxtHelper->DrawTextLine( DXUTGetFrameStats( DXUTIsVsyncEnabled() ) );
    g_pTxtHelper->DrawTextLine( DXUTGetDeviceStats() );

	// let app controller print anything it needs to
	appController->renderText(g_pTxtHelper);

	// end this
    g_pTxtHelper->End();
}


//--------------------------------------------------------------------------------------
// Reject any D3D11 devices that aren't acceptable by returning false
//--------------------------------------------------------------------------------------
bool CALLBACK IsD3D11DeviceAcceptable( const CD3D11EnumAdapterInfo *AdapterInfo, UINT Output, const CD3D11EnumDeviceInfo *DeviceInfo, DXGI_FORMAT BackBufferFormat, bool bWindowed, void* pUserContext )
{
    return true;
}


//--------------------------------------------------------------------------------------
// Create any D3D11 resources that aren't dependant on the back buffer
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnD3D11CreateDevice( ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
    HRESULT hr;
    ID3D11DeviceContext* pd3dImmediateContext = DXUTGetD3D11DeviceContext();
    V_RETURN( g_DialogResourceManager.OnD3D11CreateDevice( pd3dDevice, pd3dImmediateContext ) );
    g_pTxtHelper = new CDXUTTextHelper( pd3dDevice, pd3dImmediateContext, &g_DialogResourceManager, 15 );

	appController->deviceCreated(pd3dDevice, pd3dImmediateContext);

	DXUTEnableXInput(true);
    return S_OK;
}


//--------------------------------------------------------------------------------------
// Create any D3D11 resources that depend on the back buffer
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnD3D11ResizedSwapChain( ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
    HRESULT hr;

    V_RETURN( g_DialogResourceManager.OnD3D11ResizedSwapChain( pd3dDevice, pBackBufferSurfaceDesc ) );
	float g_width  = pBackBufferSurfaceDesc->Width;
	float g_height = pBackBufferSurfaceDesc->Height;

	appController->viewResize(g_width, g_height);

    return S_OK;
}


//--------------------------------------------------------------------------------------
// Render the scene using the D3D11 device
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D11FrameRender( ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext, double fTime, float fElapsedTime, void* pUserContext )
{
	float ClearColor[4] = { 0.4, 0.6, 1.0, 1.0 };
    ID3D11RenderTargetView* pRTV = DXUTGetD3D11RenderTargetView();
    pd3dImmediateContext->ClearRenderTargetView( pRTV, ClearColor );

    // Clear the depth stencil
    ID3D11DepthStencilView* pDSV = DXUTGetD3D11DepthStencilView();
    pd3dImmediateContext->ClearDepthStencilView( pDSV, D3D11_CLEAR_DEPTH, 1.0, 0 );

	appController->render(pd3dDevice, pd3dImmediateContext);

	//************************************************//
	// And finally, render the text					  //
	//************************************************//
    DXUT_BeginPerfEvent( DXUT_PERFEVENTCOLOR, L"HUD / Stats" );
    RenderText();
    DXUT_EndPerfEvent();

    static DWORD dwTimefirst = GetTickCount();
    if ( GetTickCount() - dwTimefirst > 5000 )
    {    
        OutputDebugString( DXUTGetFrameStats( DXUTIsVsyncEnabled() ) );
        OutputDebugString( L"\n" );
        dwTimefirst = GetTickCount();
    }
}



//--------------------------------------------------------------------------------------
// Release D3D11 resources created in OnD3D11ResizedSwapChain 
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D11ReleasingSwapChain( void* pUserContext )
{
    g_DialogResourceManager.OnD3D11ReleasingSwapChain();
}


//--------------------------------------------------------------------------------------
// Release D3D11 resources created in OnD3D11CreateDevice 
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D11DestroyDevice( void* pUserContext )
{
    g_DialogResourceManager.OnD3D11DestroyDevice();
    DXUTGetGlobalResourceCache().OnDestroyDevice();
    SAFE_DELETE( g_pTxtHelper );

	// DESTORY 
	appController->deviceDestroyed();
}


//--------------------------------------------------------------------------------------
// Called right before creating a D3D11 device, allowing the app to modify the device settings as needed
//--------------------------------------------------------------------------------------
bool CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext )
{

    // For the first device created if its a REF device, optionally display a warning dialog box
    static bool s_bFirstTime = true;
    if( s_bFirstTime )
    {
        s_bFirstTime = false;
        if( ( DXUT_D3D9_DEVICE == pDeviceSettings->ver && pDeviceSettings->d3d9.DeviceType == D3DDEVTYPE_REF ) ||
            ( DXUT_D3D11_DEVICE == pDeviceSettings->ver &&
            pDeviceSettings->d3d11.DriverType == D3D_DRIVER_TYPE_REFERENCE ) )
        {
            DXUTDisplaySwitchingToREFWarning( pDeviceSettings->ver );
        }

    }

    return true;
}


//**************************************************************************//
// Handle updates to the scene.  This is called regardless of which D3D		//
// API is used (we are only using Dx11).									//
//**************************************************************************//
void CALLBACK OnFrameMove( double fTime, float fElapsedTime, void* pUserContext )
{
	appController->update(fElapsedTime, fTime);

	for( DWORD iPort=0; iPort<DXUT_MAX_CONTROLLERS; iPort++ ) {
		DXUTGetGamepadState( iPort, &gamepad[iPort] );
	}

	for( DWORD iPort=0; iPort<DXUT_MAX_CONTROLLERS; iPort++ ) {
		if ((&gamepad[iPort])->bConnected) {
			appController->onGamePadEvent(&gamepad[iPort]);
		}
	}

}





//--------------------------------------------------------------------------------------
// Handle messages to the application
//--------------------------------------------------------------------------------------
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void* pUserContext )
{
    // Pass messages to dialog resource manager calls so GUI state is updated correctly
    *pbNoFurtherProcessing = g_DialogResourceManager.MsgProc( hWnd, uMsg, wParam, lParam );
    if( *pbNoFurtherProcessing )
        return 0;
 
    return 0;
}


// DXUTToggleFullScreen();


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// EVENT CALLBACKS THAT PASS TO MY ONES!!


// the button states - to compare when a mouse event happends to know what changed
bool leftButtonDown   = false;
bool rightButtonDown  = false;
bool middleButtonDown = false;
bool side1ButtonDown  = false;
bool side2ButtonDown  = false;

void CALLBACK OnMouse( bool bLeftButtonDown, bool bRightButtonDown, bool bMiddleButtonDown, bool bSideButton1Down, bool bSideButton2Down, int nMouseWheelDelta, int xPos, int yPos, void* pUserContext )
{
	// mouse events here

	if (nMouseWheelDelta) {
		float fDelta = (float)nMouseWheelDelta * (XM_PI / 180.0);
		appController->onMouseWheelScrollEvent(fDelta, xPos, yPos);
		return;
	}

	bool isPressed = false;
	int affectedButton = 0;
	if (bLeftButtonDown != leftButtonDown) {
		affectedButton = DirectXAppController::LeftButton;
		isPressed = bLeftButtonDown;
	} else if (bRightButtonDown != rightButtonDown) {
		affectedButton = DirectXAppController::RightButton;
		isPressed = bRightButtonDown;
	} else if (bMiddleButtonDown != middleButtonDown) {
		affectedButton = DirectXAppController::MiddleButton;
		isPressed = bMiddleButtonDown;
	} else if (bSideButton1Down != side1ButtonDown) {
		affectedButton = DirectXAppController::SideButton1;
		isPressed = bSideButton1Down;
	} else if (bSideButton2Down != side2ButtonDown) {
		affectedButton = DirectXAppController::SideButton2;
		isPressed = bSideButton2Down;
	}

	int allButtonStates = 0;
	if (bLeftButtonDown)   allButtonStates += DirectXAppController::LeftButton;
	if (bRightButtonDown)  allButtonStates += DirectXAppController::RightButton;
	if (bMiddleButtonDown) allButtonStates += DirectXAppController::MiddleButton;
	if (bSideButton1Down)  allButtonStates += DirectXAppController::SideButton1;
	if (bSideButton2Down)  allButtonStates += DirectXAppController::SideButton2;


	leftButtonDown   = bLeftButtonDown;
	rightButtonDown  = bRightButtonDown;
	middleButtonDown = bMiddleButtonDown;
	side1ButtonDown  = bSideButton1Down;
	side2ButtonDown  = bSideButton2Down;


	appController->onMouseButtonEvent(affectedButton, isPressed, allButtonStates, xPos, yPos);
}



bool ctrlDown = false;
bool shiftDown = false;

//**************************************************************************//
// Handle key presses.														//
//**************************************************************************//
void CALLBACK OnKeyboard( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext )
{
	if (nChar == VK_SHIFT) {
		ctrlDown = bKeyDown;
		return;
	}

	if (nChar == VK_CONTROL) {
		shiftDown = bKeyDown;
		return;
	}
    
	if (nChar == VK_ESCAPE) {
		exit(0);
	}


	appController->onKeyboardEvent((int)nChar, bKeyDown, shiftDown, ctrlDown, bAltDown);
}
