#include "StdAfx.h"
#include "Torus.h"


CTorus::CTorus(float innerR /* = 0.5f */, 
	float outerR /* = 1.0f */,
	UINT sides /* = 3U */,
	UINT rings /* = 3U */
	):m_innerRadius(innerR),
	m_outerRadius(outerR),
	m_sides(sides),
	m_rings(rings)
{
}


CTorus::~CTorus(void)
{
}

bool CTorus::Setup(IDirect3DDevice9 *pDevice)
{
	ID3DXMesh *pMesh;
	ID3DXBuffer *pAdjBuff;
	HRESULT hr = D3DXCreateTorus(pDevice, m_innerRadius,
		m_outerRadius, m_sides, m_rings, &pMesh, &pAdjBuff);
	if (FAILED(hr))
		return false;

	DWORD *pAdj = (DWORD*)(pAdjBuff->GetBufferPointer());
	if ( !(pMesh->GetFVF() & D3DFVF_NORMAL) )
	{
		ID3DXMesh *pTemMesh = 0;
		hr = pMesh->CloneMeshFVF(D3DXMESH_MANAGED,
			pMesh->GetFVF() | D3DFVF_NORMAL,
			pDevice,&pTemMesh);
		if (FAILED(hr))
			return false;

		hr = D3DXComputeNormals(pTemMesh,pAdj);
		if (FAILED(hr))
			return false;

		pMesh->Release();
		pMesh = pTemMesh;
	}

	hr = D3DXGeneratePMesh(pMesh,pAdj,0,0,1,D3DXMESHSIMP_FACE,&m_pMesh);
	if (FAILED(hr))
		return false;

	pMesh->Release();
	pAdj = 0;
	pAdjBuff->Release();

	m_numSubset = 1;
	m_vecMaterial.push_back(CShape::BLUE_MTRL);
	m_vecTexture.push_back(0);

	DWORD numMaxFaces = m_pMesh->GetMaxFaces();
	m_pMesh->SetNumFaces(numMaxFaces);

	return true;
}
