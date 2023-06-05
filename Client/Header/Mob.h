#pragma once
#include "Monster.h"

class ComboUI;

class CMob :
	public CMonster
{
public:
	explicit CMob(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMob();


public:
	virtual		HRESULT		Ready_Object(const _vec3& vPos, _tchar* Name);
	virtual		_int		Update_Object(const _float& fTimeDelta);
	virtual		void		LateUpdate_Object(void);
	virtual		void		Render_Object(void);


	void			Look_Direction(void);
private:
	HRESULT				Add_Component(void);
	HRESULT				Create_Item(void);
	void				Walk_Animation_Run(void);
	void				Idle_Animation_Run(void);
	void				Attack_Animation_Run(void);

	HRESULT				Build(void);
	void				Load_Animation(wstring FileName, _uint AnimationID);
	void				Run_Animation(const _float& AnimationSpeed);



private:

	CCubeCol*			m_pAnimationBox = nullptr;
	CComboUI*			m_pComboUI = nullptr;
	list<_tchar*>		m_TcharList;

	_tchar*				m_Monstername;
	MOBSTATEID			m_STATE;
	MOBSTATEID			m_BeforeState;
	MOBWALKID			m_WALK;
	MOBIDLEID			m_IDLE;
	MOBATTACKID			m_ATTACK;
	
	_tchar*			m_MonsterName;
	_bool			m_bFirst = true;
	CLayer*			pMyLayer;
	_float			m_fTimeDelta;
	_float			m_AnimationTime;
public:
	static CMob*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos, _tchar* Name);
	virtual void	Free(void);
};

