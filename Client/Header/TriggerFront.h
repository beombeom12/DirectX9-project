#pragma once
#include "PSystem.h"


class CTriggerFront : public CPSystem
{
protected:
	explicit CTriggerFront(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTriggerFront();

public:
	virtual		HRESULT		Ready_Object(void);
	virtual		_int		Update_Object(const _float& fTimeDelta);
	virtual		void		LateUpdate_Object(void);
	virtual		void		Render_Object(void);

	void					Set_PclePos(_vec3 _vPos)
	{
		m_vTriggerFrontPos = _vPos;
	}
	void					Set_PcleDir(_vec3 _vDir)
	{
		m_vDir = _vDir;
	}

public:
	HRESULT				Add_Component(void);
	static	 CTriggerFront*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual		void				Free(void);


public:
	virtual		void		resetParticle(ATTRIBUTE* attribute);

private:
	_float		m_fGravity = 0.f;
	_float		m_fSpeed = 0.f;
	
	_vec3		m_vDir = { 0.f,0.f,0.f };
	_vec3		m_vTriggerFrontPos = { 0.f,0.f,0.f };



};

