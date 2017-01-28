#pragma once

#include <string>
#include <list>
#include "typedeclare.h"

class CSObjManage
{
	typedef std::list<CSenceObj*>::iterator SOBJ_ITER;
	typedef std::list<CBullet*>::iterator BULLET_ITER;
public:
	CSObjManage(std::string fileName);
	~CSObjManage(void);
	//////////////////////////////////////////////////////////////////////////
	UINT AddScenceObj(CSenceObj *pObj) {m_listObj.push_back(pObj);return m_listObj.size();}
	UINT AddBullet(CBullet *pBul) {m_listBullet.push_back(pBul);return m_listBullet.size();}
	//////////////////////////////////////////////////////////////////////////
	void DoManagement(CPlayer *player, Terrain *terr);
	void Display(IDirect3DDevice9 *pDevice, float timeDelta);
private:
	std::string m_fileName;
	std::list<CSenceObj*> m_listObj;
	std::list<CBullet*> m_listBullet;
};

