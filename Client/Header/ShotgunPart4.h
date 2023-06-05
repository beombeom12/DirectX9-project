#pragma once
#include "Weapon.h"
#include "Export_Function.h"

USING(Engine)


class CShotgunPart4 :
	public CWeapon
{
private:
	explicit CShotgunPart4(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CShotgunPart4();

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
	static CShotgunPart4*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);
};

