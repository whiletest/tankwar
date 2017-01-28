#pragma once

#include <vector>
#include <cmath>

class CShape
{
	typedef std::vector<IDirect3DTexture9*>::iterator TEX_ITER;
	struct ROT_ANGLE //旋转角度
	{
		float _rx; //x轴旋转
		float _ry; //y轴旋转
		float _rz; //z轴旋转
		ROT_ANGLE(float x = 0.0f, float y = 0.0f ,float z = 0.0f):_rx(x),_ry(y),_rz(z){}
	};
	typedef ROT_ANGLE ROT_SPEED; //旋转速度
	typedef ROT_ANGLE SCALE_XYZ; //拉伸缩放
public:
	CShape(void);
	virtual ~CShape(void);
	//////////////////////////////////////////////////////////////////////////inline
	void SetPosition(const D3DXVECTOR3 &pos) {m_pos = pos;}
	void SetPosition(float x, float y, float z) {m_pos.x = x;m_pos.y = y;m_pos.z = z;}
	D3DXVECTOR3 GetPosition()const {return m_pos;}
	////
	void SetDirection(const D3DXVECTOR3 &dir) {m_dir = dir;}
	void SetDirection(float x, float y, float z) {m_dir.x = x;m_dir.y = y;m_dir.z = z;}
	D3DXVECTOR3 GetDirection()const {return m_dir;}
	////
	void SetRotationX(float angle = 0.0f) {m_rotation._rx = angle;}
	void SetRotationY(float angle = 0.0f) {m_rotation._ry = angle;}
	void SetRotationZ(float angle = 0.0f) {m_rotation._rz = angle;}
	////
	void GetRotationMatrixX(D3DXMATRIX *pR) 
	{D3DXMatrixRotationX(pR,m_rotation._rx);}

	void GetRotationMatrixY(D3DXMATRIX *pR) 
	{D3DXMatrixRotationY(pR,m_rotation._ry);}

	void GetRotationMatrixZ(D3DXMATRIX *pR) 
	{D3DXMatrixRotationZ(pR,m_rotation._rz);}

	void GetTranslatioMatrix(D3DXMATRIX *pT)
	{D3DXMatrixTranslation(pT, m_pos.x, m_pos.y, m_pos.z);}
	////
	void AddRotationX(float delta = 0.0f)
	{m_rotation._rx += delta; 
	if (m_rotation._rx >= 2 * PI)m_rotation._rx = 0.0f;
	else if (m_rotation._rx < 0.0f)m_rotation._rx = 2*PI;}

	void AddRotationY(float delta = 0.0f)
	{m_rotation._ry += delta; 
	if (m_rotation._ry >= 2 * PI)m_rotation._ry = 0.0f;
	else if (m_rotation._ry < 0.0f)m_rotation._ry = 2*PI;}

	void AddRotationZ(float delta = 0.0f)
	{m_rotation._rz += delta; 
	if (m_rotation._rz >= 2 * PI)m_rotation._rz = 0.0f;
	else if (m_rotation._rz < 0.0f)m_rotation._rz = 2*PI;}
	////
	void SetRotationSpeedX(float sx = 0.0f) {m_rSpeed._rx = sx;}
	void SetRotationSpeedY(float sy = 0.0f) {m_rSpeed._ry = sy;}
	void SetRotationSpeedZ(float sz = 0.0f) {m_rSpeed._rz = sz;}
	////
	void MoveOnAxis(const D3DXVECTOR3 &axis)
	{m_pos.x += axis.x;m_pos.y += axis.y;m_pos.z += axis.z;}
	////
	//////////////////////////////////////////////////////////////////////////normal
	std::vector<D3DMATERIAL9> SetMaterial(const std::vector<D3DMATERIAL9> &vecMaterial);
	void SetTexture(const std::vector<IDirect3DTexture9*> &vecTex);
	//////////////////////////////////////////////////////////////////////////virtual
	virtual bool Setup(IDirect3DDevice9 *pDevice) = 0;
	virtual void Display(IDirect3DDevice9 *pDevice, float timeDelta); //子类末尾调用此函数
public:
	static const D3DMATERIAL9 WHITE_MTRL;
	static const D3DMATERIAL9 RED_MTRL;
	static const D3DMATERIAL9 BLUE_MTRL;
	static const D3DMATERIAL9 BLACK_MTRL;
	static const D3DMATERIAL9 YELLOW_MTRL;
protected:
	ID3DXPMesh *m_pMesh; //网格
	int m_numSubset; //子集数
	D3DXVECTOR3 m_pos; //位置
	D3DXVECTOR3 m_dir; //方向
	ROT_ANGLE m_rotation; //各轴旋转角度
	ROT_SPEED m_rSpeed; //各轴旋转速度
	SCALE_XYZ m_scale; //各轴拉伸缩放
	std::vector<IDirect3DTexture9*> m_vecTexture; //纹理
	std::vector<D3DMATERIAL9> m_vecMaterial; //材质 
};

