#pragma once
#include "PSystem.h"

class CCloudEffect : public CPSystem
{
protected:
	explicit CCloudEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCloudEffect();

public:
	virtual		HRESULT		Ready_Object(void);
	virtual		_int		Update_Object(const _float& fTimeDelta);
	virtual		void		LateUpdate_Object(void);
	virtual		void		Render_Object(void);
	void					Set_PclePos(_vec3 _vPos) { vCloudEffectPos = _vPos; }

public:
	HRESULT						Add_Component(void);
	static CCloudEffect*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual		void			Free(void);

private:
	_vec3					vCloudEffectPos = { 0.f, 0.f, 0.f };

public:
	virtual		void		resetParticle(ATTRIBUTE* attribute);
};

