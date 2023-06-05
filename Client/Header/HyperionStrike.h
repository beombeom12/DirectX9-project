#pragma once
#include "GameObject.h"

class CBattleCursier;

class CHyperionStrike :
	public CGameObject
{
private:
	explicit CHyperionStrike(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CHyperionStrike();
public:
	virtual		HRESULT		Ready_Object(const _vec3& Position, DIRRECTIONSTATE _eDir = DIR_END);
	virtual		_int		Update_Object(const _float& fTimeDelta);
	virtual		void		LateUpdate_Object(void);
	virtual		void		Render_Object(void);

private:
	HRESULT					Add_Component(void);
	void					Recall_BattleCruiser(void);
	void					Bullet_Rain(void);
	void					Move_Location(void);
	void					Effect(void);
private:
	CTransform*				m_pTransCom = nullptr;
	CSphereTex*				m_pSphereTex = nullptr;
	CSphereTex*				m_pSphereTex2 = nullptr;

	CTexture*				m_pTexture = nullptr;
	CGameObject*			m_pEffect = nullptr;
	CGameObject*			m_pGameObject = nullptr;
	CBattleCursier*			m_pBattleCruiser = nullptr;


	_float					m_fAge = 0.f;
	_float					m_fBlinkTimer = 0.f;
	_float					m_fDuration = 0.f;

	_vec3					m_vPos = { 0.f,0.f,0.f };
	_vec3					m_vBattlePos = { 0.f, 0.f, 0.f };
	_vec3					m_vDirection = { 0.f,0.f,0.f };
	_float					m_fSpeed = 0.f;
	_float					m_fBulletTimer = 0.f;
	_bool					m_bMove = false;
	DIRRECTIONSTATE			m_eDir = DIR_END;
public:
	static CHyperionStrike*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& Position, DIRRECTIONSTATE _eDir = DIR_END);
	virtual void	Free(void);
};

