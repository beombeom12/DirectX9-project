#pragma once
#include "Monster.h"

BEGIN(Engine)

class CHitBox;
class CTransform;
class CCalculator;
class CCollision;
class CCubeCol;
class RcEffect;
END

class CDeadParticle;
class CIllusioner :	public CMonster
{
public:
	CIllusioner(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CIllusioner();

public:
	virtual		HRESULT		Ready_Object(const _vec3& vPos, _tchar* Name);
	virtual		_int		Update_Object(const _float& fTimeDelta);
	virtual		void		LateUpdate_Object(void);
	virtual		void		Render_Object(void);

private:
	HRESULT				Add_Component(void);
	HRESULT				Create_Item();
	void				Sound();
	void				Dead_Effect(void);
	CCubeCol*			m_pAnimationBox = nullptr;
	_float				m_fFrame = 0.f;

private:	//	애니메이션 관련
	HRESULT				Build(void);
	void				Load_Animation(wstring FileName, _uint AnimationID);
	void				Run_Animation(const _float& AnimationSpeed);
	void				Look_Direction(void);

	_float				m_fTimeDelta = 0.f;
	_bool				m_bFirst = true;
	_float				m_AnimationTime = 0.f;

	void				Walk_Animation_Run(void);
	void				Idle_Animation_Run(void);
	void				Attack_Animation_Run(void);

	_bool				m_bCreate = false;

	ILLUSIONSTATEID			m_STATE;
	ILLUSIONWALKID			m_WALK;
	ILLUSIONIDLEID			m_IDLE;
	ILLUSIONATTACKID		m_ATTACK;
	CGameObject*				m_pEffect = nullptr;
	CLayer*					pMyLayer;
	_tchar*					m_MonsterName;
	_bool					m_bRun = false;
	CGameObject*			m_pPlayerEffect = nullptr;
	CDeadParticle*			m_pDeadParticle = nullptr;

	_float			m_fPosX = 0.f;
	_float			m_fPosY = 0.f;
	_float			m_fPosZ = 0.f;


	list<_tchar*>		m_TcharList;

public:
	static CIllusioner*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos, _tchar* Name);
	virtual void	Free(void);

};

