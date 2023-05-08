#include <Windows.h>
#include <d3dx9.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <d3dx9tex.h>
#include <dinput.h>
#include "Device.h"
#include "Pozition.h"
class Rocket
{
private:
	LPDIRECT3DTEXTURE9  texture = NULL;
	LPD3DXBUFFER pD3DXMtrlBuffer = NULL;
	LPD3DXMESH  Mesh = NULL;
	DWORD NumMaterials = 0L;
	D3DMATERIAL9* MeshMaterials = NULL;
	LPDIRECT3DTEXTURE9* MeshTextures = NULL;
	D3DXMATRIX transform;
	D3DXMATRIX rotation;
	D3DXMATRIX scale;
	Device device;
public:
	VOID setDevice(Device& device);
	HRESULT setTexture();
	HRESULT setMesh();
	VOID loadMesh();
	VOID createRocket(Pozition poz);
	VOID cleanUpTexture();
	VOID pozition(int x, int y);
	VOID setTranslation(Pozition translation);
	VOID setRotation(Pozition rotation);
	VOID setScale();
	VOID setRotationY(FLOAT y);
	VOID setLight(BOOL on_off, Pozition meshPosition);

};

