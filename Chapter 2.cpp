#include <Windows.h>
#include <d3dx9.h>
//#include <d3d9.h>
//#include <d3dx9.h>
//#include <d3dx9tex.h>
//#include <dinput.h>
#include "Window.h"
#include "Device.h"
#include "Rocket.h"
#include "Planets.h"
Device myDevice;
Rocket myRocket;
Planets earth;
HRESULT InitD3D(HWND hWnd)
{
	myDevice.createD3DObject(hWnd);
	myDevice.createDevice(hWnd);
	myRocket.setDevice(myDevice);
	earth.setDevice(myDevice);
	return S_OK;

}
VOID Cleanup();

//HRESULT InitDInput(HINSTANCE hInstance, HWND hWnd)
//{
//	
//}

HRESULT InitGeometry()
{
	myRocket.loadMesh();
	earth.loadMesh();
	return S_OK;
}

VOID Cleanup()
{

	myRocket.cleanUpTexture();
	earth.cleanUpTexture();
	if (myDevice.direct3Device9 != NULL)
		myDevice.direct3Device9->Release();

	if (myDevice.directD3D != NULL)
		myDevice.directD3D->Release();
}

void SetupWorldMatrix()
{
	D3DXMATRIX g_Transform, m2;
	D3DXMatrixIdentity(&g_Transform);
	D3DXMatrixIdentity(&m2);
	D3DXMatrixScaling(&m2, 1.3, 1.3, 1.3);
	D3DXMatrixRotationY(&g_Transform, timeGetTime() / 1000.0);
	g_Transform *= m2;
	myDevice.direct3Device9->SetTransform(D3DTS_WORLD, &g_Transform);
}

void SetupWorldMatrixCerc()
{
	D3DXMATRIX g_Transform, m2;
	D3DXMatrixIdentity(&g_Transform);
	D3DXMatrixIdentity(&m2);
	D3DXMatrixTranslation(&m2, 1.3, 1.3, 1.3);
	D3DXMatrixRotationY(&g_Transform, timeGetTime() / 1000.0);
	g_Transform *= m2;
	myDevice.direct3Device9->SetTransform(D3DTS_WORLD, &g_Transform);
}
void SetupViewMatrix()
{
	D3DXVECTOR3 vEyePt(0.0f, 3.0f, -5.0f);
	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);
	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);
	myDevice.direct3Device9->SetTransform(D3DTS_VIEW, &matView);
}
void SetupProjectionMatrix()
{
	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4, 1.0f, 1.0f, 100.0f);
	myDevice.direct3Device9->SetTransform(D3DTS_PROJECTION, &matProj);
}
VOID SetupMatrices()
{
	SetupWorldMatrix();

	SetupViewMatrix();

	SetupProjectionMatrix();
}
VOID SetupLights()
{
	
}
VOID Render()
{
	myDevice.direct3Device9->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,D3DCOLOR_XRGB(0, 255, 255), 1.0f, 0);
	if (SUCCEEDED(myDevice.direct3Device9->BeginScene()))
	{
		myRocket.createRocket();
		earth.createRocket();
		SetupMatrices();
		myDevice.direct3Device9->EndScene();
	}
	myDevice.direct3Device9->Present(NULL, NULL, NULL, NULL);
}
LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		Cleanup();
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, INT)
{
	Window myWindow;
	myWindow.windowMaker(MsgProc);
	if (SUCCEEDED(InitD3D(myWindow.hWnd)))
	{

		ShowWindow(myWindow.hWnd, SW_SHOWDEFAULT);
		UpdateWindow(myWindow.hWnd);
		if (SUCCEEDED(InitGeometry()))
		{

			ShowWindow(myWindow.hWnd, SW_SHOWDEFAULT);
			UpdateWindow(myWindow.hWnd);
			MSG msg;
			ZeroMemory(&msg, sizeof(msg));
			while (msg.message != WM_QUIT)
			{
				if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				else {
					Render();
				
				}
			}
		}
	}
}