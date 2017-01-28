#include "StdAfx.h"
#include "SObjManage.h"
#include "terrain.h"
#include "Tank.h"
#include "Bullet.h"


CSObjManage::CSObjManage(std::string fileName):m_fileName(fileName)
{
}


CSObjManage::~CSObjManage(void)
{
	for (SOBJ_ITER iter = m_listObj.begin();
		iter != m_listObj.end();++iter)
	{
		delete *iter;
	}
	m_listObj.clear();

	for (BULLET_ITER iter = m_listBullet.begin();
		iter != m_listBullet.end();++iter)
	{
		delete *iter;
	}
	m_listBullet.clear();
}

void CSObjManage::DoManagement(CPlayer *player, Terrain *terr)
{
	//所有物在地上
	for (SOBJ_ITER iter = m_listObj.begin();
		iter != m_listObj.end();++iter)
	{
		(*iter)->OnTerrian(terr,0);
	}

	for (BULLET_ITER iter = m_listBullet.begin();
		iter != m_listBullet.end();++iter)
	{
		if ( (*iter)->IsHitObj(player) ) //击中玩家
		{
			//todo here...

			delete *iter;
			iter = m_listBullet.erase(iter);
			if (iter == m_listBullet.end())
				break;
		}
		else if ( (*iter)->OnTerrian(terr,0) ) //击中地面
		{
			//todo here...

			delete *iter;
			iter = m_listBullet.erase(iter);
			if (iter == m_listBullet.end())
				break;
		}
		else //击中物体
		{
			for (SOBJ_ITER objIter = m_listObj.begin();
				objIter != m_listObj.end();++objIter)
			{
				if ( (*iter)->IsHitObj(*objIter) )
				{
					//todo here...

					delete *iter;
					iter = m_listBullet.erase(iter);
					if (iter == m_listBullet.end())
						return;
					
					break;
				}
			}
		}
	}
}

void CSObjManage::Display(IDirect3DDevice9 *pDevice, float timeDelta)
{
	for (SOBJ_ITER iter = m_listObj.begin();
		iter != m_listObj.end();++iter)
	{
		(*iter)->Display(pDevice, timeDelta);
	}

	for (BULLET_ITER iter = m_listBullet.begin();
		iter != m_listBullet.end();++iter)
	{
		(*iter)->Display(pDevice, timeDelta);
	}
}