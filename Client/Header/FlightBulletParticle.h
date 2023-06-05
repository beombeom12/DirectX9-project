#pragma once
#include "PSystem.h"

class CFlightBulletParticle : public CPSystem
{
protected:
	explicit CFlightBulletParticle(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFlightBulletParticle();

public:
	virtual		HRESULT		Ready_Object(void);
	virtual		_int		Update_Object(const _float& fTimeDelta);
	virtual		void		LateUpdate_Object(void);
	virtual		void		Render_Object(void);

public:
	HRESULT								Add_Component(void);
	static CFlightBulletParticle*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual		void					Free(void);

	_int		m_iPosSet = 1;

public:
	virtual		void		resetParticle(ATTRIBUTE* attribute);
};

