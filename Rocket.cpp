#include "Rocket.h"

HRESULT Rocket::setTexture() {
	if (FAILED(D3DXCreateTextureFromFile(this->device.direct3Device9, "RocketTexture.png", &this->texture)))
	{
		if (FAILED(D3DXCreateTextureFromFile(this->device.direct3Device9, "E:\\Facultate\\DirectX\\Proiect\\Object\\RocketMesh\\RocketTexture.png", &this->texture)))
		{
			MessageBox(NULL, "Could not find bannana.", "Textures.exe", MB_OK);
			return E_FAIL;
		}
	}
}
HRESULT Rocket::setMesh() {

	if (FAILED(D3DXLoadMeshFromX("RocketMesh.x", D3DXMESH_SYSTEMMEM,
		this->device.direct3Device9, NULL,
		&this->pD3DXMtrlBuffer, NULL, &this->NumMaterials,
		&this->Mesh)))
	{
		if (FAILED(D3DXLoadMeshFromX("E:\\Facultate\\DirectX\\Proiect\\Object\\RocketMesh\\RocketMesh.x", D3DXMESH_SYSTEMMEM,
			this->device.direct3Device9, NULL,
			&this->pD3DXMtrlBuffer, NULL, &this->NumMaterials,
			&this->Mesh)))
		{
			MessageBox(NULL, "Could not find Rocket.x", "Meshes.exe", MB_OK);
			return E_FAIL;
		}
	}
}
VOID Rocket::loadMesh() {
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
VOID Rocket::createRocket(Pozition poz) {

	this->setTranslation(poz);
	this->setRotation(poz);
	this->setScale();
	this->rotation *= this->scale * this->transform;
	this->device.direct3Device9->SetTransform(D3DTS_WORLD, &this->rotation);

	for (DWORD i = 0; i < this->NumMaterials; i++)
	{
		this->device.direct3Device9->SetMaterial(&this->MeshMaterials[i]);
		this->device.direct3Device9->SetTexture(0, this->texture);
		this->Mesh->DrawSubset(i);
	}
}
VOID Rocket::setTranslation(Pozition translation) {

	D3DXMatrixIdentity(&this->transform);
	D3DXMatrixTranslation(&this->transform, translation.rocketPozition.x, translation.rocketPozition.y, translation.rocketPozition.z);
}
VOID Rocket::setRotation(Pozition rotation) {
	D3DXMatrixIdentity(&this->rotation);
	D3DXMatrixRotationYawPitchRoll(&this->rotation, rotation.rocketPozition.rotX, rotation.rocketPozition.rotY, rotation.rocketPozition.rotZ);
	
}
VOID Rocket::setRotationY(FLOAT y) {
	D3DXMatrixIdentity(&this->rotation);
	D3DXMatrixRotationY(&this->rotation, y);

}
VOID Rocket::setScale() {
	D3DXMatrixIdentity(&this->scale);
	D3DXMatrixScaling(&this->scale, 0.3, 0.3, 0.3);
}
VOID Rocket::cleanUpTexture() {


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
VOID Rocket::setDevice(Device& device) {
	this->device = device;
}
VOID Rocket::pozition(int x, int y) {
	D3DXMatrixIdentity(&this->transform);
	D3DXMatrixTranslation(&this->transform, x, y, 0);
	this->device.direct3Device9->SetTransform(D3DTS_WORLD, &this->transform);
}
VOID Rocket::setLight(BOOL on_off, Pozition meshPosition) {
	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(light));
	light.Type = D3DLIGHT_POINT;
	light.Diffuse = D3DXCOLOR(0.25f, 0.25f, 1.0f, 0.0f);
	light.Specular = D3DXCOLOR(0.5f, 0.5f, 1.0f, 1.0f);
	light.Range = 1.0f;
	light.Attenuation0 = 1.0f;
	light.Position = D3DXVECTOR3(meshPosition.rocketPozition.x, meshPosition.rocketPozition.y, meshPosition.rocketPozition.z+0.5);
	this->device.direct3Device9->SetLight(0, &light);
	this->device.direct3Device9->LightEnable(0, on_off);
	this->device.direct3Device9->SetRenderState(D3DRS_LIGHTING, on_off);
	this->device.direct3Device9->SetRenderState(D3DRS_AMBIENT, 0);
}