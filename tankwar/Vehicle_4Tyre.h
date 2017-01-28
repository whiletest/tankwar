#pragma once

#include "senceobj.h"

class CVehicle_4Tyre : public CSenceObj //4轮交通工具
{
//#define PART_NUM 5
public:
	enum 
	{
		PART_START = 0,
		PART_BODY = PART_START, //车身
		PART_TRF, //右前轮
		PART_TRB, //右后轮
		PART_TLF, //左前轮
		PART_TLB, //左后轮
		PART_END //结束
	};
public:
	CVehicle_4Tyre(const SOBJ_INIT &sobjInit, UINT id = SOBJ_VEHICLE_4TYRE);
	virtual ~CVehicle_4Tyre(void);
	//////////////////////////////////////////////////////////////////////////
	float GetTyreCurAngle()const {return m_tyreCurAngle;}
	static float GetTyreAngleDelta() {return m_tyreAngleDelta;}
	static float GetTyreAngleLimit() {return m_tyreAngleLimit;}
	//////////////////////////////////////////////////////////////////////////virtual
	virtual void Setup(IDirect3DDevice9 *pDevice);
	virtual UINT ChangeCurState(UINT cst);
	virtual bool OnTerrian(Terrain *pTerra, ON_TERRAIN *otInfo);
	virtual void Display(IDirect3DDevice9 *pDevice, float timeDelta);
protected:
	virtual void MoveOnDirection(float speed);
	virtual void Yaw();
	void FindTyrePosition(IDirect3DDevice9 *pDevice, int index, float angleDelta);
protected:
	float m_tyreCurAngle; //轮胎已转动的弧度 和m_dir的夹角
	static const float m_tyreAngleDelta; //轮胎转动增量
	static const float m_tyreAngleLimit; // 弧度,只限制前轮胎
};

