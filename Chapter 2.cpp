#include <Windows.h>
#include <d3dx9.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <d3dx9tex.h>
#include <dinput.h>
#include "Window.h"
#include "Device.h"
#include "Rocket.h"
#include "Planets.h"
#include "KeywordEvents.h"
#include "Pozition.h"
#include "Camera.h"
#include "UserAudio.h"
#include <stdio.h>
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")


Device myDevice;
Rocket myRocket;
Planets earth;
UserAudio userAudio;
Planets sun;
UserEvents userEvents;
Pozition poz;

CXCamera *myCamera;

HRESULT InitD3D(HWND hWnd)
{
	myDevice.createD3DObject(hWnd);
	myDevice.createDevice(hWnd);
	myRocket.setDevice(myDevice);
	earth.setDevice(myDevice);
	sun.setDevice(myDevice);
	return S_OK;
}
void InitiateCamera()
{
	myCamera = new CXCamera(myDevice.direct3Device9);
	D3DXVECTOR3 vEyePt(0.0f, 0.0f, -5.0f);
	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);
	D3DXMATRIXA16 matView;

	myCamera->LookAtPos(&vEyePt, &vLookatPt, &vUpVec);
	myCamera->MoveForward(4);
	myCamera->MoveUp(1.8);
	myCamera->MoveRight(0.1);
	myCamera->RotateDown(0.7);
}
VOID DetectInput();
VOID Render();
VOID Cleanup();
VOID CleanDInput();
HRESULT InitDInput(HINSTANCE hInstance, HWND hWnd)
{
	userEvents.createInputDevice(hInstance, hWnd);
	return S_OK;
}
VOID CleanDInput()
{
	userEvents.unAcquire();
}
VOID DetectInput()
{
	userEvents.detectInput();
}
HRESULT InitGeometry()
{
	poz.rocketPozition.rotY += 1.57079633;
	myRocket.loadMesh();
	earth.loadMesh("E:\\Facultate\\DirectX\\Proiect\\Object\\Planets\\Earth\\earth.jpg", "E:\\Facultate\\DirectX\\Proiect\\Object\\Planets\\Earth\\earth.x");
	sun.loadMesh("E:\\Facultate\\DirectX\\Proiect\\Object\\Planets\\Sun\\Sun.jpg", "E:\\Facultate\\DirectX\\Proiect\\Object\\Planets\\Sun\\Sun.x");
	InitiateCamera();
	
	return S_OK;
}
VOID Cleanup()
{
	myRocket.cleanUpTexture();
	earth.cleanUpTexture();
	myDevice.cleanUpDevice();
	myDevice.cleanUpDeviceObject();
}
void SetupWorldMatrix()
{

	D3DXMATRIXA16 matWorld;
	D3DXMatrixRotationY(&matWorld, timeGetTime() / 1000.0f);
	myDevice.direct3Device9->SetTransform(D3DTS_WORLD, &matWorld);
	

}

void SetupViewMatrix()
{
	
	myCamera->Update();
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
VOID SetupLights() {}
VOID Render()
{
	
	myDevice.direct3Device9->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 255, 255), 1.0f, 0);
	if (SUCCEEDED(myDevice.direct3Device9->BeginScene()))
	{
		//sun.setLight(TRUE);
		poz.earthPozition.z = 10;
		
		myRocket.createRocket(poz);
		myRocket.setLight(FALSE, poz);
		//sun.setLight(FALSE);
		earth.createPlanet(poz);
		//poz.earthPozition.z = -10;
		//sun.createPlanet(poz);
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
		InitDInput(hInst, myWindow.hWnd);
		ShowWindow(myWindow.hWnd, SW_SHOWDEFAULT);
		UpdateWindow(myWindow.hWnd);
		if (SUCCEEDED(InitGeometry()))
		{
			if (FAILED(userAudio.InitDirectShow(myWindow.hWnd)))
				return 0;

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
					DetectInput();
					Render();
					
					if (userEvents.keyword(DIK_ESCAPE)) {
						PostMessage(myWindow.hWnd, WM_DESTROY, 0, 0);
					}
					if (userEvents.keyword(DIK_W)) {
						userAudio.play();
						myRocket.setLight(TRUE, poz);
						poz.rocketPozition.z += 0.1;

						poz.rocketPozition.x += 0.1 * (sin(poz.rocketPozition.rotZ)*(- cos(poz.rocketPozition.rotZ)));

						myCamera->MoveForward(0.1);
					}
					else {
						userAudio.stop();
					}
					//userEvents.keyword2(DIK_W, poz, 0, 0.5);
					if (userEvents.keyword(DIK_S)) {
						poz.rocketPozition.z -= 0.1;
						myCamera->MoveForward(-0.1);
					}
					if (userEvents.keyword(DIK_D)) {
						poz.rocketPozition.rotZ -= 0.1;
						
					}
					
					if (userEvents.keyword(DIK_A)) {
				
						poz.rocketPozition.rotZ += 0.1;
					}
					if (userEvents.keyword(DIK_UP)) {
						myCamera->MoveForward(0.1);
					}
					if (userEvents.keyword(DIK_DOWN)) {
						myCamera->MoveForward(-0.1);
					}
					if (userEvents.keyword(DIK_RIGHT)) {
						myCamera->MoveRight(0.1);
					}
					if (userEvents.keyword(DIK_LEFT)) {
						myCamera->MoveRight(-0.1);
					}
					myCamera->RotateRight((userEvents.g_pMousestate.lX * (D3DX_PI / 180)));
					myCamera->RotateDown(userEvents.g_pMousestate.lY * (D3DX_PI / 180));
					
				}
			}
		}
	}
}