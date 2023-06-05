#pragma once
#include "GameObject.h"

class CLetterBox;

class CQuest :	public CGameObject
{
private:
	explicit	CQuest(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual		~CQuest();

public:
	HRESULT Ready_Object(void) override;
	_int   Update_Object(const _float& fTimeDelta) override;
	void   LateUpdate_Object(void) override;
	void   Render_Object(void) override;

private:
	void	Key_Input();
	void	ProjectionEffect();
	HRESULT Add_Component();

	CLetterBox* m_pLetterBox = nullptr;
	
private:
	CRcTex*			m_pBufferCom = nullptr;
	CTransform*		m_pTransCom = nullptr;
	CTexture*		m_pTextureCom = nullptr;
	CShader*		m_pShaderCom = nullptr;

	_matrix          m_ProjMatrix;
	_float           m_fX, m_fY, m_fSizeX, m_fSizeY;
	CGameObject*	 m_pPlayer = nullptr;

	_bool			m_bQuest1 = false;
	_bool			m_bQuest2 = false;
	_bool			m_bQuest3 = false;

	_bool			m_bSwitch = false;

	_int			m_iWeapon = 0;
	_int			m_iKey = 0;
	_int			m_iMiddle = 0;

	wstring			 m_strWeapon = L"";
	wstring			 m_strKey = L"";
	wstring			 m_strQuest3 = L"";

public:
	static CQuest*      Create(LPDIRECT3DDEVICE9 pGraphicDev);
	void Free(void) override;
};

