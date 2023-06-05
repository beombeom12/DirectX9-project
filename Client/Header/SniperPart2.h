#pragma once
#include "GameObject.h"
#include "Export_Function.h"

USING(Engine)


class CSniperPart2 :
	public CGameObject
{
private:
	explicit CSniperPart2(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSniperPart2();

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
	static CSniperPart2*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);
};

