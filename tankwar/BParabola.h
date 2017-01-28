#pragma once

#include "bullet.h"
#include "SObjIdentify.h"

//抛物线形式的炮弹
class CBParabola : public CBullet
{
	enum
	{
		PART_BEGIN = 0,
		PART_BODY = PART_BEGIN,
		PART_END
	};
public:
	CBParabola(const SOBJ_INIT &sobjInit, const D3DXVECTOR3 &dir ,float spDir);
	virtual ~CBParabola(void);
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////virtual
	virtual bool IsHitObj(const CSenceObj *pObj);
	virtual void Setup(IDirect3DDevice9 *pDevice);
	virtual UINT ChangeCurState(UINT cst);
	virtual bool OnTerrian(Terrain *pTerra, ON_TERRAIN *otInfo);
	virtual void Display(IDirect3DDevice9 *pDevice, float timeDelta);
protected:
	virtual void MoveOnDirection(float speed);
protected:
};

