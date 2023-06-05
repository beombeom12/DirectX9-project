#pragma once
#include "GameObject.h"

class CBulletUI : public CGameObject
{
private:
	explicit CBulletUI(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBulletUI();

public:
	HRESULT Ready_Object(void) override;
	_int   Update_Object(const _float& fTimeDelta) override;
	void   LateUpdate_Object(void) override;
	void   Render_Object(void) override;

private:
	HRESULT Add_Component();

private:
	CRcTex*			m_pBufferCom = nullptr;
	CTransform*		m_pTransCom = nullptr;
	CTexture*		m_pTextureCom = nullptr;
	CShader*		m_pShaderCom = nullptr;

	_matrix          m_ProjMatrix;
	_float           m_fX, m_fY, m_fSizeX, m_fSizeY;
	CGameObject*	 m_pPlayer = nullptr;
	_float			 m_fRemainBullet = 0;
	_float			 m_fTotalBullet = 0;
	wstring			 m_strReminaBullet = L"";
	wstring			 m_strTotalBullet = L"";

public:
	static CBulletUI*      Create(LPDIRECT3DDEVICE9 pGraphicDev);
	void Free(void) override;
};
