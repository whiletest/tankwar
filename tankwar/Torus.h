#pragma once

#include "shape.h"

class CTorus : public CShape
{
public:
	CTorus(float innerR = 0.2f, float outerR = 0.4f, UINT sides = 8U, UINT rings = 8U);
	virtual ~CTorus(void);
	//////////////////////////////////////////////////////////////////////////inline
	float GetInnerRadius()const {return m_innerRadius;}
	float GetOuterRadius()const {return m_outerRadius;}
	//////////////////////////////////////////////////////////////////////////virual
	virtual bool Setup(IDirect3DDevice9 *pDevice);
//	virtual void Display(IDirect3DDevice9 *pDevice, float timeDelta);
protected:
	float m_innerRadius;
	float m_outerRadius;
	UINT m_sides;
	UINT m_rings;
};

