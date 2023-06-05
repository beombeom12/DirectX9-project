#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer : public CComponent
{
protected:
	explicit CVIBuffer(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CVIBuffer(const CVIBuffer& rhs);
	virtual ~CVIBuffer();

public:
	virtual HRESULT		Ready_Buffer(void);
	virtual void		Render_Buffer(void);

protected:

	LPDIRECT3DVERTEXBUFFER9		m_pVB;		// 정점들을 보관하고 접근하기 위한 컴객체
	LPDIRECT3DINDEXBUFFER9		m_pIB;		// 정점들의 인덱스 값을 보관하기 위한 컴객체

	_ulong						m_dwVtxCnt;			// 버텍스 개수
	_ulong						m_dwVtxSize;		// 버텍스 크기
	_ulong						m_dwFVF;			// 버텍스의 속성 정보를 저장하는 옵션
	_ulong						m_dwTriCnt;			// 삼각형의 개수

	_ulong						m_dwIdxSize;
	D3DFORMAT					m_IdxFmt;
	
public:
	virtual void		Free(void);

};

END