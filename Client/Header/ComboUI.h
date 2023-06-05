#pragma once
#include "GameObject.h"
class CComboUI : public CGameObject
{
private:
	explicit CComboUI(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CComboUI();

public:
	HRESULT Ready_Object(void) override;
	_int   Update_Object(const _float& fTimeDelta) override;
	void   LateUpdate_Object(void) override;
	void   Render_Object(void) override;

	void   On_Switch() 
	{
		m_bSwitch = true; 
		m_fFrame = 2.f;
	}

	void   Off_Switch() { m_bSwitch = false; }

	_float	Get_ComboCnt() { return m_fComboCnt; }

	void   ComboCntPlus() { m_fComboCnt++; }
	void   KillCntPlus() { m_fKillCnt++; }

private:
	HRESULT Add_Component();

private:
	CRcTex*		m_pBufferCom = nullptr;
	CTransform* m_pTransCom = nullptr;
	CTexture*	m_pTextureCom = nullptr;
	CRcTex*		m_pHUDBufferCom = nullptr;
	CTransform* m_pHUDTransCom = nullptr;
	CTexture*	m_pHUDTextureCom = nullptr;
	CShader*		m_pShaderCom = nullptr;

	_matrix m_matProj;

	CGameObject* m_pPlayer = nullptr;

	wstring m_strCombo = L"";
	wstring m_strKill = L"";

	wstring m_strComboCnt = L"";
	_float m_fComboCnt = 0.f;

	wstring m_strKillCnt = L"";
	_float m_fKillCnt = 0.f;

	_float m_fFrame = 2.f;

	_float  m_fX, m_fY, m_fSizeX, m_fSizeY;
	_float  m_fSizeHUDX, m_fSizeHUDY;

	_bool	m_bSwitch = false;

public:
	static CComboUI*      Create(LPDIRECT3DDEVICE9 pGraphicDev);
	void Free(void) override;
};

