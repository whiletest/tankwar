#include "StdAfx.h"
#include "Cylinder.h"


CCylinder::CCylinder(float nr /* = 0.4f */, 
	float pr /* = 0.4f */,
	float len /* =0.5f */,
	UINT slices /* = 10U */, 
	UINT stacks /* = 10U */
	):m_slices(slices),
	m_stacks(stacks),
	m_nRadius(nr),
	m_pRadius(pr),
	m_length(len)
{
}


CCylinder::~CCylinder(void)
{
}

bool CCylinder::Setup(IDirect3DDevice9 *pDevice)
{
	ID3DXMesh *pMesh;
	ID3DXBuffer *pAdjBuff;
	HRESULT hr = D3DXCreateCylinder(pDevice,m_nRadius,
		m_pRadius,m_length,m_slices,m_stacks,&pMesh,&pAdjBuff);
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
	m_vecMaterial.push_back(CShape::RED_MTRL);
	m_vecTexture.push_back(0);

	DWORD numMaxFaces = m_pMesh->GetMaxFaces();
	m_pMesh->SetNumFaces(numMaxFaces);

	return true;
}