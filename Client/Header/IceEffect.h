#pragma once
#include "PSystem.h"

class CIceEffect : public CPSystem
{
protected:
	explicit CIceEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CIceEffect();

public:
	virtual		HRESULT		Ready_Object(void);
	virtual		_int		Update_Object(const _float& fTimeDelta);
	virtual		void		LateUpdate_Object(void);
	virtual		void		Render_Object(void);
	void					Set_PclePos(_vec3 _vPos) { vIceEffectPos = _vPos; }

public:
	HRESULT					Add_Component(void);
	static CIceEffect*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual		void		Free(void);

private:
	_vec3					vIceEffectPos = { 0.f, 0.f, 0.f };

public:
	virtual		void		resetParticle(ATTRIBUTE* attribute);
};

