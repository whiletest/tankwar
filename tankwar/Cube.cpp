#include "StdAfx.h"
#include "Cube.h"


CCube::CCube(float width /* = 1.0f */, 
	float height /* = 1.0f */, 
	float depth /* = 2.0f */):m_width(width),m_height(height),m_depth(depth)
{
}


CCube::~CCube(void)
{
}

bool CCube::Setup(IDirect3DDevice9 *pDevice)
{
	ID3DXMesh *pMesh;
	ID3DXBuffer *pAdjBuff;
	HRESULT hr = D3DXCreateBox(pDevice,m_width,m_height,m_depth,&pMesh,&pAdjBuff);
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
