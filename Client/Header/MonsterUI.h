#pragma once
#include "GameObject.h"
class CMonsterUI :
	public CGameObject
{
public:
	CMonsterUI(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMonsterUI();

public:
	HRESULT Ready_Object(void) override;
	_int	Update_Object(const _float& fTimeDelta) override;
	void	LateUpdate_Object(void) override;
	void	Render_Object(void) override;

public:
	void	Set_Hp(_float _hp) { m_fHp = _hp; }
	void	Set_MaxHp(_float _maxHp) { m_fMaxHp = _maxHp; }
	void	Set_Name(wstring& _strName) { m_strObjName = _strName; }

	void	On_Switch() { m_bSwitch = true; }
	void	Off_Switch() { m_bSwitch = false; }

private:
	HRESULT Add_Component();

private:
	CRcTex*		m_pBufferCom = nullptr;
	CTransform* m_pTransCom = nullptr;
	CTexture*	m_pTextureCom = nullptr;
	CShader*	m_pShaderCom = nullptr;
	CRcTex*		m_pHUDBufferCom = nullptr;
	CTransform* m_pHUDTransCom = nullptr;
	CTexture*	m_pHUDTextureCom = nullptr;

	_matrix m_matProj;

	CGameObject* m_pPlayer = nullptr;
	
	_float	m_fHp = 0.f;
	_float  m_fMaxHp = 0.f;

	wstring m_strMonsterUI = L"";
	wstring m_strObjName = L"";
	wstring m_strHp = L"";
	wstring m_strMaxHp = L"";

	_float  m_fX, m_fY, m_fSizeX, m_fSizeY;
	_float  m_fSizeHUDX, m_fSizeHUDY;

	_bool	m_bSwitch = false;

public:
	static CMonsterUI*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	void Free(void) override;
};

