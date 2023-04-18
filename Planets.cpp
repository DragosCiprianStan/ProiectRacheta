#include "Planets.h"
HRESULT Planets::setTexture() {
	if (FAILED(D3DXCreateTextureFromFile(this->device.direct3Device9, "RocketTexture.png", &this->texture)))
	{
		if (FAILED(D3DXCreateTextureFromFile(this->device.direct3Device9, "E:\\Facultate\\DirectX\\Proiect\\Object\\Planets\\Earth\\earth.jpg", &this->texture)))
		{
			MessageBox(NULL, "Could not find bannana.", "Textures.exe", MB_OK);
			return E_FAIL;
		}
	}

}
HRESULT Planets::setMesh() {

	if (FAILED(D3DXLoadMeshFromX("earth.x", D3DXMESH_SYSTEMMEM,
		this->device.direct3Device9, NULL,
		&this->pD3DXMtrlBuffer, NULL, &this->NumMaterials,
		&this->Mesh)))
	{
		if (FAILED(D3DXLoadMeshFromX("E:\\Facultate\\DirectX\\Proiect\\Object\\Planets\\Earth\\earth.x", D3DXMESH_SYSTEMMEM,
			this->device.direct3Device9, NULL,
			&this->pD3DXMtrlBuffer, NULL, &this->NumMaterials,
			&this->Mesh)))
		{
			MessageBox(NULL, "Could not find tiger.x", "Meshes.exe", MB_OK);
			return E_FAIL;
		}
	}

}

VOID Planets::loadMesh() {
	this->setTexture();
	this->setMesh();

	D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)this->pD3DXMtrlBuffer->GetBufferPointer();
	this->MeshMaterials = new D3DMATERIAL9[NumMaterials];
	this->MeshTextures = new LPDIRECT3DTEXTURE9[NumMaterials];

	for (DWORD i = 0; i < this->NumMaterials; i++)
	{
		this->MeshMaterials[i] = d3dxMaterials[i].MatD3D;
		this->MeshMaterials[i].Ambient = this->MeshMaterials[i].Diffuse;

		this->MeshTextures[i] = NULL;
		if (d3dxMaterials[i].pTextureFilename != NULL &&
			lstrlen(d3dxMaterials[i].pTextureFilename) > 0)
		{
			if (FAILED(D3DXCreateTextureFromFile(this->device.direct3Device9,
				d3dxMaterials[i].pTextureFilename,
				&this->MeshTextures[i])))
			{

				const TCHAR* strPrefix = TEXT("..\\");
				const int lenPrefix = lstrlen(strPrefix);
				TCHAR strTexture[MAX_PATH];
				lstrcpyn(strTexture, strPrefix, MAX_PATH);
				lstrcpyn(strTexture + lenPrefix, d3dxMaterials[i].pTextureFilename, MAX_PATH - lenPrefix);
				if (FAILED(D3DXCreateTextureFromFile(this->device.direct3Device9,
					strTexture,
					&this->MeshTextures[i])))
				{
					MessageBox(NULL, "Could not find texture map", "Meshes.exe", MB_OK);
				}
			}
		}
	}
}
VOID Planets::createRocket() {

	for (DWORD i = 0; i < this->NumMaterials; i++)
	{
		this->device.direct3Device9->SetMaterial(&this->MeshMaterials[i]);
		this->device.direct3Device9->SetTexture(0, this->texture);
		this->Mesh->DrawSubset(i);
	}
}
VOID Planets::cleanUpTexture() {


	if (this->MeshMaterials != NULL)
		delete[] this->MeshMaterials;

	if (this->MeshTextures)
	{
		for (DWORD i = 0; i < this->NumMaterials; i++)
		{
			if (this->MeshTextures[i])
				this->MeshTextures[i]->Release();
		}
		delete[] this->MeshTextures;
	}

	if (this->texture != NULL)
		this->texture->Release();

}

VOID Planets::setDevice(Device& device) {
	this->device = device;
}