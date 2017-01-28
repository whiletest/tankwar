#pragma once

#include "vehicle_4tyre.h"

class CBullet;

class CTank : public CVehicle_4Tyre
{
public:
	enum
	{
		PART_START = CVehicle_4Tyre::PART_END,
		PART_CAP = CTank::PART_START,
		PART_GUN,
		PART_END
	};
public:
	CTank(const SOBJ_INIT &sobjInit, UINT id = SOBJ_TANK);
	virtual ~CTank(void);
	//////////////////////////////////////////////////////////////////////////
	virtual void Setup(IDirect3DDevice9 *pDevice);
	virtual UINT ChangeCurState(UINT cst);
	virtual bool OnTerrian(Terrain *pTerra, ON_TERRAIN *otInfo);
	virtual void Display(IDirect3DDevice9 *pDevice, float timeDelta);
	//////////////////////////////////////////////////////////////////////////
	CBullet* Fire(IDirect3DDevice9 *pDevice);
	D3DXVECTOR3 GetMuzzleDir()const;
	D3DXVECTOR3 GetCapPosition()const;
	//////////////////////////////////////////////////////////////////////////
	static float GetCapAngleDelta() {return m_capAngleDelta;}
protected:
	void FindGunPosition(IDirect3DDevice9 *pDevice,float angleDelta);
	void GunRotation(IDirect3DDevice9 *pDevice);
	D3DXVECTOR3 GetMuzzlePosition()const; //获取炮口位置
	void RotateMuzzleDir(float angleDelta); //旋转炮口朝向向量
protected:
	float m_fireFreq;   //发射频率(两发炮弹应间隔时间,单位s)
	float m_fireTimeDelta; //记录上一发炮弹发射到现在的时间
	float m_capCurAngle; //坦克顶转动夹角 
	static const float m_capAngleDelta;
};

