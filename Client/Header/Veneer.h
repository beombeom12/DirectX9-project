#pragma once
#include "GameObject.h"

class CVeneer :	public CGameObject
{
private:
	explicit CVeneer(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CVeneer();

public:
	virtual HRESULT Ready_Object(const _vec3& vPos);
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual void	Render_Object(void) override;

private:
	HRESULT			Add_Component(void);

private:
	CTransform*		m_pTransform = nullptr;
	CTexture*		m_pTexture = nullptr;
	CRcTex*			m_pBufferCom = nullptr;

	_matrix m_matWorld;
	_matrix m_matView;
	_matrix m_matProj;

	_float	m_fFrame = 0.f;

public:
	static CVeneer*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos);
	virtual void		Free(void);

};

