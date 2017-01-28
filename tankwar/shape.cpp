#include "StdAfx.h"
#include "shape.h"
#include "d3dUtility.h"


CShape::CShape(void):m_pMesh(0),m_numSubset(0),m_scale(1.0f, 1.0f, 1.0f)
{
	m_pos.x = 0.0f;m_pos.y = 0.0f;m_pos.z = 0.0f;
	m_dir.x = 0.0f;m_dir.y = 0.0f;m_dir.z = 1.0f;
}

const D3DMATERIAL9 CShape::WHITE_MTRL = {
	d3d::WHITE,d3d::WHITE,
	d3d::WHITE,d3d::WHITE,
	0.0f};
const D3DMATERIAL9 CShape::RED_MTRL = {
	d3d::RED,d3d::RED,
	d3d::RED,d3d::RED,
	0.0f};
const D3DMATERIAL9 CShape::BLUE_MTRL = {
	d3d::BLUE,d3d::BLUE,
	d3d::BLUE,d3d::BLUE,
	0.0f};
const D3DMATERIAL9 CShape::BLACK_MTRL = {
	d3d::BLACK,d3d::BLACK,
	d3d::BLACK,d3d::BLACK,
	0.0f};
const D3DMATERIAL9 CShape::YELLOW_MTRL = {
	d3d::YELLOW,d3d::YELLOW,
	d3d::YELLOW,d3d::YELLOW,
	0.0f};

CShape::~CShape(void)
{
	if (m_pMesh)
		m_pMesh->Release();

	for (TEX_ITER iter = m_vecTexture.begin();
		iter != m_vecTexture.end();++iter)
	{
		if (*iter)
			(*iter)->Release();
	}
	m_vecTexture.clear();

	m_vecMaterial.clear();
}

std::vector<D3DMATERIAL9> CShape::SetMaterial(const std::vector<D3DMATERIAL9> &vecMaterial)
{
	std::vector<D3DMATERIAL9> vecPrevMaterial;
	vecPrevMaterial.assign(m_vecMaterial.begin(), m_vecMaterial.end());
	m_vecMaterial.assign(vecMaterial.begin(), vecMaterial.end());
	return vecPrevMaterial;
}

void CShape::SetTexture(const std::vector<IDirect3DTexture9*> &vecTex)
{
	for (TEX_ITER iter = m_vecTexture.begin();
		iter != m_vecTexture.end();++iter)
		d3d::Release<IDirect3DTexture9*>(*iter);

	m_vecTexture.assign(vecTex.begin(), vecTex.end());
}

void CShape::Display(IDirect3DDevice9 *pDevice, float timeDelta)
{
	if (m_pMesh)
	{
		for (int index = 0;index != m_numSubset;++index)
		{
			pDevice->SetMaterial(&m_vecMaterial[index]);
			pDevice->SetTexture(0,m_vecTexture[index]);
			m_pMesh->DrawSubset(index);
		}
	}
}