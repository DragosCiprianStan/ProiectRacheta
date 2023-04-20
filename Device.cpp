#include "Device.h"

HRESULT Device::createD3DObject(HWND hWnd) {
	if (NULL == (this->directD3D = Direct3DCreate9(D3D_SDK_VERSION)))
		return E_FAIL;
}

VOID Device::deviceStructure() {

	ZeroMemory(&this->d3dpp, sizeof(this->d3dpp));
	this->d3dpp.Windowed = TRUE;
	this->d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	this->d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	this->d3dpp.EnableAutoDepthStencil = TRUE;
	this->d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
}

HRESULT Device::createDevice(HWND hWnd) {
	this->deviceStructure();

	if (FAILED(this->directD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&this->d3dpp, &this->direct3Device9)))
	{
		if (FAILED(directD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&this->d3dpp, &this->direct3Device9)))
			return E_FAIL;
	}
	this->direct3Device9->SetRenderState(D3DRS_ZENABLE, TRUE);
	this->direct3Device9->SetRenderState(D3DRS_AMBIENT, 0xffffffff);
	this->direct3Device9->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	this->direct3Device9->SetRenderState(D3DRS_LIGHTING, FALSE);

}
VOID Device::cleanUpDevice() {
	if (this->direct3Device9 != NULL)
		this->direct3Device9->Release();
}
VOID Device::cleanUpDeviceObject() {
	if (this->directD3D != NULL)
		this->directD3D->Release();
}
VOID Device::SetupLights() {

	D3DMATERIAL9 mtrl;
	ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
	mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
	mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
	mtrl.Diffuse.b = mtrl.Ambient.b = 0.0f;
	mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;
	this->direct3Device9->SetMaterial(&mtrl);

	D3DXVECTOR3 vecDir;
	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(D3DLIGHT9));
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Diffuse.r = 1.0f;
	light.Diffuse.g = 1.0f;
	light.Diffuse.b = 1.0f;

	vecDir = D3DXVECTOR3(cosf(timeGetTime() / 350.0f),
		1.0f,
		sinf(timeGetTime() / 350.0f));
	D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &vecDir);
	light.Range = 1000.0f;
	this->direct3Device9->SetLight(0, &light);
	this->direct3Device9->LightEnable(0, TRUE);
	this->direct3Device9->SetRenderState(D3DRS_LIGHTING, TRUE);
	this->direct3Device9->SetRenderState(D3DRS_AMBIENT, 0x00202020);
}