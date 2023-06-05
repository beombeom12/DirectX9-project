#pragma once
#include "PSystem.h"
class CMonsterParticle : public CPSystem
{
protected:
	explicit CMonsterParticle(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMonsterParticle();

public:
	virtual		HRESULT		Ready_Object(void);
	virtual		_int		Update_Object(const _float& fTimeDelta);
	virtual		void		LateUpdate_Object(void);
	virtual		void		Render_Object(void);
	void					Set_PclePos(_vec3 _vPos)
	{
		m_vHitPatriclePos = _vPos;
	}

public:
	HRESULT						Add_Component(void);
	static CMonsterParticle*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual		void			Free(void);

private:
	_vec3	m_vHitPatriclePos = { 0.f,0.f,0.f };

public:
	virtual		void		resetParticle(ATTRIBUTE* attribute);
};

