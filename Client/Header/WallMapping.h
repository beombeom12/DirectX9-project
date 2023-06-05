#pragma once
#include "Export_Function.h"
#include "BaseMapping.h"
#include "GameObject.h"

USING(Engine)


class CWallMapping :
	public CGameObject
{
private:
	explicit CWallMapping(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CWallMapping();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual void	Render_Object(void) override;
	void			Set_Texture(_int _iTexIndex) { m_iTextureIndex = _iTexIndex; }
private:
	void			Begin_OrthoProj();
	void			End_OrthoProj();
	void			Key_Input(void);

	HRESULT			Add_Component(void);

private:
	CTransform*		m_pTransform = nullptr;
	CTexture*		m_pTexture = nullptr;
	CCubeTex*		m_pCube = nullptr;
	CRcTex*			m_pRcCom = nullptr;
	CBaseMapping*	m_pBaseMapping = nullptr;

	_matrix			m_matWorld;
	_matrix			m_matView;
	_matrix			m_matProj;
	_int			m_iTextureIndex = 71;
	_bool			m_bWorldMap = false;
	_bool			m_bMinimap = false;


public:
	static CWallMapping*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);
};

