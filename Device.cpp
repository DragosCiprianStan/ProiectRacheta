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