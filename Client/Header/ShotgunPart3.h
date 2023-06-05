#pragma once
#include "Weapon.h"
#include "Export_Function.h"

USING(Engine)


class CShotgunPart3 :
	public CWeapon
{
private:
	explicit CShotgunPart3(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CShotgunPart3();

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
	static CShotgunPart3*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);
};

