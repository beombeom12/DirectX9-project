#pragma once
#include "PSystem.h"


class CMFieldEffect : public CPSystem
{
protected:
	explicit CMFieldEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMFieldEffect();

public:
	virtual		HRESULT		Ready_Object(void);
	virtual		_int		Update_Object(const _float& fTimeDelta);
	virtual		void		LateUpdate_Object(void);
	virtual		void		Render_Object(void);

	void					Set_PclePos(_vec3 _vPos)
	{
		m_vMFieldEffectPos = _vPos;
	}
	void					Set_PcleDir(_vec3 _vDir)
	{
		m_vDir = _vDir;
	}
	void					Set_PcleMoveDir(_vec3 _vDir)
	{
		m_vMoveDir = _vDir;
	}
public:
	HRESULT				Add_Component(void);
	static	 CMFieldEffect*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual		void				Free(void);


public:
	virtual		void		resetParticle(ATTRIBUTE* attribute);

private:
	_float		m_fGravity = 0.f;
	_float		m_fSpeed = 0.f;
	_vec3		m_vDir = { 0.f,0.f,0.f };
	_vec3		m_vMoveDir = { 0.f,0.f,0.f };
	_vec3		m_vMFieldEffectPos = { 0.f,0.f,0.f };



};

