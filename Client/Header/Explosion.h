#pragma once
#include "GameObject.h"

class CExplosion : public CGameObject
{
private:
	explicit CExplosion(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CExplosion();

public:
	virtual		HRESULT		Ready_Object(const _vec3& vPos, _tchar* Name);
	virtual		_int		Update_Object(const _float& fTimeDelta);
	virtual		void		LateUpdate_Object(void);
	virtual		void		Render_Object(void);

private:
	HRESULT					Add_Component(void);
	CTransform*				m_pTransCom = nullptr;
	CCollision*				m_pCollision = nullptr;

	CTransform*				m_pPlayerTransCom = nullptr;

private:
	void					Hit_Check_Player(void);
	void					Hit_Check_Monster(void);

private:
	HRESULT					Build(void);
	void					Load_Animation(wstring FileName, _uint AnimationID);
	void					Run_Animation(const _float& AnimationSpeed);

	void					Explosion_Animation_Run(void);

	_bool					m_bFirst = true;
	CLayer*					pMyLayer;
	_tchar*					m_AnimationName;
	list<_tchar*>			m_TcharList;
	_float					m_fTimeDelta = 0.f;
	_float					m_AnimationTime = 0.f;

	EXPLOSIONID				m_ANIMATION;

public:
	static CExplosion*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos, _tchar* Name);
	virtual void			Free(void);
};

