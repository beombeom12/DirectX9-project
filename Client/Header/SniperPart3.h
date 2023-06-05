#pragma once
#include "GameObject.h"
#include "Export_Function.h"

USING(Engine)


class CSniperPart3 :
	public CGameObject
{
private:
	explicit CSniperPart3(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSniperPart3();

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
	static CSniperPart3*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);
};

