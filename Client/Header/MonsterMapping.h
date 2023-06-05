#pragma once
#include "Export_Function.h"
#include "BaseMapping.h"

#include "GameObject.h"

USING(Engine)


class CMonsterMapping :
	public CGameObject
{
private:
	explicit CMonsterMapping(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMonsterMapping();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual void	Render_Object(void) override;
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
	_int			m_iTextureIndex = 0;
	_bool			m_bWorldMap = false;
	_bool			m_bMinimap = false;

public:
	static CMonsterMapping*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);
};

