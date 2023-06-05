#pragma once
#include "GameObject.h"


class CStaticCamera;
class CKrakenHit;
class CKrakenBullet :
	public CGameObject
{
public:
	CKrakenBullet(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CKrakenBullet();

public:
	virtual		HRESULT		Ready_Object(const _vec3 * vPos, const _vec3 * vDir, _float _fSpeed, _float _fDamage);
	virtual		_int		Update_Object(const _float& fTimeDelta);
	virtual		void		LateUpdate_Object(void);
	virtual		void		Render_Object(void);

private:
	HRESULT					Add_Component(void);

public:
	void			Set_Pos(const _vec3& vPos);
	void			Set_Dir(const _vec3& vDir) { m_vDir = vDir; }
	void			Restore(void) { m_bDead = false, m_fTimeDelta = 0.f; }
	void			Collision_Check(void);

	void			Kraken_BulletParticle(void);



protected:
	CTransform*			m_pKrakenBullet;
	CSphereTex*				m_pSphereTex;
	CTexture*				m_pTexture = nullptr;

	CTransform*				m_pHitBoxCom;
	CHitBox*				m_pHitBox;
	CCollision*				m_pCollision = nullptr;
	CTransform*			m_pPlayerTransCom = nullptr;
	CKrakenHit*			m_pKrakenHit = nullptr;
private:
	_tchar*					m_KrakenInk;
	_vec3					m_vDir = { 0.f, 0.f, 0.f };
	_vec3					m_vPos;
	_float					m_fSpeed = 10.f;

	_float				m_fTimeDelta = 0.f;
	_bool				m_bDamage = false;
	_float				m_fDamage = 0.f;
	CStaticCamera*		m_pStaticCam = nullptr;
	CGameObject*		m_pPlayer = nullptr;
	CGameObject*		m_pHitBarUI = nullptr;
public:
	static CKrakenBullet*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 * vPos, const _vec3 * vDir, _float _fSpeed, _float _fDamage);
	virtual void	Free(void);
};
