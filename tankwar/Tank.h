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
	D3DXVECTOR3 GetMuzzlePosition()const; //��ȡ�ڿ�λ��
	void RotateMuzzleDir(float angleDelta); //��ת�ڿڳ�������
protected:
	float m_fireFreq;   //����Ƶ��(�����ڵ�Ӧ���ʱ��,��λs)
	float m_fireTimeDelta; //��¼��һ���ڵ����䵽���ڵ�ʱ��
	float m_capCurAngle; //̹�˶�ת���н� 
	static const float m_capAngleDelta;
};

