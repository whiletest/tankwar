#include "StdAfx.h"
#include "CameraEx.h"


CCameraEx::CCameraEx(void)
{
	m_right.x = 1.0f;m_right.y = 0.0f;m_right.z = 0.0f;
}


CCameraEx::~CCameraEx(void)
{
}

void CCameraEx::GetViewMatrix(D3DXMATRIX *view)
{
	D3DXVec3Normalize(&m_look, &m_look);
	D3DXVec3Cross(&m_right, &m_up, &m_look);
	D3DXVec3Normalize(&m_right, &m_right);
	D3DXVec3Cross(&m_up, &m_look, &m_right);
	D3DXVec3Normalize(&m_up, &m_up);

	float x = -D3DXVec3Dot(&m_right, &m_pos);
	float y = -D3DXVec3Dot(&m_up, &m_pos);
	float z = -D3DXVec3Dot(&m_look, &m_pos);

	(*view)(0,0) = m_right.x;(*view)(0,1) = m_up.x;(*view)(0,2) = m_look.x;(*view)(0,3) = 0.0f;
	(*view)(1,0) = m_right.y;(*view)(1,1) = m_up.y;(*view)(1,2) = m_look.y;(*view)(1,3) = 0.0f;
	(*view)(2,0) = m_right.z;(*view)(2,1) = m_up.z;(*view)(2,2) = m_look.z;(*view)(2,3) = 0.0f;
	(*view)(3,0) = x;(*view)(3,1) = y;(*view)(3,2) = z;(*view)(3,3) = 1.0f;
}

void CCameraEx::Walk(float units)
{
	m_pos += (m_look * units); 
}

void CCameraEx::Walk(const D3DXVECTOR3 &axis, float units)
{
	m_pos += (axis * units);
}

void CCameraEx::Yaw(float angleDelta)
{
	D3DXMATRIX RUp;

	D3DXMatrixRotationAxis(&RUp, &m_up, angleDelta);

	D3DXVec3TransformNormal(&m_right, &m_right, &RUp);
	D3DXVec3TransformNormal(&m_look, &m_look, &RUp);
}