#include <Windows.h>
#include <d3dx9.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <d3dx9tex.h>
#include <dinput.h>
struct POZITION
{
	FLOAT x = 0.1;
	FLOAT y = 0.1;
	FLOAT z = 0.1;
	FLOAT rotX = 0.1;
	FLOAT rotY = 0.1;
	FLOAT rotZ = 0.1;
};

#pragma once
class Pozition
{
public:
	POZITION rocketPozition;
	POZITION earthPozition;
};

