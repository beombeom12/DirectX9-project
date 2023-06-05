#pragma once
#include "Trap.h"
class CMagma : public CTrap
{
private:
	explicit CMagma(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMagma();

public:
	virtual HRESULT		Ready_Object(const _vec3& vPos, _tchar* Name);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

private:
	HRESULT				Add_Component(void);
	void				HitCheck(void);

private:	//	애니메이션 관련
	HRESULT				Build(void);
	void				Load_Animation(wstring FileName, _uint AnimationID);
	void				Run_Animation(const _float& AnimationSpeed);

	void				Up_Animation_Run(void);
	void				Down_Animation_Run(void);

	_float				m_fTimeDelta = 0.f;
	_bool				m_bFirst = true;
	_float				m_AnimationTime = 0.f;
	_bool				m_bStateChange = false;

	MAGMASTATEID		m_STATE;

	CLayer*				pMyLayer;
	_tchar*				m_TrapName;

public:
	static CMagma*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos, _tchar* Name);
	virtual void	Free(void)override;
};

