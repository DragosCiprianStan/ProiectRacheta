#include <Windows.h>
#include <d3dx9.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <d3dx9tex.h>
#include <dinput.h>
#include "Device.h"
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
	Device device;
	VOID setDevice(Device& device);
	HRESULT setTexture();
	HRESULT setMesh();
	VOID loadMesh();
	VOID createPlanet();
	VOID cleanUpTexture();
};

