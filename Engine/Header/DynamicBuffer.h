#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CDynamicBuffer :	public CVIBuffer
{
private:
	explicit CDynamicBuffer(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CDynamicBuffer(const CDynamicBuffer& rhs);
	virtual ~CDynamicBuffer();

public:
	HRESULT Ready_Buffer(const _tchar* _path, _float fCellSize, _float fUVX = 1.f, _float fUVY = 1.f);
	virtual void Render_Buffer() override;

public:
	static CDynamicBuffer* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* _path, _float fCellSize, _float fUVX = 1.f, _float fUVY = 1.f);
	CComponent* Clone();
	virtual void Free() override;
};

END