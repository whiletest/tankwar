#pragma once

#include "senceobj.h"

class CVehicle_4Tyre : public CSenceObj //4�ֽ�ͨ����
{
//#define PART_NUM 5
public:
	enum 
	{
		PART_START = 0,
		PART_BODY = PART_START, //����
		PART_TRF, //��ǰ��
		PART_TRB, //�Һ���
		PART_TLF, //��ǰ��
		PART_TLB, //�����
		PART_END //����
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
	float m_tyreCurAngle; //��̥��ת���Ļ��� ��m_dir�ļн�
	static const float m_tyreAngleDelta; //��̥ת������
	static const float m_tyreAngleLimit; // ����,ֻ����ǰ��̥
};

