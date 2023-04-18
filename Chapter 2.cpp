#include <Windows.h>
#include <d3dx9.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <d3dx9tex.h>
#include <dinput.h>
//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")
LPDIRECT3D9             directD3D = NULL; // Used to create the D3DDevice
LPDIRECT3DDEVICE9       direct3Device9 = NULL; // Our rendering device
LPDIRECT3DVERTEXBUFFER9 vertexBuffer = NULL;
LPDIRECT3DTEXTURE9      g_pTexture = NULL; // Our texture
LPD3DXMESH              Mesh = NULL; // Our mesh object in sysmem
D3DMATERIAL9* MeshMaterials = NULL; // Materials for our mesh
LPDIRECT3DTEXTURE9* MeshTextures = NULL; // Textures for our mesh
DWORD                   NumMaterials = 0L;   // Number of mesh materials sau number of subsets
POINT					g_SurfacePosition;				// The position of the surface
BYTE					g_Keystate[256];
LPDIRECTINPUT8			g_pDin;							// the pointer to our DirectInput interface
LPDIRECTINPUTDEVICE8	g_pDinKeyboard;					// the pointer to the keyboard device
LPDIRECTINPUTDEVICE8	g_pDinmouse;					// the pointer to the mouse device
DIMOUSESTATE			g_pMousestate;

//-----------------------------------------------------------------------------
// Custom vertex
//-----------------------------------------------------------------------------
struct CUSTOMVERTEX
{
	D3DXVECTOR3 position; // The 3D position for the vertex
	D3DXVECTOR3 normala;   // The surface normal for the vertex
	FLOAT tu, tv;   // The texture coordinates 
};

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1 )

VOID DetectInput();
VOID Render();
VOID Cleanup();
VOID CleanDInput();

VOID CleanDInput()
{
	g_pDinKeyboard->Unacquire();    // make sure the keyboard is unacquired
	g_pDinmouse->Unacquire();    // make sure the mouse in unacquired
	g_pDin->Release();    // close DirectInput before exiting
}
VOID DetectInput()
{
	// get access if we don't have it already
	g_pDinKeyboard->Acquire();
	g_pDinmouse->Acquire();

	// get the input data
	g_pDinKeyboard->GetDeviceState(256, (LPVOID)g_Keystate);
	g_pDinmouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&g_pMousestate);


}
//-----------------------------------------------------------------------------
// Name: InitD3D()
// Desc: Initializes Direct3D
//-----------------------------------------------------------------------------
HRESULT InitD3D(HWND hWnd)
{
	// Create the D3D object.
	if (NULL == (directD3D = Direct3DCreate9(D3D_SDK_VERSION)))
		return E_FAIL;

	// Set up the structure used to create the D3DDevice. Since we are now
	// using more complex geometry, we will create a device with a zbuffer.
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	// Create the D3DDevice
	if (FAILED(directD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp, &direct3Device9)))
	{
		if (FAILED(directD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &direct3Device9)))
			return E_FAIL;
	}

	// Turn on the zbuffer
	direct3Device9->SetRenderState(D3DRS_ZENABLE, TRUE);

	// Turn on ambient lighting 
	direct3Device9->SetRenderState(D3DRS_AMBIENT, 0xffffffff);

	// Turn off culling, so we see the front and back of the triangle
	direct3Device9->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// Turn off D3D lighting, since we are providing our own vertex colors
	direct3Device9->SetRenderState(D3DRS_LIGHTING, FALSE);

	return S_OK;
}


HRESULT InitDInput(HINSTANCE hInstance, HWND hWnd)
{
	// create the DirectInput interface
	DirectInput8Create(hInstance,    // the handle to the application
		DIRECTINPUT_VERSION,    // the compatible version
		IID_IDirectInput8,    // the DirectInput interface version
		(void**)&g_pDin,    // the pointer to the interface
		NULL);    // COM stuff, so we'll set it to NULL

	// create the keyboard device
	g_pDin->CreateDevice(GUID_SysKeyboard,    // the default keyboard ID being used
		&g_pDinKeyboard,    // the pointer to the device interface
		NULL);    // COM stuff, so we'll set it to NULL

	// create the mouse device
	g_pDin->CreateDevice(GUID_SysMouse,
		&g_pDinmouse,  // the pointer to the device interface
		NULL); // COM stuff, so we'll set it to NULL

	// set the data format to keyboard format
	g_pDinKeyboard->SetDataFormat(&c_dfDIKeyboard);

	// set the data format to mouse format
	g_pDinmouse->SetDataFormat(&c_dfDIMouse);

	// set the control we will have over the keyboard
	g_pDinKeyboard->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);

	// set the control we will have over the mouse
	g_pDinmouse->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND);

	return S_OK;
}

int n = 23;
//-----------------------------------------------------------------------------
// Name: InitGeometry()
// Desc: Load the vertex buffer
//-----------------------------------------------------------------------------
HRESULT InitGeometry()
{
	LPD3DXBUFFER pD3DXMtrlBuffer;
	// Use D3DX to create a texture from a file based image
	if (FAILED(D3DXCreateTextureFromFile(direct3Device9, "banana.bmp", &g_pTexture)))
	{
		// If texture is not in current folder, try parent folder
		if (FAILED(D3DXCreateTextureFromFile(direct3Device9, "E:\\Facultate\\DirectX\\Lab03\\Surse_texturare\\banana.bmp", &g_pTexture))) //aici nu vede imaginea
		{
			MessageBox(NULL, "Could not find bannana.", "Textures.exe", MB_OK);
			return E_FAIL;
		}
	}

	// Load the mesh from the specified file
	if (FAILED(D3DXLoadMeshFromX("Tiger.x", D3DXMESH_SYSTEMMEM,
		direct3Device9, NULL,
		&pD3DXMtrlBuffer, NULL, &NumMaterials,
		&Mesh)))
	{
		// If model is not in current folder, try parent folder
		if (FAILED(D3DXLoadMeshFromX("C:\\Users\\radaa\\Documents\\FACULTATE3\\FacultaSem2\\LabDirectX\\Chapter 2\\Tiger.x", D3DXMESH_SYSTEMMEM,
			direct3Device9, NULL,
			&pD3DXMtrlBuffer, NULL, &NumMaterials,
			&Mesh)))
		{
			MessageBox(NULL, "Could not find tiger.x", "Meshes.exe", MB_OK);
			return E_FAIL;
		}
	}
	// We need to extract the material properties and texture names from the 
	// pD3DXMtrlBuffer
	D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
	MeshMaterials = new D3DMATERIAL9[NumMaterials];
	MeshTextures = new LPDIRECT3DTEXTURE9[NumMaterials];//contine drmaterial9

	for (DWORD i = 0; i < NumMaterials; i++)
	{
		// Copy the material
		MeshMaterials[i] = d3dxMaterials[i].MatD3D;

		// Set the ambient color for the material (D3DX does not do this)
		MeshMaterials[i].Ambient = MeshMaterials[i].Diffuse;

		MeshTextures[i] = NULL;
		if (d3dxMaterials[i].pTextureFilename != NULL &&
			lstrlen(d3dxMaterials[i].pTextureFilename) > 0)
		{
			// Create the texture
			if (FAILED(D3DXCreateTextureFromFile(direct3Device9,
				d3dxMaterials[i].pTextureFilename,
				&MeshTextures[i])))
			{
				// If texture is not in current folder, try parent folder
				const TCHAR* strPrefix = TEXT("..\\");
				const int lenPrefix = lstrlen(strPrefix);
				TCHAR strTexture[MAX_PATH];
				lstrcpyn(strTexture, strPrefix, MAX_PATH);
				lstrcpyn(strTexture + lenPrefix, d3dxMaterials[i].pTextureFilename, MAX_PATH - lenPrefix);
				// If texture is not in current folder, try parent folder
				if (FAILED(D3DXCreateTextureFromFile(direct3Device9,
					strTexture,
					&MeshTextures[i])))
				{
					MessageBox(NULL, "Could not find texture map", "Meshes.exe", MB_OK);
				}
			}
		}
	}


	D3DXVECTOR3 normala(1, 1, 1);
	// Initialize three vertices for rendering a triangle
	CUSTOMVERTEX g_Vertices[1000] =
	{
		/* {D3DXVECTOR3(-1.0f,-1.0f, 0.0f), D3DXVECTOR3(-1.0f,-1.0f, 1.0f), 0.0f, 0.0f,},
		 {  D3DXVECTOR3(1.0f,-1.0f, 0.0f), D3DXVECTOR3(1.0f,-1.0f, 1.0f), 1.0f, 0.0f, },
		 {  D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 1.0f), 0.5f, 0.5f, },*/


		 // Base
		 {D3DXVECTOR3(0.0f,0.0f, 0.0f), normala ,0, 0},
		 {D3DXVECTOR3(1.0f,0.0f, 0.0f), normala, 1.0f, 0.0f},
		 {D3DXVECTOR3(0.0f, 1.0f, 0.0f), normala,0.0f, 1.0f},

		 {D3DXVECTOR3(0.0f,0.0f, 0.0f), normala ,0, 0},
		 {D3DXVECTOR3(0.0f, 0.0f, 1.0f), normala,1.0f,0.0f},
		 {D3DXVECTOR3(0.0f, 1.0f, 0.0f), normala,0.0f, 1.0f},

		 {D3DXVECTOR3(0.0f,0.0f, 0.0f), normala ,0, 0},
		 {D3DXVECTOR3(0.0f, 0.0f, 1.0f), normala,0.0f,1.0f},
		 {D3DXVECTOR3(1.0f,0.0f, 0.0f), normala, 1.0f, 1.0f},

		 {D3DXVECTOR3(0.0f, 0.0f, 1.0f), normala,0.0f,0.0f},
		 {D3DXVECTOR3(1.0f,0.0f, 0.0f), normala, 1.0f, 0.0f},
		 {D3DXVECTOR3(0.0f, 1.0f, 0.0f), normala,0.0f, 1.0f},

	};

	g_Vertices[12] = { D3DXVECTOR3(0.0f,0.0f, 0.0f), normala ,0.75, 0.75 };

	for (int i = 0; i < n; i++)
	{
		float agle = 2 * 3.14 * i / n;
		g_Vertices[12 + i + 1] = { D3DXVECTOR3(cosf(agle),sinf(agle), 0.0f), normala ,(float)((cosf(agle) + 1) / 4 + 0.5),(float)((sinf(agle) + 1) / 4 + 0.5) };
	}
	g_Vertices[12 + n] = g_Vertices[12 + 1];


	// Create the vertex buffer.
	if (FAILED(direct3Device9->CreateVertexBuffer(1000 * sizeof(CUSTOMVERTEX),
		0, D3DFVF_CUSTOMVERTEX,
		D3DPOOL_DEFAULT, &vertexBuffer, NULL)))
	{
		return E_FAIL;
	}

	// Fill the vertex buffer.
	VOID* pVertices;
	if (FAILED(vertexBuffer->Lock(0, sizeof(g_Vertices), (void**)&pVertices, 0)))
		return E_FAIL;
	memcpy(pVertices, g_Vertices, sizeof(g_Vertices));
	vertexBuffer->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------------------
// Name: Cleanup()
// Desc: Releases all previously initialized objects
//-----------------------------------------------------------------------------
VOID Cleanup()
{


	if (MeshMaterials != NULL)
		delete[] MeshMaterials;

	if (MeshTextures)
	{
		for (DWORD i = 0; i < NumMaterials; i++)
		{
			if (MeshTextures[i])
				MeshTextures[i]->Release();
		}
		delete[] MeshTextures;
	}

	if (g_pTexture != NULL)
		g_pTexture->Release();

	if (vertexBuffer)
		vertexBuffer->Release();

	if (direct3Device9 != NULL)
		direct3Device9->Release();

	if (directD3D != NULL)
		directD3D->Release();
}

void SetupWorldMatrix()
{
	// For our world matrix, we will just leave it as the identity
	D3DXMATRIX g_Transform, m2;
	D3DXMatrixIdentity(&g_Transform);
	D3DXMatrixIdentity(&m2);
	D3DXMatrixScaling(&m2, 1.3, 1.3, 1.3);
	D3DXMatrixRotationY(&g_Transform, timeGetTime() / 1000.0);
	g_Transform *= m2;
	direct3Device9->SetTransform(D3DTS_WORLD, &g_Transform);
}

void SetupWorldMatrixCerc()
{
	// For our world matrix, we will just leave it as the identity
	D3DXMATRIX g_Transform, m2;
	D3DXMatrixIdentity(&g_Transform);
	D3DXMatrixIdentity(&m2);
	D3DXMatrixTranslation(&m2, 1.3, 1.3, 1.3);
	D3DXMatrixRotationY(&g_Transform, timeGetTime() / 1000.0);
	g_Transform *= m2;
	direct3Device9->SetTransform(D3DTS_WORLD, &g_Transform);
}
void SetupViewMatrix()
{
	D3DXVECTOR3 vEyePt(0.0f, 3.0f, -5.0f);
	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);
	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);
	direct3Device9->SetTransform(D3DTS_VIEW, &matView);
}

void SetupProjectionMatrix()
{
	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4, 1.0f, 1.0f, 100.0f);
	direct3Device9->SetTransform(D3DTS_PROJECTION, &matProj);
}

//-----------------------------------------------------------------------------
// Name: SetupMatrices()
// Desc: Sets up the world, view, and projection transform matrices.
//-----------------------------------------------------------------------------
VOID SetupMatrices()
{
	SetupWorldMatrix();

	SetupViewMatrix();

	SetupProjectionMatrix();
}

//-----------------------------------------------------------------------------
// Name: SetupLights()
// Desc: Sets up the lights and materials for the scene.
//-----------------------------------------------------------------------------
VOID SetupLights()
{
	// Set up a material. The material here just has the diffuse and ambient
	// colors set to yellow. Note that only one material can be used at a time.
	D3DMATERIAL9 mtrl;
	ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
	mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
	mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
	mtrl.Diffuse.b = mtrl.Ambient.b = 0.0f;
	mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;
	direct3Device9->SetMaterial(&mtrl);

	// Set up a white, directional light, with an oscillating direction.
	// Note that many lights may be active at a time (but each one slows down
	// the rendering of our scene). However, here we are just using one. Also,
	// we need to set the D3DRS_LIGHTING renderstate to enable lighting
	D3DXVECTOR3 vecDir;
	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(D3DLIGHT9));
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Diffuse.r = 1.0f;
	light.Diffuse.g = 1.0f;
	light.Diffuse.b = 1.0f;
	// sa se execute aplicatia cu lumina in pozitie fixa si cu lumina rotindu-se
	vecDir = D3DXVECTOR3(cosf(timeGetTime() / 350.0f),
		1.0f,
		sinf(timeGetTime() / 350.0f));
	//vecDir = D3DXVECTOR3(1, 1.0f, 1 );
	D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &vecDir);
	light.Range = 1000.0f;
	direct3Device9->SetLight(0, &light);
	direct3Device9->LightEnable(0, TRUE);
	direct3Device9->SetRenderState(D3DRS_LIGHTING, TRUE);

	// Finally, turn on some ambient light.
	direct3Device9->SetRenderState(D3DRS_AMBIENT, 0x00202020);
}
double position_x = 0.9;
double position_y = 0;
//-----------------------------------------------------------------------------
// Name: Render()
// Desc: Draws the scene
//-----------------------------------------------------------------------------
VOID Render()
{
	// Clear the backbuffer and the zbuffer
	direct3Device9->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);

	// Begin the scene
	if (SUCCEEDED(direct3Device9->BeginScene()))
	{
		// Setup the lights and materials
		SetupLights();

		// Setup the world, view, and projection matrices
		SetupMatrices();
		direct3Device9->SetRenderState(D3DRS_LIGHTING, FALSE);
		for (DWORD i = 0; i < NumMaterials; i++)
		{
			// Set the material and texture for this subset
			direct3Device9->SetMaterial(&MeshMaterials[i]);
			direct3Device9->SetTexture(0, MeshTextures[i]);

			// Draw the mesh subset
			Mesh->DrawSubset(i);
		}

		D3DXMATRIX g_Transform;
		D3DXMatrixIdentity(&g_Transform);
		D3DXMatrixTranslation(&g_Transform, position_x, position_y, 0);

		D3DXMATRIX g_TransformrrOTATIO;
		D3DXMatrixIdentity(&g_TransformrrOTATIO);
		UINT iTime = timeGetTime() % 100000;
		FLOAT fAngle = iTime * (2.0f * D3DX_PI) / 1000.0f;
		D3DXMatrixRotationZ(&g_TransformrrOTATIO, fAngle);

		g_Transform = g_TransformrrOTATIO * g_Transform;
		
		direct3Device9->SetTransform(D3DTS_WORLD, &g_Transform);


		for (DWORD i = 0; i < NumMaterials; i++)
		{
			// Set the material and texture for this subset
			direct3Device9->SetMaterial(&MeshMaterials[i]);
			//	direct3Device9->SetTexture(0, MeshTextures[i]);
			direct3Device9->SetTexture(0, g_pTexture);

			// Draw the mesh subset
			Mesh->DrawSubset(i);
		}





		// Setup our texture. Using textures introduces the texture stage states,
		// which govern how textures get blended together (in the case of multiple
		// textures) and lighting information. In this case, we are modulating
		// (blending) our texture with the diffuse color of the vertices.
		direct3Device9->SetTexture(0, g_pTexture);
		direct3Device9->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		direct3Device9->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		direct3Device9->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
		direct3Device9->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

		direct3Device9->SetStreamSource(0, vertexBuffer, 0, sizeof(CUSTOMVERTEX));
		direct3Device9->SetFVF(D3DFVF_CUSTOMVERTEX);

		SetupWorldMatrix();
		direct3Device9->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 4);
		SetupWorldMatrixCerc();

		direct3Device9->DrawPrimitive(D3DPT_TRIANGLEFAN, 12, n - 1);
		//deseneaza puncte
		//direct3Device9->DrawPrimitive( D3DPT_POINTLIST, 0, 3 );
		//deseneaza linii
		//direct3Device9->DrawPrimitive( D3DPT_LINESTRIP, 0, 2 );
		// End the scene
		direct3Device9->EndScene();
	}

	// Present the backbuffer contents to the display
	direct3Device9->Present(NULL, NULL, NULL, NULL);
}

//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: The window's message handler
//-----------------------------------------------------------------------------
LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		Cleanup();
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: The application's entry point
//-----------------------------------------------------------------------------
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, INT)
{
	// Register the window class
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
					  GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
					  "D3D Tutorial", NULL };
	RegisterClassEx(&wc);

	// Create the application's window
	HWND hWnd = CreateWindow("D3D Tutorial", "Vertex Buffer",
		WS_OVERLAPPEDWINDOW, 0, 0,1980, 1980,
		GetDesktopWindow(), NULL, wc.hInstance, NULL);

	// Initialize Direct3D
	if (SUCCEEDED(InitD3D(hWnd)))
	{
		InitDInput(hInst, hWnd);

		// Show the window
		ShowWindow(hWnd, SW_SHOWDEFAULT);
		UpdateWindow(hWnd);
		// Create the scene geometry
		if (SUCCEEDED(InitGeometry()))
		{
			// Show the window
			ShowWindow(hWnd, SW_SHOWDEFAULT);
			UpdateWindow(hWnd);

			// Enter the message loop
			MSG msg;
			ZeroMemory(&msg, sizeof(msg));
			while (msg.message != WM_QUIT)
			{
				if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				else {
					DetectInput();
					Render();
					if (g_Keystate[DIK_ESCAPE] & 0x80) {
						PostMessage(hWnd, WM_DESTROY, 0, 0);
					}
					if (g_Keystate[DIK_D] & 0x80) {
						position_x += 0.1;
					}
					if (g_Keystate[DIK_A] & 0x80) {
						position_x -= 0.1;
					}
					position_x += g_pMousestate.lX;
					position_y += g_pMousestate.lY;
				}
			}
		}
	}

	UnregisterClass("D3D Tutorial", wc.hInstance);
	return 0;
}