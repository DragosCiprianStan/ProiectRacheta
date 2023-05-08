#include "UserAudio.h"
HRESULT UserAudio::InitDirectShow(HWND hWnd) {
	HRESULT hr = CoCreateInstance(CLSID_FilterGraph, NULL,
		CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void**)&this->graphBuilder);

	//Create Media Control and Events   
	hr = graphBuilder->QueryInterface(IID_IMediaControl, (void**)&this->mediaControl);
	hr = graphBuilder->QueryInterface(IID_IMediaEventEx, (void**)&this->mediaEvent);
	hr = graphBuilder->QueryInterface(IID_IMediaSeeking, (void**)&this->mediaSeeking);

	//Load a file   
	hr = graphBuilder->RenderFile(L"E:\\Facultate\\DirectX\\Proiect\\Audio\\RocketAudio.MP3", NULL);

	//Set window for events  - basically we tell our event in case you raise an event use the following event id.
	this->mediaEvent->SetNotifyWindow((OAHWND)hWnd, WM_GRAPHNOTIFY, 0);
	return S_OK;
}

VOID UserAudio::play() {
	this->mediaControl->Run();
}
VOID UserAudio::stop() {
	this->mediaControl->Stop();
}