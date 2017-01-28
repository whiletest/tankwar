#pragma once

#include "camera.h"

class CCameraEx : public CCamera
{
public:
	CCameraEx(void);
	virtual ~CCameraEx(void);
	//////////////////////////////////////////////////////////////////////////
	void SetVecRight(const D3DXVECTOR3 &right) {m_right = right;}
	D3DXVECTOR3 GetVecRight()const {return m_right;}

	void GetViewMatrix(D3DXMATRIX *view);

	void Walk(float units); //forward/backward
	void Walk(const D3DXVECTOR3 &axis, float units);
	void Yaw(float angleDelta);
protected:
	D3DXVECTOR3 m_right;
};

