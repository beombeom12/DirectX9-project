#pragma once
#include "Trap.h"
class CTerret :
	public CTrap
{
public:
	explicit CTerret(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTerret();

public:
	virtual HRESULT		Ready_Object(const _vec3& vPos, _tchar* Name);
	virtual _int		Update_Object(const _float& fTimeDelta);
	virtual void		LateUpdate_Object(void);
	virtual void		Render_Object(void);

private:
	HRESULT				Add_Component(void);
	void				HitCheck(_float _deltaTime);
	void				Look_Direction(void);
private:
	HRESULT				Build(void);
	void				Load_Animation(wstring FileName, _uint AnimationID);
	void				Run_Animation(const _float& AnimationSpeed);

	void				Shoot_Animation_Run(void);
	void				Idle_Animation_Run(void);

private:
	_tchar*				m_TrapName;
	CLayer*				pMyLayer;
	
	
	_float				m_fTimeDelta = 0.f;
	_float				m_AnimationTime = 0.f;
	_float				m_fUISwitchTime = 0.f;
	_float				m_fFrame = 0.f;
	_bool				m_bFirst = true;
	_vec3				vUIPos;
	TERRETSTATEID		m_STATE;
	TERRETIDLEID		m_IDLE;
	TERRETATTACKID		m_ATTACK;



	TERRETABILITY*		m_tAbility;

public:
	static CTerret* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos, _tchar* Name);
	virtual void	Free(void)override;

};

