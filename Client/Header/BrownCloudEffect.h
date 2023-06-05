#pragma once
#include "PSystem.h"

class CBrownCloudEffect : public CPSystem
{
protected:
	explicit CBrownCloudEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBrownCloudEffect();

public:
	virtual		HRESULT		Ready_Object(void);
	virtual		_int		Update_Object(const _float& fTimeDelta);
	virtual		void		LateUpdate_Object(void);
	virtual		void		Render_Object(void);
	void					Set_PclePos(_vec3 _vPos) { vBrownCloudEffectPos = _vPos; }

public:
	HRESULT						Add_Component(void);
	static CBrownCloudEffect*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual		void			Free(void);

private:
	_vec3					vBrownCloudEffectPos = { 0.f, 0.f, 0.f };

public:
	virtual		void		resetParticle(ATTRIBUTE* attribute);
};

