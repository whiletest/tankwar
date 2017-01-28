#include "StdAfx.h"
#include "Cube.h"
#include "Torus.h"
#include <cmath>
#include "Vehicle_4Tyre.h"
#include "terrain.h"
#include "d3dUtility.h"

CVehicle_4Tyre::CVehicle_4Tyre(
	const SOBJ_INIT &sobjInit,
	UINT id /* = SOBJ_VEHICLE_4TYRE */
	):CSenceObj(sobjInit, id),m_tyreCurAngle(0.0f)
{
}

const float CVehicle_4Tyre::m_tyreAngleDelta = 0.001f;
const float CVehicle_4Tyre::m_tyreAngleLimit = PI/3.0f;

CVehicle_4Tyre::~CVehicle_4Tyre(void)
{
}

void CVehicle_4Tyre::Setup(IDirect3DDevice9 *pDevice)
{
	//车身
	CShape *pOBJ = new CCube;
	m_vecCombine.push_back(pOBJ);
	pOBJ->SetPosition(m_pos);
	D3DXVECTOR3 cubePos = m_pos;//pOBJ->GetPosition();
	float cubeWidth = ((CCube*)pOBJ)->GetWidth();
	float cubeHeight = ((CCube*)pOBJ)->GetHeight();
	float cubeDepth = ((CCube*)pOBJ)->GetDepth();

	//右前轮
	pOBJ = new CTorus;
	D3DXVECTOR3 tyrePos;
	float diffuse = ((CTorus*)pOBJ)->GetOuterRadius() - ((CTorus*)pOBJ)->GetInnerRadius();
	const float delta = 0.1f * diffuse;

	tyrePos.x = cubePos.x + cubeWidth*0.5f + diffuse + delta;
	tyrePos.y = cubePos.y - cubeHeight*0.5f;
	tyrePos.z = cubePos.z + cubeDepth*0.5f;

	pOBJ->SetPosition(tyrePos);
	pOBJ->SetRotationY(0.5f * PI);
	m_vecCombine.push_back(pOBJ);

	//右后轮
	pOBJ = new CTorus;
	//不变 tyrePos.x
	//不变 tyrePos.y 
	tyrePos.z = cubePos.z - cubeDepth*0.5f;
	
	pOBJ->SetPosition(tyrePos);
	pOBJ->SetRotationY(0.5f * PI);
	m_vecCombine.push_back(pOBJ);

	//左前轮
	pOBJ = new CTorus;
	tyrePos.x = cubePos.x - cubeWidth*0.5f - diffuse - delta;
	tyrePos.y = cubePos.y - cubeHeight*0.5f;
	tyrePos.z = cubePos.z + cubeDepth*0.5f;

	pOBJ->SetPosition(tyrePos);
	pOBJ->SetRotationY(0.5f * PI);
	m_vecCombine.push_back(pOBJ);

	//左后轮
	pOBJ = new CTorus;
	tyrePos.z = cubePos.z - cubeDepth*0.5f;

	pOBJ->SetPosition(tyrePos);
	pOBJ->SetRotationY(0.5f * PI);
	m_vecCombine.push_back(pOBJ);

	CSenceObj::Setup(pDevice);
}

void CVehicle_4Tyre::MoveOnDirection(float speed)
{
	if (IS_FB_STATIC(m_curState))
	{
		if (m_curSpeedDir != 0.0f)
			m_curSpeedDir = 0.0f;
	}
	else if (IS_FB_SPEED_DOWN(m_curState))
	{
		if (IS_FORWARD(m_curState))
		{
		 	m_curSpeedDir -= m_accelDir;
			if (m_curSpeedDir < 0.0f)
			{
				RESET_FORWARD_BACKWARD(m_curState);
				RESET_FB_SPEED_DOWN(m_curState);
			}
		}
		else if (IS_BACKWARD(m_curState))
		{
		 	m_curSpeedDir += m_accelDir;
			if (m_curSpeedDir > 0.0f)
			{
				RESET_FORWARD_BACKWARD(m_curState);
				RESET_FB_SPEED_DOWN(m_curState);
			}
		}
	}
	else if (IS_FORWARD(m_curState))
	{
		if (m_curSpeedDir < m_maxSpeedDir)
			m_curSpeedDir += m_accelDir;
	}
	else if (IS_BACKWARD(m_curState))
	{
		if (m_curSpeedDir > -m_maxSpeedDir)
			m_curSpeedDir -= m_accelDir;
	}

	for (UINT index = PART_TRF;index != CVehicle_4Tyre::PART_END;++index)
		m_vecCombine[index]->AddRotationZ(m_curSpeedDir*1.5f);

	CSenceObj::MoveOnDirection(m_curSpeedDir);

	m_pos = m_vecCombine[PART_BODY]->GetPosition();
}

UINT CVehicle_4Tyre::ChangeCurState(UINT cst)
{
	UINT prevState = CSenceObj::ChangeCurState(cst);

	switch (cst)
	{
	case CST_LR_STATIC:
		RESET_LEFT_RIGHT(m_curState);
		break;
	case CST_LEFT:
		if (!IS_LEFT(m_curState))
		{
			RESET_LEFT_RIGHT(m_curState);
			SET_LEFT(m_curState);
		}
		break;
	case CST_RIGHT:
		if (!IS_RIGHT(m_curState))
		{
			RESET_LEFT_RIGHT(m_curState);
			SET_RIGHT(m_curState);
		}
		break;
	default:
		break;
	}

	return prevState;
}

void CVehicle_4Tyre::Yaw()
{
	if (IS_LR_STATIC(m_curState))
		return;

	if (IS_LEFT(m_curState))
	{
		if (m_tyreCurAngle > -m_tyreAngleLimit)
			m_tyreCurAngle -= m_tyreAngleDelta;
	}
	else if (IS_RIGHT(m_curState))
	{
		if (m_tyreCurAngle < m_tyreAngleLimit)
			m_tyreCurAngle += m_tyreAngleDelta;
	}
}

bool CVehicle_4Tyre::OnTerrian(Terrain *pTerra, ON_TERRAIN *otInfo)
{
	D3DXVECTOR3 tyrePos;
	float terraY;
	float outerRadius;
	float delta = 0.0f;
	for (UINT index = PART_TRF;index != CVehicle_4Tyre::PART_END;++index)
	{
		tyrePos = m_vecCombine[index]->GetPosition();
		terraY = pTerra->getHeight(tyrePos.x, tyrePos.z);
		outerRadius = ((CTorus*)m_vecCombine[index])->GetOuterRadius();

		if (delta == 0.0f)
			delta = terraY + outerRadius - tyrePos.y;

		tyrePos.y = terraY + outerRadius;
		m_vecCombine[index]->SetPosition(tyrePos);
	}

	otInfo->delatY = delta;

	D3DXVECTOR3 cubePos = m_vecCombine[PART_BODY]->GetPosition();
	cubePos.y += delta;
	m_vecCombine[PART_BODY]->SetPosition(cubePos);

	return true;
}

void CVehicle_4Tyre::Display(IDirect3DDevice9 *pDevice, float timeDelta)
{
	D3DXMATRIX Rz;
	D3DXMATRIX Ry;
	D3DXMATRIX RUp;
	D3DXMATRIX trans;
	D3DXMATRIX comb;
	D3DXMatrixIdentity(&comb);

	float angleDelta = m_curSpeedDir * m_tyreCurAngle * ANGLE_SCALE_ONE;

	if (IS_FORWARD(m_curState) || IS_BACKWARD(m_curState))
	{
		m_objUpCurAngle += angleDelta;
		if (m_objUpCurAngle >= 2*PI)
			m_objUpCurAngle = 0.0f;
		else if (m_objUpCurAngle < 0.0f)
			m_objUpCurAngle = 2*PI;

		RoateVecOnUp(angleDelta);
	}
	
	//绘制车身
	D3DXMatrixRotationAxis(&RUp, &m_up, m_objUpCurAngle);
	comb *= RUp;
	m_vecCombine[PART_BODY]->GetTranslatioMatrix(&trans);
	comb *= trans;
	pDevice->SetTransform(D3DTS_WORLD, &comb);
	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_vecCombine[PART_BODY]->Display(pDevice, timeDelta);

	//绘制轮
	for (UINT index = PART_TRF;index != CVehicle_4Tyre::PART_END;++index)
	{
		m_vecCombine[index]->GetRotationMatrixZ(&Rz);
		comb = Rz;

		m_vecCombine[index]->GetRotationMatrixY(&Ry);
		comb *= Ry;

		if (index == PART_TRF || index == PART_TLF)
			D3DXMatrixRotationAxis(&RUp, &m_up, m_objUpCurAngle+m_tyreCurAngle);
		else
			D3DXMatrixRotationAxis(&RUp, &m_up, m_objUpCurAngle);
		comb *= RUp;
	
		if (IS_FORWARD(m_curState) || IS_BACKWARD(m_curState))
			FindTyrePosition(pDevice, index, angleDelta);

		m_vecCombine[index]->GetTranslatioMatrix(&trans);
		comb *= trans;
	
		pDevice->SetTransform(D3DTS_WORLD, &comb);
		std::vector<D3DMATERIAL9> vecMtrl;
		vecMtrl.push_back(CShape::BLACK_MTRL);
		m_vecCombine[index]->SetMaterial(vecMtrl);
		pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		m_vecCombine[index]->Display(pDevice, timeDelta);

		vecMtrl.clear();
		vecMtrl.push_back(CShape::BLUE_MTRL);
		m_vecCombine[index]->SetMaterial(vecMtrl);
		pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		m_vecCombine[index]->Display(pDevice, timeDelta);
	}

	//
	MoveOnDirection(0.0f);
	Yaw();
}

void CVehicle_4Tyre::FindTyrePosition(IDirect3DDevice9 *pDevice, int index, float angleDelta)
{
	D3DXMATRIX trans;
	D3DXVECTOR3 tyrePrePos = m_vecCombine[index]->GetPosition();
	D3DXVECTOR3 cubePos = m_vecCombine[PART_BODY]->GetPosition();
	D3DXVECTOR3 temPos = tyrePrePos - cubePos;
	D3DXMatrixTranslation(&trans, temPos.x, temPos.y, temPos.z);
	pDevice->SetTransform(D3DTS_WORLD, &trans);

	D3DXMATRIX RPos;
	D3DXMatrixRotationAxis(&RPos,&m_up,angleDelta);
	D3DXVec3TransformCoord(&temPos,&temPos,&RPos);
	m_vecCombine[index]->SetPosition(temPos+cubePos);
}