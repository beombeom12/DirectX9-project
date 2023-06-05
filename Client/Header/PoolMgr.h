#pragma once

#include "GameObject.h"

class CPoolMgr
{
	DECLARE_SINGLETON(CPoolMgr)

private:
	explicit CPoolMgr();
	virtual ~CPoolMgr();

public:
	void Collect_Obj(CGameObject* pObj);
	void Collect_PlayerBullet(CGameObject* pObj);
	void Collect_ExBullet(CGameObject* pObj);
	void Collect_KraKenBullet(CGameObject* pObj);
	void Collect_Laser(CGameObject* pObj);

	HRESULT Reuse_Obj(LPDIRECT3DDEVICE9& pGraphicDev, const _vec3* vPos, const _vec3* vDir, _float _fDamage);
	HRESULT Reuse_PlayerBullet(LPDIRECT3DDEVICE9& pGraphicDev, const _vec3* vPos, const _vec3* vDir, _float _fDamage, _float _fSpeed);
	HRESULT Reuse_ExBullet(LPDIRECT3DDEVICE9& pGraphicDev, const _vec3* vPos, const _vec3* vDir, _float _fSpeed);
	HRESULT Reuse_KrakenBullet(LPDIRECT3DDEVICE9& pGraphicDev, const _vec3* vPos, const _vec3* vDir, _float _fSpeed, _float fDamage);
	HRESULT Reuse_Laser(LPDIRECT3DDEVICE9& pGraphicDev, const _vec3* vPos, const _vec3* vDir, _float _fSpeed, _int _iIndex);

private:
	list<CGameObject*>		m_ObjectPool;
	list<CGameObject*>		m_PlayerBulletPool;
	list<CGameObject*>		m_ExBulletPool;
	list < CGameObject*>	m_KrakenBulletPool;
	list<CGameObject*>      m_LaserPool;

	int						m_iCount = 0;

private:
	list<TCHAR*>		m_liBulletName;
	_uint				m_iCnt = 0;
	_uint				m_iBulletCnt = 0;

public:
	virtual	void Free();

private:
	virtual CComponent* Clone() { return nullptr; }
};
