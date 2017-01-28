#include "StdAfx.h"
#include "SenceObj.h"
#include "shape.h"


CSenceObj::CSenceObj(
	const SOBJ_INIT &sobjInit,
	UINT id /* = SOBJ_NONE */
	):m_identify(id),
	m_curState(ST_STATIC),
	m_pos(sobjInit.pos),
	m_curSpeedDir(0.0f),
	m_objUpCurAngle(sobjInit.ra_up),m_objRightCurAngle(sobjInit.ra_right),
	m_maxSpeedDir(sobjInit.msp_dir),
	m_accelDir(sobjInit.accel_dir)
{
	m_dir.x = 0.0f;m_dir.y = 0.0f;m_dir.z = 1.0f;
	m_up.x = 0.0f;m_up.y = 1.0f;m_up.z = 0.0f;
	m_right.x = 1.0f;m_right.y = 0.0f,m_right.z = 0.0f;
}


CSenceObj::~CSenceObj(void)
{
	for (COMB_ITER iter = m_vecCombine.begin();
		iter != m_vecCombine.end();++iter)
	{
		if (*iter)
			delete *iter;
	}
	m_vecCombine.clear();
}

void CSenceObj::RoateVecOnUp(float angle)
{
	D3DXMATRIX RUp;
	D3DXMatrixRotationAxis(&RUp,&m_up,angle);

	D3DXVec3TransformNormal(&m_dir,&m_dir,&RUp);
	D3DXVec3TransformNormal(&m_right,&m_right,&RUp);
}

D3DXVECTOR3 CSenceObj::GetPartPosition(UINT index)const
{
	return m_vecCombine[index]->GetPosition();
}

UINT CSenceObj::ChangeCurState(UINT cst)
{
	UINT prevState = m_curState;

	switch (cst)
	{
	case CST_FB_STATIC:
		RESET_FORWARD_BACKWARD(m_curState);
		RESET_FB_SPEED_DOWN(m_curState);
		break;
	case CST_FB_SPEED_DOWN:
		if (!IS_FB_SPEED_DOWN(m_curState))
		{
			SET_FB_SPEED_DOWN(m_curState);
		}
		break;
	case CST_FORWARD:
		if (IS_FB_SPEED_DOWN(m_curState))
		{
			RESET_FB_SPEED_DOWN(m_curState);
		}
		if (!IS_FORWARD(m_curState))
		{
			RESET_FORWARD_BACKWARD(m_curState);
			SET_FORWARD(m_curState);
		}
		break;
	case CST_BACKWARD:
		if (IS_FB_SPEED_DOWN(m_curState))
		{
			RESET_FB_SPEED_DOWN(m_curState);
		}
		if (!IS_BACKWARD(m_curState))
		{
			RESET_FORWARD_BACKWARD(m_curState);
			SET_BAKCKWARD(m_curState);
		}
		break;
	default:
		break;
	}

	return prevState;
}

void CSenceObj::MoveOnDirection(float speed)
{
	D3DXVECTOR3 vec = m_dir * speed;
	for (COMB_ITER iter = m_vecCombine.begin();
		iter != m_vecCombine.end();++iter)
		if (*iter)
			(*iter)->MoveOnAxis(vec);
}


void CSenceObj::Setup(IDirect3DDevice9 *pDevice)
{
	for (COMB_ITER iter = m_vecCombine.begin();
		iter != m_vecCombine.end();++iter)
	{
		if (*iter)
			(*iter)->Setup(pDevice);
	}
}
