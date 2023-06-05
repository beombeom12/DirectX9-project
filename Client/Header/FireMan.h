#pragma once
#include "Monster.h"

BEGIN(Engine)

class CHitBox;
class CTransform;
class CCalculator;
class CCollision;
class CCubeCol;

END

class CDeadParticle;

class CFireMan : public CMonster
{
private:
	explicit CFireMan(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFireMan();

public:
	virtual		HRESULT		Ready_Object(const _vec3& vPos, _tchar* Name);
	virtual		_int		Update_Object(const _float& fTimeDelta);
	virtual		void		LateUpdate_Object(void);
	virtual		void		Render_Object(void);
	
private:
	HRESULT				Add_Component(void);
	HRESULT				Create_Item();
	void				Sound();
	CCubeCol*			m_pAnimationBox = nullptr;
	void				Dead_Effect();
	_float				m_fInterval = 0.f;
	


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

	FIREMANSTATEID		m_STATE;
	FIREMANSTATEID		m_BeforeState;
	FIREMANWALKID		m_WALK;
	FIREMANIDLEID		m_IDLE;
	FIREMANATTACKID		m_ATTACK;

	CLayer*				pMyLayer;
	_tchar*				m_MonsterName;
	CDeadParticle*		m_pDeadParticle = nullptr;
	list<_tchar*>		m_TcharList;

public:
	static CFireMan*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos, _tchar* Name);
	virtual void		Free(void);
};

