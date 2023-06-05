#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CRcRD : public CVIBuffer
{
private:
	explicit CRcRD(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRcRD(const CRcRD& rhs);
	virtual ~CRcRD();

public:
	virtual HRESULT Ready_Buffer(void) override;
	virtual void Render_Buffer(void) override;

public:
	static CRcRD*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	CComponent*	Clone(void);
	virtual void Free(void) override;
};

END