#include <D3DX9Mesh.h>

#pragma once
class SkyBox
{
public:
    SkyBox(LPCSTR textureFile);
    ~SkyBox();
    void Draw();
protected:
    IDirect3DCubeTexture9* texture;
    LPD3DXMESH mesh;
};

