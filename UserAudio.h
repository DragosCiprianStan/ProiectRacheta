#include <Windows.h>
#include <d3dx9.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <d3dx9tex.h>
#include <dinput.h>
#include <mmsystem.h>   
#include <d3dx9.h>   
#include <dshow.h>   
#include <mmsystem.h>
#include <d3dx9.h>


#pragma once
class UserAudio
{
#define WM_GRAPHNOTIFY  WM_APP + 1   
	IGraphBuilder* graphBuilder = NULL;

	IMediaControl* mediaControl = NULL;

	IMediaEventEx* mediaEvent = NULL;

	IMediaSeeking* mediaSeeking = NULL;
public :
	HRESULT InitDirectShow(HWND hWnd);
	VOID play();
	VOID stop();
};

