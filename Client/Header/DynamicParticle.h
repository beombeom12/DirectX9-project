#pragma once
#include "PSystem.h"
class CDynamicParticle : public CPSystem
{
protected:
	explicit CDynamicParticle(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CDynamicParticle();

public:
	virtual		HRESULT		Ready_Object(void);
	virtual		_int		Update_Object(const _float& fTimeDelta);
	virtual		void		LateUpdate_Object(void);
	virtual		void		Render_Object(void);
	virtual		HRESULT		Add_Component(void);
	void					Set_PclePos(_vec3 _vPos)
	{
		m_vDynamicPatriclePos = _vPos;
	}

public:
	static CDynamicParticle*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual		void			Free(void);

protected:
	_vec3	m_vDynamicPatriclePos = { 0.f,0.f,0.f };
	_int	m_iTextureIndex = 0;
	_float  m_fTextureTime = 0;
public:
	virtual		void		resetParticle(ATTRIBUTE* attribute);
};

