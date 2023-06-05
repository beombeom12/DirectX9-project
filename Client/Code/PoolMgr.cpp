#include "stdafx.h"

#include "PoolMgr.h"
#include "Export_Function.h"
#include "Bullet.h"
#include "SpBullet.h"
#include "ExBullet.h"
#include "KrakenBullet.h"
#include "Laser.h"
IMPLEMENT_SINGLETON(CPoolMgr)

CPoolMgr::CPoolMgr()
{
}

CPoolMgr::~CPoolMgr()
{
	Free();
}

void CPoolMgr::Collect_Obj(CGameObject * pObj)
{
	if (pObj == nullptr)
		return;

	m_ObjectPool.push_back(pObj);
}

void CPoolMgr::Collect_PlayerBullet(CGameObject * pObj)
{
	if (pObj == nullptr)
		return;

	m_PlayerBulletPool.push_back(pObj);
}

void CPoolMgr::Collect_ExBullet(CGameObject * pObj)
{
	if (pObj == nullptr)
		return;

	m_ExBulletPool.push_back(pObj);
}

void CPoolMgr::Collect_KraKenBullet(CGameObject * pObj)
{
	if (pObj == nullptr)
		return;

	m_KrakenBulletPool.push_back(pObj);
}

void CPoolMgr::Collect_Laser(CGameObject * pObj)
{
	if (pObj == nullptr)
		return;

	m_LaserPool.push_back(pObj);

}

HRESULT CPoolMgr::Reuse_Obj(LPDIRECT3DDEVICE9& pGraphicDev, const _vec3* vPos, const _vec3* vDir, _float _fDamage)
{
	CGameObject* pObj = nullptr;

	if (m_ObjectPool.empty())
	{
		pObj = CBullet::Create(pGraphicDev, vPos, vDir, _fDamage);
		NULL_CHECK_RETURN(pObj, E_FAIL);

		Engine::Get_Layer(STAGE_BULLET)->Add_GameList(pObj);
	}
	else
	{
		pObj = m_ObjectPool.front();

		NULL_CHECK_RETURN(pObj, E_FAIL);
		m_ObjectPool.pop_front();

		dynamic_cast<CBullet*>(pObj)->Set_Dir(*vDir);

		Engine::Get_Layer(STAGE_BULLET)->Add_GameList(pObj);
	}

	dynamic_cast<CBullet*>(pObj)->Set_Pos(*vPos);
	//dynamic_cast<CBullet*>(pObj)->MoveToDir(*vDir);

	return S_OK;
}

HRESULT CPoolMgr::Reuse_PlayerBullet(LPDIRECT3DDEVICE9 & pGraphicDev, const _vec3 * vPos, const _vec3 * vDir, _float _fDamage, _float _fSpeed)
{
	CGameObject* pObj = nullptr;

	if (m_PlayerBulletPool.empty())
	{
		pObj = CSpBullet::Create(pGraphicDev, vPos, vDir, _fDamage, _fSpeed);
		NULL_CHECK_RETURN(pObj, E_FAIL);

		Engine::Get_Layer(STAGE_BULLETPLAYER)->Add_GameList(pObj);
	}
	else
	{
		pObj = m_PlayerBulletPool.front();

		NULL_CHECK_RETURN(pObj, E_FAIL);
		m_PlayerBulletPool.pop_front();

		dynamic_cast<CSpBullet*>(pObj)->isAlive(true);
		dynamic_cast<CSpBullet*>(pObj)->Set_Dir(*vDir);

		Engine::Get_Layer(STAGE_BULLETPLAYER)->Add_GameList(pObj);
	}

	dynamic_cast<CSpBullet*>(pObj)->Set_Pos(*vPos);
	//dynamic_cast<CSpBullet*>(pObj)->MoveToDir(*vDir);

	return S_OK;
}

HRESULT CPoolMgr::Reuse_ExBullet(LPDIRECT3DDEVICE9 & pGraphicDev, const _vec3 * vPos, const _vec3 * vDir, _float _fSpeed)
{
	CGameObject* pObj = nullptr;

	if (m_ExBulletPool.empty())
	{
		pObj = CExBullet::Create(pGraphicDev, vPos, vDir, _fSpeed);
		NULL_CHECK_RETURN(pObj, E_FAIL);

		Engine::Get_Layer(STAGE_EXBULLET)->Add_GameList(pObj);
	}
	else
	{
		pObj = m_ExBulletPool.front();

		NULL_CHECK_RETURN(pObj, E_FAIL);
		m_ExBulletPool.pop_front();

		dynamic_cast<CExBullet*>(pObj)->Set_Pos(*vPos);
		dynamic_cast<CExBullet*>(pObj)->Set_Dir(*vDir);
		//dynamic_cast<CExBullet*>(pObj)->MoveToDir(*vDir);
		dynamic_cast<CExBullet*>(pObj)->Restore();

		Engine::Get_Layer(STAGE_EXBULLET)->Add_GameList(pObj);
	}

	return S_OK;
}

HRESULT CPoolMgr::Reuse_KrakenBullet(LPDIRECT3DDEVICE9 & pGraphicDev, const _vec3 * vPos, const _vec3 * vDir, _float _fSpeed, _float _fDamage)
{

	CGameObject* pObj = nullptr;

	if (m_KrakenBulletPool.empty())
	{
		pObj = CKrakenBullet::Create(pGraphicDev, vPos, vDir, _fSpeed, _fDamage);
		NULL_CHECK_RETURN(pObj, E_FAIL);

		Engine::Get_Layer(STAGE_KRAKENBULLET)->Add_GameList(pObj);
	}
	else
	{
		pObj = m_KrakenBulletPool.front();

		NULL_CHECK_RETURN(pObj, E_FAIL);
		m_KrakenBulletPool.pop_front();

		dynamic_cast<CKrakenBullet*>(pObj)->Restore();

		Engine::Get_Layer(STAGE_KRAKENBULLET)->Add_GameList(pObj);

		dynamic_cast<CKrakenBullet*>(pObj)->Set_Pos(*vPos);
		dynamic_cast<CKrakenBullet*>(pObj)->Set_Dir(*vDir);
	}

	return S_OK;
}

HRESULT CPoolMgr::Reuse_Laser(LPDIRECT3DDEVICE9 & pGraphicDev, const _vec3 * vPos, const _vec3 * vDir, _float _fSpeed, _int _iIndex)
{
	CGameObject* pObj = nullptr;

	if (m_LaserPool.empty())
	{
		pObj = CLaser::Create(pGraphicDev, vPos, vDir, _fSpeed, _iIndex);
		NULL_CHECK_RETURN(pObj, E_FAIL);

		Engine::Get_Layer(STAGE_LASER)->Add_GameList(pObj);
	}
	else
	{
		pObj = m_LaserPool.front();

		NULL_CHECK_RETURN(pObj, E_FAIL);
		m_LaserPool.pop_front();

		dynamic_cast<CLaser*>(pObj)->Set_Pos(*vPos);
		dynamic_cast<CLaser*>(pObj)->Set_Dir(*vDir);
		dynamic_cast<CLaser*>(pObj)->Restore();

		Engine::Get_Layer(STAGE_LASER)->Add_GameList(pObj);
	}

	return S_OK;

}

void CPoolMgr::Free()
{
	for (auto& iter : m_ObjectPool)
	{
		Safe_Release<CGameObject*>(iter);
	}

	for (auto& iter : m_PlayerBulletPool)
	{
		Safe_Release<CGameObject*>(iter);
	}

	for (auto& iter : m_ExBulletPool)
	{
		Safe_Release<CGameObject*>(iter);
	}

	for (auto& iter : m_KrakenBulletPool)
	{
		Safe_Release<CGameObject*>(iter);
	}

	for (auto& iter : m_LaserPool)
	{
		Safe_Release<CGameObject*>(iter);
	}

	m_ObjectPool.clear();
}