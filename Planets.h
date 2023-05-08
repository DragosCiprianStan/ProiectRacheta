#include <Windows.h>
#include <d3dx9.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <d3dx9tex.h>
#include <dinput.h>
#include "Device.h"
#include "Pozition.h"
#pragma once
class Planets
{
public:
	LPDIRECT3DTEXTURE9  texture = NULL;
	LPD3DXBUFFER pD3DXMtrlBuffer = NULL;
	LPD3DXMESH  Mesh = NULL;
	DWORD NumMaterials = 0L;
	D3DMATERIAL9* MeshMaterials = NULL;
	LPDIRECT3DTEXTURE9* MeshTextures = NULL;
	D3DXMATRIX transform;
	D3DXMATRIX rotation;
	D3DXMATRIX scale;
	Pozition meshPosition;
	Device device;
	D3DMATERIAL9 mtrl;
	VOID setDevice(Device& device);

	HRESULT setTexture(LPCSTR planetsTexture);
	HRESULT setMesh(LPCSTR planetMesh);
	VOID loadMesh(LPCSTR planetTexture, LPCSTR planetMesh);
	VOID createPlanet(Pozition translation);
	VOID cleanUpTexture();
	VOID pathFile(D3DXMATERIAL* d3dxMaterials, DWORD i);
	VOID setTranslation(Pozition translation);
	VOID setRotation(Pozition rotation);
	VOID setScale();
	VOID setLight(BOOL on_off);

};

