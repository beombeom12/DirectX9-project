#pragma once
#include "Export_Function.h"
#include "Weapon.h"

USING(Engine)


class CUziPart1 :
	public CWeapon
{
	private:
	explicit CUziPart1(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CUziPart1();

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
	static CUziPart1*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);
};

