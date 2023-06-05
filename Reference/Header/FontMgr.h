#pragma once

#include "Base.h"
#include "Engine_Include.h"
#include "MyFont.h"

BEGIN(Engine)

class ENGINE_DLL CFontMgr :public CBase
{
	DECLARE_SINGLETON(CFontMgr)

private:
	explicit CFontMgr();
	virtual ~CFontMgr();

public:
	HRESULT			Ready_Font(LPDIRECT3DDEVICE9 pGraphicDev,
		const _tchar* pFontTag,	 // 키값
		const _tchar* pFontType, // 서체
		const _uint& iWidth,
		const _uint& iHeight,
		const _uint& iWeight);

	void	Render_Font(const _tchar* pFontTag,
		const _tchar* pString,
		const _vec2* pPos,
		D3DXCOLOR Color);
private:
	CMyFont*		Find_Font(const _tchar* pFontTag);

private:
	map<const _tchar*, CMyFont*>		m_mapFont;

private:
	virtual void	Free(void);
};

END