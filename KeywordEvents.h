#include <d3d9.h>
#include <d3dx9.h>
#include <d3dx9tex.h>
#include <dinput.h>

#pragma once
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")
class UserEvents
{
public:
	BYTE					g_Keystate[256];				
	LPDIRECTINPUTDEVICE8	g_pDinKeyboard;
	LPDIRECTINPUT8			g_pDin;
	LPDIRECTINPUTDEVICE8	g_pDinmouse;					
	DIMOUSESTATE			g_pMousestate;

	VOID unAcquire();
	VOID detectInput();
	HRESULT createInputDevice(HINSTANCE hInstance, HWND hWnd);
	bool exit();
};

