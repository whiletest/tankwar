#pragma once


//如需移动的摄像机从此派生 
class CCamera
{
public:
	CCamera(void);
	virtual ~CCamera(void);
	//////////////////////////////////////////////////////////////////////////
	void SetPosition(const D3DXVECTOR3 &pos) {m_pos = pos;}
	void SetVecLook(const D3DXVECTOR3 &look) {m_look = look;}
	void SetVecUp(const D3DXVECTOR3 &up) {m_up = up;}

	D3DXVECTOR3 GetPosition()const {return m_pos;}
	D3DXVECTOR3 GetVecLook()const {return m_look;}
	D3DXVECTOR3 GetVecUp()const {return m_up;}
	//////////////////////////////////////////////////////////////////////////
	void SetCamera(IDirect3DDevice9 *pDevice);
protected:
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_look;
	D3DXVECTOR3 m_up;
};

