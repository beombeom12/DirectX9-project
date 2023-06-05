#pragma once
#include "Monster.h"
class CDeadParticle;
class CAlien :
	public CMonster
{
public:
	explicit CAlien(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CAlien();

public:
	virtual		HRESULT		Ready_Object(const _vec3& vPos, _tchar* Name);
	virtual		_int		Update_Object(const _float& fTimeDelta);
	virtual		void		LateUpdate_Object(void);
	virtual		void		Render_Object(void);

private:
	HRESULT				Add_Component(void);
	HRESULT				Create_Item(void);
	void				Look_Direction(void);
	void				Dead_Effect(void);
	_float				m_fTimeDelta = 0.f;
	_float				m_fFrame = 0.f;
private:
	ALIENSTATEID		m_STATE;
	ALIENSTATEID		m_BeforeState;
	ALIENWALKID			m_WALK;
	ALIENIDLEID			m_IDLE;
	ALIENATTACKID		m_ATTACK;
	ALIENSHOTTINGID		m_SHOT;
	_tchar*				m_MonsterName;
	CLayer*				pMyLayer = nullptr;
	_float				m_AnimationTime = 0.f;
	_bool				m_bFirst = true;

private:

	void				Walk_Animation_Run(void);
	void				Idle_Animation_Run(void);
	void				Attack_Animation_Run(void);

	HRESULT				Build(void);
	void				Load_Animation(wstring FileName, _uint AnimationID);
	void				Run_Animation(const _float& AnimationSpeed);

	
	CDeadParticle*	    m_pDeadParticle = nullptr;
	CCubeCol*			m_pAnimationBox = nullptr;
	list<_tchar*>		m_TcharList;


public:
	static CAlien*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos, _tchar* Name);
	virtual void	Free(void);


};

