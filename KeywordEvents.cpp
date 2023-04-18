#include "KeywordEvents.h"

VOID UserEvents::unAcquire() {
	this->g_pDinKeyboard->Unacquire();    
	this->g_pDinmouse->Unacquire();
	this->g_pDin->Release();
}

VOID UserEvents::detectInput() {
	this->g_pDinKeyboard->Acquire();
	this->g_pDinmouse->Acquire();
	this->g_pDinKeyboard->GetDeviceState(256, (LPVOID)this->g_Keystate);
	this->g_pDinmouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&this->g_pMousestate);

}
HRESULT UserEvents::createInputDevice(HINSTANCE hInstance, HWND hWnd) {
	DirectInput8Create(hInstance,    
		DIRECTINPUT_VERSION,   
		IID_IDirectInput8,    
		(void**)&this->g_pDin,    
		NULL);    
	this->g_pDin->CreateDevice(GUID_SysKeyboard,
		&this->g_pDinKeyboard,
		NULL);   
	this->g_pDin->CreateDevice(GUID_SysMouse,
		&this->g_pDinmouse,
		NULL); 
	this->g_pDinKeyboard->SetDataFormat(&c_dfDIKeyboard);
	this->g_pDinmouse->SetDataFormat(&c_dfDIMouse);
	this->g_pDinKeyboard->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	this->g_pDinmouse->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND);
	return S_OK;
}
bool UserEvents::exit() {
	return this->g_Keystate[DIK_ESCAPE] & 0x80;
}