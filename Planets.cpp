#include "Planets.h"
//"E:\\Facultate\\DirectX\\Proiect\\Object\\Planets\\Earth\\earth.jpg"
//"E:\\Facultate\\DirectX\\Proiect\\Object\\Planets\\Earth\\earth.x"
HRESULT Planets::setTexture(LPCSTR planetTexture) {
	if (FAILED(D3DXCreateTextureFromFile(this->device.direct3Device9, "earth.jpg", &this->texture)))
	{
		if (FAILED(D3DXCreateTextureFromFile(this->device.direct3Device9, planetTexture, &this->texture)))
		{
			MessageBox(NULL, "Could not find bannana.", "Textures.exe", MB_OK);
			return E_FAIL;
		}
	}

}
HRESULT Planets::setMesh(LPCSTR planetMesh) {

	if (FAILED(D3DXLoadMeshFromX("earth.x", D3DXMESH_SYSTEMMEM, this->device.direct3Device9, NULL, &this->pD3DXMtrlBuffer,
		NULL, &this->NumMaterials,
		&this->Mesh)))
	{
		if (FAILED(D3DXLoadMeshFromX(planetMesh, D3DXMESH_SYSTEMMEM, this->device.direct3Device9, NULL,
			&this->pD3DXMtrlBuffer, NULL, &this->NumMaterials,
			&this->Mesh)))
		{
			MessageBox(NULL, "Could not find tiger.x", "Meshes.exe", MB_OK);
			return E_FAIL;
		}
	}
}
VOID Planets::pathFile(D3DXMATERIAL* d3dxMaterials, DWORD i) {
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
VOID Planets::loadMesh(LPCSTR planetTexture, LPCSTR planetMesh) {
	this->setTexture(planetTexture);
	this->setMesh(planetMesh);
	D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)this->pD3DXMtrlBuffer->GetBufferPointer();
	this->MeshMaterials = new D3DMATERIAL9[NumMaterials];
	this->MeshTextures = new LPDIRECT3DTEXTURE9[NumMaterials];
	for (DWORD i = 0; i < this->NumMaterials; i++)
	{
		this->MeshMaterials[i] = d3dxMaterials[i].MatD3D;
		this->MeshMaterials[i].Ambient = this->MeshMaterials[i].Diffuse;
		this->MeshTextures[i] = NULL;
		if (d3dxMaterials[i].pTextureFilename != NULL && lstrlen(d3dxMaterials[i].pTextureFilename) > 0)
		{
			if (FAILED(D3DXCreateTextureFromFile(this->device.direct3Device9, d3dxMaterials[i].pTextureFilename, &this->MeshTextures[i])))
			{
				this->pathFile(d3dxMaterials, i);

			}
		}
	}
}
VOID Planets::createPlanet(Pozition translation, Pozition rotation) {
	this->setTranslation(translation);
	this->setRotation(rotation);
	this->setScale();
	this->transform *= this->scale;
	this->device.direct3Device9->SetTransform(D3DTS_WORLD, &this->transform);

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
VOID Planets::setTranslation(Pozition translation) {

	D3DXMatrixIdentity(&this->transform);
	D3DXMatrixTranslation(&this->transform, translation.earthPozition.x, translation.earthPozition.y, translation.earthPozition.z);
}
VOID Planets::setRotation(Pozition rotation) {
	D3DXMatrixIdentity(&this->rotation);

	D3DXMatrixRotationX(&this->rotation, rotation.earthPozition.rotX);
}
VOID Planets::setScale() {
	D3DXMatrixIdentity(&this->scale);
	D3DXMatrixScaling(&this->scale, 1, 1, 1);
}
VOID Planets::setDevice(Device& device) {
	this->device = device;
}
VOID Planets::setLight(BOOL on_off) {

	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(light));
	light.Type = D3DLIGHT_POINT;
	light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	light.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	light.Range = 100.0f;
	light.Attenuation0 = 1.0f;
	light.Position = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	this->device.direct3Device9->SetLight(0, &light);
	this->device.direct3Device9->LightEnable(0, on_off);
	this->device.direct3Device9->SetRenderState(D3DRS_LIGHTING, on_off);
	this->device.direct3Device9->SetRenderState(D3DRS_AMBIENT, 0);
}
