#pragma once
#include "PSystem.h"

class CBulletParticle : public CPSystem
{
protected:
	explicit CBulletParticle(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBulletParticle();

public:
	virtual		HRESULT		Ready_Object(void);
	virtual		_int		Update_Object(const _float& fTimeDelta);
	virtual		void		LateUpdate_Object(void);
	virtual		void		Render_Object(void);

public:
	HRESULT						Add_Component(void);
	static CBulletParticle*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual		void			Free(void);

public:
	virtual		void		resetParticle(ATTRIBUTE* attribute);
};

