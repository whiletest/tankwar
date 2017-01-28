#pragma once

#include <vector>
#include "typedeclare.h"

class CMyView
{
#define CAMERA_XZ_DELTA  -5.0f  
#define CAMERA_Y_DELTA   2.5f
	typedef std::vector<CSenceObj*>::iterator OBJ_ITER;
public:
	CMyView(void);
	~CMyView(void);
	//////////////////////////////////////////////////////////////////////////
	bool InitD3D(HINSTANCE hInstance, 
		HWND hwnd, int width, int height,
		bool windowed, D3DDEVTYPE deviceType);
	////
	int EnterMsgLoop();
	////
	bool Setup();
public: //player interface
	void P_ChangeCurState(UINT md);
	void P_Fire(void);
private:
	//////////////////////////////////////////////////////////////////////////view fun
	void Display(float timeDelta);
	//////////////////////////////////////////////////////////////////////////camera fun
	void Cam_Yaw();
private:
	const D3DCOLOR m_bkColor;
	IDirect3DDevice9 *m_pDevice;
	HWND m_hwnd;
	HINSTANCE m_hInst;
	int m_width;
	int m_height;
	//////////////////////////////////////////////////////////////////////////
	CPlayer *m_pPlayer;
	CCameraEx *m_pCamera;
	Terrain *m_pTerrain;
	CSObjManage *m_pObjMng;
};

