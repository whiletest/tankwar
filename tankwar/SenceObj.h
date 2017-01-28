#pragma once

#include "SObjIdentify.h"
#include "typedeclare.h"
#include <vector>

class CShape;
class Terrain;

class CSenceObj
{
protected:
	typedef std::vector<CShape*>::iterator COMB_ITER;
	//////////////////////////////////////////////////////////////////////////
	struct ON_TERRAIN
	{
		float delatY;
		float raDir; //rotation angle on direction
		float raUp;
		float raRight;
	};
public:
	struct SOBJ_INIT
	{
		D3DXVECTOR3 pos;
		float ra_dir, ra_up, ra_right;
		float accel_dir, accel_up, accel_right;
		float msp_dir, msp_up, msp_right;
		SOBJ_INIT()
		{
			pos.x = 0.0f;pos.y = 0.0f;pos.z = 0.0f;
			ra_dir = 0.0f;ra_up = 0.0f;ra_right = 0.0f;
			accel_dir = 0.000005f;accel_up = 0.0f;accel_right = 0.0f;
			msp_dir = 0.006f;msp_up = 0.0f;msp_right = 0.0f;
		}
	};
public:
	CSenceObj(const SOBJ_INIT &sobjInit, UINT id = SOBJ_NONE);
	virtual ~CSenceObj(void);
	//////////////////////////////////////////////////////////////////////////inline
	UINT GetIdentify()const {return m_identify;}
	UINT GetCurState()const {return m_curState;}
	float GetCurSpeedDir()const {return m_curSpeedDir;}
	float GetAccelDir()const {return m_accelDir;}
	float GetMaxSpeedDir()const {return m_maxSpeedDir;}
	D3DXVECTOR3 GetDirection()const {return m_dir;}
	D3DXVECTOR3 GetPosition()const {return m_pos;}

	void SetMaxSpeedDir(float maxSpeedDir) {m_maxSpeedDir = maxSpeedDir;}
	void SetAccelDir(float accelDir) {m_accelDir = accelDir;}
	//////////////////////////////////////////////////////////////////////////normal
	void RoateVecOnUp(float angle);
	D3DXVECTOR3 GetPartPosition(UINT index)const;
	//////////////////////////////////////////////////////////////////////////virtual
	virtual void Setup(IDirect3DDevice9 *pDevice);
	virtual UINT ChangeCurState(UINT cst);
	virtual bool OnTerrian(Terrain *pTerra,ON_TERRAIN *otInfo) = 0;
	virtual void Display(IDirect3DDevice9 *pDevice, float timeDelta) = 0;
protected:
	virtual void MoveOnDirection(float speed); //在朝向向量上移动
	virtual void Yaw() = 0;
protected:
	UINT m_identify;
	UINT m_curState;
	float m_curSpeedDir; //面向方向的速度
	float m_accelDir; //面向方向的加速度
	float m_maxSpeedDir; //面向方向的最大速度
	D3DXVECTOR3 m_pos; //整个物体位置
	D3DXVECTOR3 m_dir; //整个物体的正面朝向
	D3DXVECTOR3 m_up; //向上
	D3DXVECTOR3 m_right; //向右
	float m_objUpCurAngle; //整个物体围绕up轴已转动角度
	float m_objRightCurAngle; //整个物体围绕right轴已转动角度
	std::vector<CShape*> m_vecCombine; //网格对象的组合
};

