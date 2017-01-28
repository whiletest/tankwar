#pragma once

#include "shape.h"

class CSphere : public CShape
{
public:
	CSphere(float radius = 0.5f, UINT slices = 10U, UINT stacks = 10U);
	virtual ~CSphere(void);

	float GetRadius()const {return m_radius;}
	//////////////////////////////////////////////////////////////////////////
	virtual bool Setup(IDirect3DDevice9 *pDevice);
protected:
	FLOAT m_radius;
	UINT m_slices;
	UINT m_stacks;
};

