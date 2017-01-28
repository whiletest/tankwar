#include "StdAfx.h"
#include "Tank.h"
#include "Cylinder.h"
#include "Cube.h"
#include "BParabola.h"


CTank::CTank(
	const SOBJ_INIT &sobjInit,
	UINT id /* = SOBJ_TANK */
	):CVehicle_4Tyre(sobjInit, id),
	m_capCurAngle(0.0f),
	m_fireFreq(1.0f),
	m_fireTimeDelta(0.0f)
{
}

const float CTank::m_capAngleDelta = 0.001f;

CTank::~CTank(void)
{
}

void CTank::Setup(IDirect3DDevice9 *pDevice)
{
	CVehicle_4Tyre::Setup(pDevice);


	CShape *pObj = new CCylinder;
	m_vecCombine.push_back(pObj);
	D3DXVECTOR3 cubePos = m_vecCombine[PART_BODY]->GetPosition();
	float cubeHeight = ((CCube*)m_vecCombine[PART_BODY])->GetHeight();
	float capLen = ((CCylinder*)pObj)->GetLength();
	D3DXVECTOR3 capPos;
	capPos.x = cubePos.x;
	capPos.y = cubePos.y + cubeHeight/2 + capLen/2;
	capPos.z = cubePos.z;
	pObj->SetPosition(capPos);
	pObj->SetRotationX(0.5f * PI);
	pObj->Setup(pDevice);
	float capRadius = (((CCylinder*)pObj)->GetNegativeZRadius()+((CCylinder*)pObj)->GetPositiveZRadius())*0.5f;

	pObj = new CCylinder(0.2f, 0.2f, 2.0f);
	m_vecCombine.push_back(pObj);
	D3DXVECTOR3 gunPos;
	float gunLen = ((CCylinder*)pObj)->GetLength();
	gunPos.x = capPos.x;
	gunPos.y = capPos.y;
	gunPos.z = capPos.z + capRadius + gunLen*0.4f;
	pObj->SetPosition(gunPos);
	pObj->Setup(pDevice);
}

UINT CTank::ChangeCurState(UINT cst)
{
	UINT prevState = CVehicle_4Tyre::ChangeCurState(cst);

	switch (cst)
	{
	case CST_GRNP_STATIC:
		RESET_GRN_GRP(m_curState);
		break;
	case CST_GR_NEGATIVE:
		if (!IS_GRN(m_curState))
		{
			RESET_GRN_GRP(m_curState);
			SET_GRN(m_curState);
		}
		break;
	case CST_GR_POSITIVE:
		if (!IS_GRP(m_curState))
		{
			RESET_GRN_GRP(m_curState);
			SET_GRP(m_curState);
		}
		break;
	}

	return prevState;
}

bool CTank::OnTerrian(Terrain *pTerra, ON_TERRAIN *otInfo)
{
	ON_TERRAIN ot;
	bool rt = CVehicle_4Tyre::OnTerrian(pTerra, &ot);

	D3DXVECTOR3 pos;
	for (UINT index = PART_START;index != PART_END;++index)
	{
		pos = m_vecCombine[index]->GetPosition();
		pos.y += ot.delatY;
		m_vecCombine[index]->SetPosition(pos);
	}

	return rt;
}

void CTank::Display(IDirect3DDevice9 *pDevice, float timeDelta)
{
	CVehicle_4Tyre::Display(pDevice, timeDelta);

	D3DXMATRIX Rx;
	D3DXMATRIX trans;
	D3DXMATRIX RUp;
	D3DXMATRIX comb;
	D3DXMatrixIdentity(&comb);

	//车顶
	m_vecCombine[PART_CAP]->GetRotationMatrixX(&Rx);
	comb *= Rx;

	D3DXMatrixRotationAxis(&RUp, &m_up, m_objUpCurAngle+m_capCurAngle);
	comb *= RUp;

	m_vecCombine[PART_CAP]->GetTranslatioMatrix(&trans);
	comb *= trans;

	pDevice->SetTransform(D3DTS_WORLD, &comb);
	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_vecCombine[PART_CAP]->Display(pDevice, timeDelta);

	//炮管
	D3DXMatrixIdentity(&comb);

	D3DXMatrixRotationAxis(&RUp, &m_up, m_objUpCurAngle+m_capCurAngle);
	comb *= RUp;

	if (IS_FORWARD(m_curState) || IS_BACKWARD(m_curState))
	{
		float angleDelta = m_curSpeedDir * m_tyreCurAngle * ANGLE_SCALE_ONE;
 		FindGunPosition(pDevice, angleDelta);
	}

	m_vecCombine[PART_GUN]->GetTranslatioMatrix(&trans);
	comb *= trans;

	pDevice->SetTransform(D3DTS_WORLD, &comb);
	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_vecCombine[PART_GUN]->Display(pDevice, timeDelta);

	//转动跑管
	GunRotation(pDevice); 
	//距离上次开火时间记录(s)
	m_fireTimeDelta += timeDelta;
}

void CTank::FindGunPosition(IDirect3DDevice9 *pDevice,float angleDelta)
{
	D3DXVECTOR3 capPos = m_vecCombine[PART_CAP]->GetPosition();
	D3DXVECTOR3 gunPos = m_vecCombine[PART_GUN]->GetPosition();
	D3DXVECTOR3 temPos = gunPos - capPos;

	D3DXMATRIX trans;
	D3DXMatrixTranslation(&trans, temPos.x, temPos.y, temPos.z);

	pDevice->SetTransform(D3DTS_WORLD, &trans);

	D3DXMATRIX RUp;
	D3DXMatrixRotationAxis(&RUp, &m_up, angleDelta);
	D3DXVec3TransformCoord(&temPos, &temPos, &RUp);
	m_vecCombine[PART_GUN]->SetPosition(temPos+capPos);

	//转动炮管的朝向向量
	D3DXVECTOR3 gunDir = m_vecCombine[PART_GUN]->GetDirection();
	D3DXVec3TransformNormal(&gunDir, &gunDir, &RUp);
	D3DXVec3Normalize(&gunDir, &gunDir);
	m_vecCombine[PART_GUN]->SetDirection(gunDir);
}

void CTank::GunRotation(IDirect3DDevice9 *pDevice)
{
	if (IS_GRPN_STATIC(m_curState))
		return;

	float tempCapAngleDelta;

	if (IS_GRP(m_curState))
	{
		m_capCurAngle += m_capAngleDelta;
		if (m_capCurAngle > 2.0f * PI)
		{
			m_capCurAngle -= 2.0f * PI;
		}

		tempCapAngleDelta = m_capAngleDelta;
	}
	else if (IS_GRN(m_curState))
	{
		m_capCurAngle -= m_capAngleDelta;
		if (m_capCurAngle < 0.0f)
		{
			m_capCurAngle += 2.0f * PI;
		}

		tempCapAngleDelta = -m_capAngleDelta;
	}

	FindGunPosition(pDevice, tempCapAngleDelta);
}

void CTank::RotateMuzzleDir(float angleDelta)
{
	D3DXVECTOR3 gunDir = m_vecCombine[PART_GUN]->GetDirection();
	D3DXMATRIX RUp;
	D3DXMatrixRotationAxis(&RUp, &m_up, angleDelta);
	D3DXVec3TransformNormal(&gunDir, &gunDir, &RUp);
	D3DXVec3Normalize(&gunDir, &gunDir);
	m_vecCombine[PART_GUN]->SetDirection(gunDir);
}

CBullet* CTank::Fire(IDirect3DDevice9 *pDevice)
{
	CBullet *pBull = 0;

	if (m_fireTimeDelta > m_fireFreq)
	{
		m_fireTimeDelta = 0.0f;

		SOBJ_INIT si;
		si.pos = GetMuzzlePosition();
		D3DXVECTOR3 dir = m_vecCombine[PART_GUN]->GetDirection();
		dir.y += PI/6.0f;
		pBull = new CBParabola(si, dir, 0.1f);
		pBull->Setup(pDevice);
	}

	return pBull;
}

//protected
D3DXVECTOR3 CTank::GetMuzzlePosition()const
{
	D3DXVECTOR3 MuzzleDir = m_vecCombine[PART_GUN]->GetDirection();
	D3DXVec3Normalize(&MuzzleDir, &MuzzleDir);

	float gunLen = ((CCylinder*)m_vecCombine[PART_GUN])->GetLength();
	D3DXVECTOR3 gunPos = m_vecCombine[PART_GUN]->GetPosition();
	D3DXVECTOR3 firePos = gunPos + MuzzleDir * gunLen * 0.5f;

	return firePos;
}

D3DXVECTOR3 CTank::GetMuzzleDir()const
{
	D3DXVECTOR3 dir = m_vecCombine[PART_GUN]->GetDirection();
	D3DXVec3Normalize(&dir, &dir);
	return dir;
}

D3DXVECTOR3 CTank::GetCapPosition()const
{
	return m_vecCombine[PART_CAP]->GetPosition();
}