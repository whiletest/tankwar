#include "StdAfx.h"
#include "Bullet.h"


CBullet::CBullet(
	const SOBJ_INIT &sobjInit, 
	const D3DXVECTOR3 &dir,
	UINT id /* = SOBJ_BULLET */
	):CSenceObj(sobjInit, id)
{
	m_dir = dir;
	D3DXVec3Normalize(&m_dir, &m_dir);
}


CBullet::~CBullet(void)
{
}
