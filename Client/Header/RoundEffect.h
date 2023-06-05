#pragma once
#include "PSystem.h"


class CRoundEffect : public CPSystem
{
protected:
	explicit CRoundEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CRoundEffect();

public:
	virtual		HRESULT		Ready_Object(void);
	virtual		_int		Update_Object(const _float& fTimeDelta);
	virtual		void		LateUpdate_Object(void);
	virtual		void		Render_Object(void);

	void					Set_CenterPos(_vec3 _vCenter)
	{
		m_vCenterPos = _vCenter;
	}
	void					Set_PclePos(_vec3 _vPos)
	{
		m_vRoundEffectPos = _vPos;
	}
	void					Set_PcleDir(_vec3 _vDir)
	{
		m_vDir = _vDir;
	}

public:
	HRESULT				Add_Component(void);
	static	 CRoundEffect*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual		void				Free(void);


public:
	virtual		void		resetParticle(ATTRIBUTE* attribute);

private:
	_float		m_fGravity = 0.f;
	_float		m_fSpeed = 0.f;
	_vec3		m_vDir = { 0.f,0.f,0.f };
	_vec3		m_vRoundEffectPos = { 0.f,0.f,0.f };
	_vec3		m_vCenterPos = { 0.f,0.f,0.f };



};

