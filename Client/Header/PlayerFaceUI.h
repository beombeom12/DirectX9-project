#pragma once
#include "GameObject.h"

class CPlayerFaceUI : public CGameObject
{
private:
	explicit CPlayerFaceUI(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPlayerFaceUI();

public:
	HRESULT Ready_Object(void) override;
	_int Update_Object(const _float& fTimeDelta) override;
	void LateUpdate_Object(void) override;
	void Render_Object(void) override;

private:
	HRESULT Add_Component();

private:
	CGameObject* m_pPlayer = nullptr;

	CRcTex*     m_pBufferCom = nullptr;
	CTransform* m_pTransCom = nullptr;
	CShader*	m_pShaderCom = nullptr;

	CTexture*   m_pTexture_100 = nullptr;
	CTexture*   m_pTexture_75 = nullptr;
	CTexture*   m_pTexture_50 = nullptr;
	CTexture*   m_pTexture_25 = nullptr;
	CTexture*   m_pTexture_0 = nullptr;

	_float		m_fFrame = 0.f;
	_matrix     m_ProjMatrix;
	_float      m_fX, m_fY, m_fSizeX, m_fSizeY;

public:
	static CPlayerFaceUI*      Create(LPDIRECT3DDEVICE9 pGraphicDev);
	void Free(void) override;
};
