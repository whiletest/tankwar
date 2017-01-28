#include "StdAfx.h"
#include "Camera.h"


CCamera::CCamera(void)
{
	m_pos.x = 0.0f;m_pos.y = 2.0f;m_pos.z = -5.0f;
	m_look.x = 0.0f;m_look.y = 0.0f;m_look.z = 1.0f;
	m_up.x = 0.0f;m_up.y = 1.0f;m_up.z = 0.0f;
}


CCamera::~CCamera(void)
{
}

void CCamera::SetCamera(IDirect3DDevice9 *pDevice)
{
	D3DXMATRIX view;
	D3DXMatrixLookAtLH(&view,&m_pos,&m_look,&m_up);
	pDevice->SetTransform(D3DTS_VIEW,&view);
}