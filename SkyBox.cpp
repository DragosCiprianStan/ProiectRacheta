#include "SkyBox.h"
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_TEX1)
VOID SkyBox::setTranslation(FLOAT x, FLOAT y, FLOAT z)
{
	D3DXMatrixIdentity(&this->transform);
	D3DXMatrixTranslation(&this->transform, x, y,z);
}

VOID SkyBox::setDevice(Device& device)
{
    this->device = device;
}

VOID SkyBox::setScale()
{
	D3DXMatrixIdentity(&this->scale);
	D3DXMatrixScaling(&this->scale, 5, 5, 5);
}

HRESULT SkyBox::loadTexture(LPCSTR nameTexture, LPCSTR nameTexture2, LPDIRECT3DTEXTURE9 &g_pTexture)
{
    if (FAILED(D3DXCreateTextureFromFile(device.direct3Device9, nameTexture, &g_pTexture)))
    {
  
        if (FAILED(D3DXCreateTextureFromFile(device.direct3Device9, nameTexture2, &g_pTexture)))
        {
            MessageBox(NULL, "Could not find sus.png", "Proj.exe", MB_OK);
            return E_FAIL;
        }
    }


}

HRESULT SkyBox::loadSkybox()
{
	loadTexture("2_front.jpg", "E:\\Facultate\\DirectX\\Proiect\\Object\\SkyBoxImage\\2_front.jpg", skyboxTextures[0]);
	loadTexture("1_left.jpg", "E:\\Facultate\\DirectX\\Proiect\\Object\\SkyBoxImage\\1_left.jpg", skyboxTextures[1]);
	loadTexture("5_reight.jpg", "E:\\Facultate\\DirectX\\Proiect\\Object\\SkyBoxImage\\5_reight.jpg", this->skyboxTextures[2]);
	loadTexture("3_top.jpg", "E:\\Facultate\\DirectX\\Proiect\\Object\\SkyBoxImage\\3_top.jpg", this->skyboxTextures[3]);
	loadTexture("4_bottom.jpg", "E:\\Facultate\\DirectX\\Proiect\\Object\\SkyBoxImage\\4_bottom.jpg", this->skyboxTextures[4]);
	loadTexture("6_back.jpg", "E:\\Facultate\\DirectX\\Proiect\\Object\\SkyBoxImage\\6_back.jpg", this->skyboxTextures[5]);
	CUSTOMVERTEX skybox[24] = {


		//fata
		{ 10, -10, -10, 0.0f, 1.0f },
		{ 10, 10, -10, 0.0f, 0.0f },
		{ -10, -10, -10, 1.0f, 1.0f },
		{ -10, 10, -10, 1.0f, 0.0f },

		//stanga
		{ -10, -10, -10, 0.0f, 1.0f },
		{ -10, 10, -10, 0.0f, 0.0f },
		{ -10, -10, 10, 1.0f, 1.0f },
		{ -10, 10, 10, 1.0f, 0.0f },

		//dreapta
		{ 10, -10, 10, 0.0f, 1.0f },
		{ 10, 10, 10, 0.0f, 0.0f },
		{ 10, -10, -10, 1.0f, 1.0f },
		{ 10, 10, -10, 1.0f, 0.0f },

		//sus
		{ -10, 10, 10, 0.0f, 1.0f },
		{ -10, 10, -10, 0.0f, 0.0f },
		{ 10, 10, 10, 1.0f, 1.0f },
		{ 10, 10, -10, 1.0f, 0.0f },

		//jos
		{ -10, -10, -10, 0.0f, 1.0f },
		{ -10, -10, 10, 0.0f, 0.0f },
		{ 10, -10, -10, 1.0f, 1.0f },
		{ 10, -10, 10, 1.0f, 0.0f },

	   { -10, -10, 10, 0.0f, 1.0f },
	   { -10, 10, 10, 0.0f, 0.0f },
	   { 10, -10, 10, 1.0f, 1.0f },
	   { 10, 10, 10, 1.0f, 0.0f },

	};
	if (FAILED(device.direct3Device9->CreateVertexBuffer(24 * sizeof(CUSTOMVERTEX),
		0, D3DFVF_CUSTOMVERTEX,
		D3DPOOL_DEFAULT, &this->vertexBuffer, NULL)))
	{
		MessageBox(NULL, "Nu s-a putut crea obiectul vertex buffer", "InitGeometry", MB_OK);
		return E_FAIL;
	}
	VOID* pVertices;
	if (FAILED(this->vertexBuffer->Lock(0, sizeof(skybox), (void**)&pVertices, 0)))
		return E_FAIL;
	memcpy(pVertices, skybox, 24 * sizeof(CUSTOMVERTEX));
	this->vertexBuffer->Unlock();

}

VOID SkyBox::createSkybox()
{
		setTranslation(0, 0, 0);
		setScale();
		device.direct3Device9->SetStreamSource(0, vertexBuffer, 0, sizeof(CUSTOMVERTEX));
		device.direct3Device9->SetFVF(D3DFVF_CUSTOMVERTEX);
		transform *= scale;
		this->device.direct3Device9->SetTransform(D3DTS_WORLD, &this->transform);
		for (int i = 0; i < 6; i++)
		{
			device.direct3Device9->SetTexture(0, skyboxTextures[i]);
			device.direct3Device9->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * i, 2);
		}
}
