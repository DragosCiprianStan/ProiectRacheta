#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <d3dx9tex.h>
#include <dinput.h>
#include "Device.h"
class CXCamera
{
private:
protected:
	D3DXVECTOR3 m_Position;
	D3DXVECTOR3 m_LookAt;
	D3DXVECTOR3 m_Right;
	D3DXVECTOR3 m_Up;

	float m_fRotAboutUp;
	float m_fRotAboutRight;
	float m_fRotAboutFacing;
	Device dev;
	D3DXMATRIX m_ViewTransform;

	LPDIRECT3DDEVICE9 m_pDevice;
	bool m_UpdateRequired;

	HRESULT UpdateCameraMatrices();
public:
	void LookAtPos(D3DXVECTOR3* Position, D3DXVECTOR3* LookAt, D3DXVECTOR3* Up);
	void SetPosition(FLOAT X, FLOAT Y, FLOAT Z);
	D3DXVECTOR3* GetPosition() { return &m_Position; }
	D3DXVECTOR3* GetLookAt() { return &m_LookAt; }
	D3DXVECTOR3* GetRight() { return &m_Right; }
	D3DXVECTOR3* GetUp() { return &m_Up; }
	D3DXMATRIX* GetViewMatrix() { return &m_ViewTransform; }
	CXCamera(LPDIRECT3DDEVICE9 pDevice);
	HRESULT Update();
	void RotateDown(float fAngle);
	void RotateRight(float fAngle);
	void Roll(float fAngle);
	void MoveForward(float fDist);
	void MoveRight(float fDist);
	void MoveUp(float fDist);
	void MoveInDirection(float fDist, D3DXVECTOR3* Dir);
};


