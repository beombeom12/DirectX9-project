#pragma once
#include "GameObject.h"
class CHitBarUI : public CGameObject
{
private:
	explicit CHitBarUI(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CHitBarUI();

public:
	HRESULT Ready_Object(void) override;
	_int Update_Object(const _float& fTimeDelta) override;
	void LateUpdate_Object(void) override;
	void Render_Object(void) override;

	void OnSwitch(_float fRadian) 
	{
		m_fAngle = fRadian;
		m_bSwitch = true;
	};

	void OffSwitch()
	{
		m_bSwitch = false;
	};

private:
	HRESULT Add_Component();

private:
	CRcTex*			m_pBufferCom = nullptr;
	CTransform*		m_pTransCom = nullptr;
	CTexture*		m_pTextureCom = nullptr;
	CShader*		m_pShaderCom = nullptr;

	_bool		m_bSwitch = false;

	_float		m_fAngle = 0.f;

	_matrix            m_ProjMatrix;
	_float            m_fX, m_fY, m_fSizeX, m_fSizeY;
public:
	static CHitBarUI*      Create(LPDIRECT3DDEVICE9 pGraphicDev);
	void Free(void) override;
};

