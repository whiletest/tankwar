#include "StdAfx.h"
#include "MyView.h"
#include "Cube.h"
#include "Torus.h"
#include "Tank.h"
#include "CameraEx.h"
#include "terrain.h"
#include "SObjManage.h"


CMyView::CMyView(void):
    m_bkColor(0x00000000),
	m_pPlayer(0),
	m_pCamera(0),
	m_pDevice(0),
	m_pTerrain(0),
	m_pObjMng(0)
{
}


CMyView::~CMyView(void)
{
	if (m_pDevice)
		m_pDevice->Release();

	if (m_pCamera)
		delete m_pCamera;

	if (m_pTerrain)
		delete m_pTerrain;

	if (m_pObjMng)
		delete m_pObjMng;
}

bool CMyView::InitD3D(HINSTANCE hInstance,
	HWND hwnd,int width, int height, 
	bool windowed, D3DDEVTYPE deviceType)
{
	// Init D3D: 
	//

	HRESULT hr = 0;

	// Step 1: Create the IDirect3D9 object.

	IDirect3D9* d3d9 = 0;
	d3d9 = Direct3DCreate9(D3D_SDK_VERSION);

	if( !d3d9 )
	{
		::MessageBox(0, TEXT("Direct3DCreate9() - FAILED"), 0, 0);
		return false;
	}

	// Step 2: Check for hardware vp.

	D3DCAPS9 caps;
	d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, deviceType, &caps);

	int vp = 0;
	if( caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT )
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	// Step 3: Fill out the D3DPRESENT_PARAMETERS structure.

	D3DPRESENT_PARAMETERS d3dpp;
	d3dpp.BackBufferWidth            = width;
	d3dpp.BackBufferHeight           = height;
	d3dpp.BackBufferFormat           = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount            = 1;
	d3dpp.MultiSampleType            = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality         = 0;
	d3dpp.SwapEffect                 = D3DSWAPEFFECT_DISCARD; 
	d3dpp.hDeviceWindow              = hwnd;
	d3dpp.Windowed                   = windowed;
	d3dpp.EnableAutoDepthStencil     = true; 
	d3dpp.AutoDepthStencilFormat     = D3DFMT_D24S8;
	d3dpp.Flags                      = 0;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;

	// Step 4: Create the device.

	hr = d3d9->CreateDevice(
		D3DADAPTER_DEFAULT, // primary adapter
		deviceType,         // device type
		hwnd,               // window associated with device
		vp,                 // vertex processing
		&d3dpp,             // present parameters
		&m_pDevice);            // return created device

	if( FAILED(hr) )
	{
		// try again using a 16-bit depth buffer
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

		hr = d3d9->CreateDevice(
			D3DADAPTER_DEFAULT,
			deviceType,
			hwnd,
			vp,
			&d3dpp,
			&m_pDevice);

		if( FAILED(hr) )
		{
			d3d9->Release(); // done with d3d9 object
			::MessageBox(0, TEXT("CreateDevice() - FAILED"), 0, 0);
			return false;
		}
	}

	d3d9->Release(); // done with d3d9 object

	m_width = width;
	m_height = height;
	m_hwnd = hwnd;
	m_hInst = hInstance;
	return true;
}

int CMyView::EnterMsgLoop()
{
	MSG msg;
	::ZeroMemory(&msg, sizeof(MSG));

	static float lastTime = (float)timeGetTime(); 

	while(msg.message != WM_QUIT)
	{
		if(::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		else
		{	
			float currTime  = (float)timeGetTime();
			float timeDelta = (currTime - lastTime)*0.001f;

			Display(timeDelta);

			lastTime = currTime;
		}
	}

	return msg.wParam;
}

bool CMyView::Setup()
{
	CSenceObj::SOBJ_INIT si;
	m_pPlayer = new CTank(si);
	m_pPlayer->Setup(m_pDevice);

	m_pCamera = new CCameraEx;
	//位置信息在display中设置..

	D3DXVECTOR3 lightDirection(0.0f, 1.0f, 0.0f);
	m_pTerrain = new Terrain(m_pDevice, TERRAIN_FILE_PATH, 64, 64, 10, 0.5f);
	m_pTerrain->loadTexture(TEXTURE_FILE_PATH);

	m_pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	m_pObjMng = new CSObjManage("null");

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(&proj,PI*0.5f,
		(float)m_width/(float)m_height,1.0f,1000.0f);
	m_pDevice->SetTransform(D3DTS_PROJECTION,&proj);


	return true;
}

void CMyView::Display(float timeDelta)
{
	if (m_pDevice)
	{
 		D3DXVECTOR3 dir = m_pPlayer->GetMuzzleDir();
 		dir.y = 0.0f;
		m_pCamera->SetVecLook(dir);
		D3DXVECTOR3 pos = m_pPlayer->GetPosition();
		pos.y += CAMERA_Y_DELTA;
		pos.x += (dir.x * CAMERA_XZ_DELTA);
		pos.z += (dir.z * CAMERA_XZ_DELTA);
		m_pCamera->SetPosition(pos);


		m_pPlayer->OnTerrian(m_pTerrain, 0);
		m_pObjMng->DoManagement(m_pPlayer, m_pTerrain);

		D3DXMATRIX view;
		m_pCamera->GetViewMatrix(&view);
		m_pDevice->SetTransform(D3DTS_VIEW, &view);

		m_pDevice->Clear(0,0,D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,m_bkColor,1.0f,0);
		m_pDevice->BeginScene();

		m_pPlayer->Display(m_pDevice, timeDelta);
		m_pObjMng->Display(m_pDevice, timeDelta);

		if (m_pTerrain)
		{
			D3DXMATRIX terra;
			D3DXMatrixIdentity(&terra);
			m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
			m_pTerrain->draw(&terra, false);
		}

		m_pDevice->EndScene();
		m_pDevice->Present(0,0,0,0);
	}
}

//////////////////////////////////////////////////////////////////////////
//player interface
void CMyView::P_ChangeCurState(UINT cst)
{
	m_pPlayer->ChangeCurState(cst);
}

void CMyView::P_Fire()
{
	CBullet *pBul = m_pPlayer->Fire(m_pDevice);
	if (pBul)
		m_pObjMng->AddBullet(pBul);
}

//////////////////////////////////////////////////////////////////////////
//camera func
void CMyView::Cam_Yaw()
{
	UINT ps = m_pPlayer->GetCurState();
	if (IS_FB_STATIC(ps) && IS_GRPN_STATIC(ps))
		return;

	if (!IS_FB_STATIC(ps))
	{
		float angleDelta = ANGLE_SCALE_THREE * m_pPlayer->GetTyreCurAngle();
		if (angleDelta != 0.0f)
		{
			if (IS_BACKWARD(ps))
				angleDelta = -angleDelta;

			m_pCamera->Yaw(angleDelta);
		}
	}

	if (!IS_GRPN_STATIC(ps))
	{
		float angleDelta = CPlayer::GetCapAngleDelta();
		if (IS_GRN(ps))
			angleDelta = -angleDelta;

		m_pCamera->Yaw(angleDelta);
	}
}