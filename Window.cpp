#include "Window.h"

VOID Window::windowMaker(WNDPROC  MsgProc) {
	this->wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
						GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
						  "D3D Tutorial", NULL };
	RegisterClassEx(&this->wc);
	this->hWnd = CreateWindow("D3D Tutorial", "Racheta",
		WS_OVERLAPPEDWINDOW, 0, 0, 1024, 1024,
		GetDesktopWindow(), NULL, this->wc.hInstance, NULL);
}

