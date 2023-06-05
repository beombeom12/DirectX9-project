#pragma once
#include "GameObject.h"
class CGunUI : public CGameObject
{
private:
	explicit CGunUI(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CGunUI();

public:
	HRESULT Ready_Object(void) override;
	_int   Update_Object(const _float& fTimeDelta) override;
	void   LateUpdate_Object(void) override;
	void   Render_Object(void) override;

private:
	HRESULT Add_Component();

private:
	CRcTex*   m_pBufferCom = nullptr;
	CTransform* m_pTransCom = nullptr;
	CTexture*		  m_pTextureCom = nullptr;
	CShader*		m_pShaderCom = nullptr;

	CGameObject*	  m_pPlayer = nullptr;

	_matrix           m_ProjMatrix;
	_float            m_fX, m_fY, m_fSizeX, m_fSizeY;
	_int m_iGunIndex = 0;

public:
	static CGunUI*      Create(LPDIRECT3DDEVICE9 pGraphicDev);
	void Free(void) override;
};
