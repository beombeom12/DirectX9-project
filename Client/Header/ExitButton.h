#pragma once
#include "GameObject.h"
#include "Engine_Include.h"

class CExitButton :	public CGameObject
{
public:
	CExitButton(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CExitButton();

public:
	virtual HRESULT Ready_Object()	override;
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void)	override;
	virtual void	Render_Object(void) override;

private:
	CRcTex*			m_pRcTexCom = nullptr;
	CTransform*     m_pTransformCom = nullptr;
	CTexture*		m_pTextureCom = nullptr;
	CShader*		m_pShaderCom = nullptr;

private:
	HRESULT		Add_Component(void);
	void		Begin_OrthoProj();
	void		End_OrthoProj();

private:
	_bool		Mouse_check = false;
	_bool		Checking = false;
	_bool		PointMouse(void);

	wstring		m_strEB;
	_int		m_iIndex = 0;

public:
	static CExitButton* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void);
};