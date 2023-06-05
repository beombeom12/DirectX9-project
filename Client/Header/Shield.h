#pragma once
#include "GameObject.h"

class CDeffensiveMatrix;
class CShield :
	public CGameObject
{
private:
	explicit CShield(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CShield();

public:
	virtual		HRESULT		Ready_Object(const _vec3& Position, const _vec3& Direction);
	virtual		_int		Update_Object(const _float& fTimeDelta);
	virtual		void		LateUpdate_Object(void);
	virtual		void		Render_Object(void);

	void					On_Move(void) { m_bMove = true; }
	void					Off_Move(void) { m_bMove = false; }
	

private:
	HRESULT					Add_Component(void);
	void					Move(void);
	void					DefensiveMatrix(void);
	

public:
	static CShield*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& Position, const _vec3& Direction);
	virtual void	Free(void);

private:
	CTransform*				m_pTransCom = nullptr;
	CTexture*				m_pTexture = nullptr;
	CCubeTex*				m_pCube = nullptr;
	CCollision*				m_pCollision = nullptr;
	CHitBox*				m_pHitBox = nullptr;
	CDeffensiveMatrix*		m_pDeffensiveMatrix = nullptr;
	CGameObject*			m_pEffect = nullptr;


private:
	_vec3					m_vPos = { 0.f,0.f,0.f };
	_vec3					m_vDir = { 0.f,0.f,0.f };
	_vec3					m_vScale = { 0.f,0.f,0.f };
	_bool					m_bMove = false;
	_bool					m_bShield = false;
	_float					m_fSpeed = 1.f;
};

