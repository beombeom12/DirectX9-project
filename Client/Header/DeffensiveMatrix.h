#pragma once
#include "GameObject.h"

class CDeffensiveMatrix :
	public CGameObject
{
private:
	explicit CDeffensiveMatrix(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CDeffensiveMatrix();


public:
	virtual		HRESULT		Ready_Object(const _vec3& Position);
	virtual		_int		Update_Object(const _float& fTimeDelta);
	virtual		void		LateUpdate_Object(void);
	virtual		void		Render_Object(void);

private:
	HRESULT					Add_Component(void);
	void					Scaling();
	void					Dead();
	void					DeadParticle();
public:
	static	CDeffensiveMatrix*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& Position);
	virtual		void			Free(void);

private:
	CTransform*				m_pTransCom = nullptr;
	CTransform*				m_pHitBoxTransCom = nullptr;
	CTexture*				m_pTexture = nullptr;
	CCubeTex*				m_pCube = nullptr;
	CCollision*				m_pCollision = nullptr;
	CHitBox*				m_pHitBox = nullptr;

private:
	_vec3					m_vPos = { 0.f,0.f,0.f };
	_vec3					m_vDir = { 0.f,0.f,0.f };
	_vec3					m_vScale = { 0.f,0.f,0.f };
	_bool					m_bMove = false;
	_bool					m_bShield = false;
	_float					m_fSpeed = 1.f;
	_float					m_fTimer = 0.f;
	_float					m_fDuration = 0.f;
	_int iT = 0;
};

