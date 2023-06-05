#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CTriCol :	public CVIBuffer
{
private:
	explicit CTriCol(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTriCol(const CTriCol& rhs);
	virtual ~CTriCol();

public:
	virtual HRESULT Ready_Buffer(void) override;
	virtual void Render_Buffer(void) override;

public:
	static CTriCol*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	CComponent*		Clone(void);
	virtual void Free(void) override;

};

END