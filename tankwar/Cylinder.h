#pragma once

#include "shape.h"

class CCylinder : public CShape
{
public:
	CCylinder(float nr = 0.4f, float pr = 0.4f, float len =0.5f, UINT slices = 10U, UINT stacks = 10U);
	virtual ~CCylinder(void);
	//////////////////////////////////////////////////////////////////////////
	float GetLength()const {return m_length;}
	float GetNegativeZRadius()const {return m_nRadius;}
	float GetPositiveZRadius()const {return m_pRadius;}
	//////////////////////////////////////////////////////////////////////////
	virtual bool Setup(IDirect3DDevice9 *pDevice);
protected:
	float m_nRadius; //negative z
	float m_pRadius; //positive z
	float m_length;
	UINT m_slices;
	UINT m_stacks;
};

