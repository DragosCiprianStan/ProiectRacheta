#include <Windows.h>
#include <d3dx9.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <d3dx9tex.h>
#include <dinput.h>
class Window
{
public:

	WNDCLASSEX wc;
	HWND hWnd;
	VOID windowMaker(WNDPROC  MsgProc);

};

