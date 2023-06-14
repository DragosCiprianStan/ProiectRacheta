#include "UserAudio.h"
HRESULT UserAudio::InitDirectShow(HWND hWnd) {
	HRESULT hr = CoCreateInstance(CLSID_FilterGraph, NULL,
		CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void**)&this->graphBuilder);

	hr = graphBuilder->QueryInterface(IID_IMediaControl, (void**)&this->mediaControl);
	hr = graphBuilder->QueryInterface(IID_IMediaEventEx, (void**)&this->mediaEvent);
	hr = graphBuilder->QueryInterface(IID_IMediaSeeking, (void**)&this->mediaSeeking);
 
	hr = graphBuilder->RenderFile(L"E:\\Facultate\\DirectX\\Proiect\\Audio\\RocketAudio.MP3", NULL);

	this->mediaEvent->SetNotifyWindow((OAHWND)hWnd, WM_GRAPHNOTIFY, 0);
	return S_OK;
}

VOID UserAudio::play() {
	this->mediaControl->Run();
}
VOID UserAudio::stop() {
	this->mediaControl->Stop();
}