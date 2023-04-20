#include <Windows.h>
#include <d3dx9.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <d3dx9tex.h>
#include <dinput.h>
#pragma once
class Device
{
public:
	LPDIRECT3D9             directD3D = NULL;
	LPDIRECT3DDEVICE9       direct3Device9 = NULL;
	D3DPRESENT_PARAMETERS d3dpp;
	HRESULT createD3DObject(HWND hWnd);
	VOID deviceStructure();
	VOID cleanUpDevice();
	VOID cleanUpDeviceObject();
	VOID SetupLights();
	HRESULT createDevice(HWND hWnd);
};

