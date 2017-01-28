#pragma once

#include "shape.h"

class CCube : public CShape
{
public:
	CCube(float width = 1.0f, float height = 1.0f, float depth = 2.0f);
	virtual ~CCube(void);
	//////////////////////////////////////////////////////////////////////////inline
	float GetWidth()const {return m_width;}
	float GetHeight()const {return m_height;}
	float GetDepth()const {return m_depth;}
	//////////////////////////////////////////////////////////////////////////virtual
	virtual bool Setup(IDirect3DDevice9 *pDevice);
protected:
	float m_width;
	float m_height;
	float m_depth;
};

