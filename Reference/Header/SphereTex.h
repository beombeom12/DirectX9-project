#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CSphereTex : public CVIBuffer
{
private:
	explicit CSphereTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CSphereTex(const CSphereTex& rhs);
	virtual ~CSphereTex();

public:
	virtual HRESULT Ready_Buffer(_float _radius);
	virtual void Render_Buffer(void) override;

public:
	static CSphereTex*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _float _radius);
	CComponent*	Clone(void);
	virtual void Free(void) override;
};

END