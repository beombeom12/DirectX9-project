#pragma once
#include "Export_Function.h"
#include "BaseMapping.h"

#include "GameObject.h"

USING(Engine)

class CPlayerMapping :	public CGameObject
{
private:
	explicit CPlayerMapping(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPlayerMapping();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual void	Render_Object(void) override;

	_bool		Get_WorldMap() { return m_bWorldMap; }

private:
	HRESULT			Add_Component(void);
	void			Begin_OrthoProj();
	void			End_OrthoProj();
	void			Key_Input(const _float& fTimeDelta);

	HRESULT			Bombard(const _float& fTimeDelta);

private:
	CTransform*		m_pTransformPlayer = nullptr;
	CTransform*		m_pTransform = nullptr;
	CTexture*		m_pTexture = nullptr;
	CRcTex*			m_pBombBuffer = nullptr;
	CRcTex*			m_pBufferCom = nullptr;
	CBaseMapping*	m_pBaseMapping = nullptr;

	CTexture*		m_pBombTexure = nullptr;
	CTransform*		m_pBombTransform = nullptr;

	_bool	m_bWorldMap = false;
	_bool	m_bMinimap = false;

	_float m_fSpeed = 15.f;

	_matrix m_matWorld;
	_matrix m_matView;
	_matrix m_matProj;

	_bool	m_bBombard = false;

	_int	m_iRand = 0;

	_float	m_fFrame = 0.f;
	_float  m_fSkillFrame = 0.f;



public:
	static CPlayerMapping*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);
};

