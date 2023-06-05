#pragma once
#include "GameObject.h"
#include "Engine_Include.h"
BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRenderer;
class CCalculator;

END

class CLoadingBar :	public CGameObject
{
public:
	explicit CLoadingBar(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLoadingBar(void);

public:
	virtual HRESULT			 Ready_Object() override;
	virtual _int			 Update_Object(const _float& fTimeDelta) override;
	virtual	void			LateUpdate_Object(void) override;
	virtual void Render_Object(void) override;

private:
	HRESULT		Add_Component(void);
	void		Begin_OrthoProj();
	void		End_OrthoProj();
	void		Begin_HudOrthoProj();
	void		End_HudOrthoProj();

public:
	void		Set_Min(_float Min) {m_fMin = Min; }
	void		Set_Max(_float Max) { m_fMax = Max; }

private:
	CRcTex*			m_pBufferUICom = nullptr;
	CTransform*		m_pUITransCom = nullptr;
	CTexture*		m_pUITextureCom = nullptr;

	CRcTex*			m_pHUDBufferCom = nullptr;
	CTransform*		m_pHUDTransCom = nullptr;
	CTexture*		m_pHUDTextureCom = nullptr;

	CShader*		m_pShaderCom = nullptr;

	_matrix m_matProj;

	_float  m_fX, m_fY, m_fSizeX, m_fSizeY;
	_float  m_fSizeHUDX, m_fSizeHUDY;

	_float	m_fMin = 0;
	_float	m_fMax = 0;

public:
	static CLoadingBar*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void Free(void);
};

