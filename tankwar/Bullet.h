#pragma once

#include "SenceObj.h"


class CBullet : public CSenceObj
{
public:
	CBullet(const SOBJ_INIT &sobjInit, const D3DXVECTOR3 &dir, UINT id = SOBJ_BULLET);
	virtual ~CBullet(void);
	//////////////////////////////////////////////////////////////////////////
	virtual bool IsHitObj(const CSenceObj *pObj) = 0;
protected:
	virtual void Yaw(){}
protected:
};

