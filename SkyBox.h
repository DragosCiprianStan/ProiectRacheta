#include <Windows.h>
#include <d3dx9.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <d3dx9tex.h>
#include <dinput.h>
#include "Device.h"
#include "Pozition.h"
#pragma once
struct CUSTOMVERTEX {
    FLOAT x, y, z;
    FLOAT tu, tv;
};
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_TEX1 )
class SkyBox
{
private:

    LPDIRECT3DTEXTURE9      skyboxTextures[6];
    D3DXMATRIX transform;
    LPDIRECT3DVERTEXBUFFER9 vertexBuffer = NULL;
    Device device;
    D3DXMATRIX scale;
    //LPDIRECT3DTEXTURE9		g_pTexture_dreapta = NULL;
public:

    VOID setTranslation(FLOAT x,FLOAT y,FLOAT z);
    VOID setDevice(Device& device);
    HRESULT loadTexture(LPCSTR nameTexture, LPCSTR nameTexture2,  LPDIRECT3DTEXTURE9& g_pTexture);
    HRESULT loadSkybox();
    VOID createSkybox();
    VOID setScale();
};

