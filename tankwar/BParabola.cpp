#include "StdAfx.h"
#include "BParabola.h"
#include "terrain.h"
#include "Sphere.h"

const float g_GavityAccel = 0.00002f; //重力加速度

CBParabola::CBParabola(
	const SOBJ_INIT &sobjInit,
	const D3DXVECTOR3 &dir,
	float spDir
	):CBullet(sobjInit, dir)
{
	m_curSpeedDir = spDir;
}


CBParabola::~CBParabola(void)
{
}

bool CBParabola::IsHitObj(const CSenceObj *pObj)
{
	return false;
}

void CBParabola::Setup(IDirect3DDevice9 *pDevice)
{
	CShape *psh = new CSphere;
	psh->SetPosition(m_pos);
	m_vecCombine.push_back(psh);
	psh->Setup(pDevice);
}

UINT CBParabola::ChangeCurState(UINT cst)
{
	return CSenceObj::ChangeCurState(cst);
}

bool CBParabola::OnTerrian(Terrain *pTerra, ON_TERRAIN *otInfo)
{
	return false;
}

void CBParabola::Display(IDirect3DDevice9 *pDevice, float timeDelta)
{
	D3DXMATRIX Rx, Ry, Rz, trans;
	m_vecCombine[PART_BODY]->GetRotationMatrixX(&Rx);
	m_vecCombine[PART_BODY]->GetRotationMatrixY(&Ry);
	m_vecCombine[PART_BODY]->GetRotationMatrixZ(&Rz);

	m_vecCombine[PART_BODY]->GetTranslatioMatrix(&trans);

	D3DXMATRIX comb = Rx * Ry * Rz * trans;

	pDevice->SetTransform(D3DTS_WORLD, &comb);
	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_vecCombine[PART_BODY]->Display(pDevice, timeDelta);

	//
	MoveOnDirection(0.0f);
}

void CBParabola::MoveOnDirection(float speed)
{
	float Vx = m_curSpeedDir * m_dir.x; //x方向分量的速度
	float Vy = m_curSpeedDir * m_dir.y; //y方向分量的速度
	float Vz = m_curSpeedDir * m_dir.z; //z方向分量的速度

	Vy -= g_GavityAccel;

	if (IS_UP(m_curState) && Vy <= 0.0f)
	{
		RESET_UP_FALL(m_curState);
		SET_FALL(m_curState);
	}

	//计算速度方向
	m_dir.x = Vx;
	m_dir.y = Vy;
	m_dir.z = Vz;
	D3DXVec3Normalize(&m_dir, &m_dir);

	//计算合速度
	m_curSpeedDir = sqrtf((Vx*Vx + Vy*Vy + Vz*Vz));

	//炮弹旋转
	m_vecCombine[PART_BODY]->AddRotationX(Vx*ANGLE_SCALE_ONE);
	m_vecCombine[PART_BODY]->AddRotationY(Vy*ANGLE_SCALE_ONE);
	m_vecCombine[PART_BODY]->AddRotationZ(Vz*ANGLE_SCALE_ONE);

	CSenceObj::MoveOnDirection(m_curSpeedDir);

	m_pos = m_vecCombine[PART_BODY]->GetPosition();
}