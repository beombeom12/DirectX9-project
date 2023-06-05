#pragma once
#include "PSystem.h"

class CSparkEffect : public CPSystem
{
protected:
	explicit CSparkEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSparkEffect();

public:
	virtual		HRESULT		Ready_Object(void);
	virtual		_int		Update_Object(const _float& fTimeDelta);
	virtual		void		LateUpdate_Object(void);
	virtual		void		Render_Object(void);
	void					Set_PclePos(_vec3 _vPos) { vSparkEffectPos = _vPos; }

public:
	HRESULT					Add_Component(void);
	static CSparkEffect*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual		void		Free(void);

private:
	_vec3					vSparkEffectPos = { 0.f, 0.f, 0.f };

public:
	virtual		void		resetParticle(ATTRIBUTE* attribute);
};

