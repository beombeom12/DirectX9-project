#pragma once
#include "Export_Function.h"
#include "GameObject.h"

USING(Engine)


class CBaseMapping :
	public CGameObject
{
private:
	explicit CBaseMapping(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBaseMapping();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual void	Render_Object(void) override;
	_bool			Get_Worldmap(void) { return m_bWorldmap; }
	_bool			Get_Minimap(void) { return m_bMinimap; }
	void			Switch_Worldmap(void) { m_bWorldmap = !m_bWorldmap; }
	void			Key_Input(void);

private:
	void			Begin_OrthoProj();
	void			End_OrthoProj();
	void			WorldMap(void);

	HRESULT			Add_Component(void);

private:
	CTransform*		m_pTransform = nullptr;
	CTexture*		m_pTexture = nullptr;
	CCubeTex*		m_pCube = nullptr;
	CRcTex*			m_pRcCom = nullptr;
	_matrix			m_matWorld;
	_matrix			m_matView;
	_matrix			m_matProj;
	_int			m_iTextureIndex = 0;

	_bool			m_bWorldmap = false;
	_bool			m_bMinimap = false;
	_bool			m_bKeyDown = false;
public:
	static CBaseMapping*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);
};

