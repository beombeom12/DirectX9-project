#pragma once
#include "GameObject.h"
#include "Export_Function.h"

USING(Engine)


class CSniperPart4 :
	public CGameObject
{
private:
	explicit CSniperPart4(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSniperPart4();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual void	Render_Object(void) override;

private:
	HRESULT			Add_Component(void);

private:
	CTransform*		m_pTransform = nullptr;
	CTexture*		m_pTexture = nullptr;
	CCubeTex*		m_pCube = nullptr;

public:
	static CSniperPart4*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);
};

