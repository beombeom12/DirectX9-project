#pragma once
#include "Weapon.h"
#include "Wall.h"
#include "ShotgunPart1.h"
#include "ShotgunPart2.h"
#include "ShotgunPart3.h"
#include "ShotgunPart4.h"
#include "ShotgunPart5.h"
#include "Export_Function.h"
USING(Engine)

class CShotgun : public CWeapon
{
private:
	explicit CShotgun(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CShotgun();

public:
	void			Set_ShotGun(void) { m_bEquiped = true; }
	void			Off_ShotGun(void) { m_bEquiped = false; }
	_bool			Get_State(void) { return m_bEquiped; }
private:
	_bool			m_bEquiped = false;

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual void	Render_Object(void) override;

private:
	HRESULT			Add_Component(void);
	HRESULT			Get_Parts(void);
	void			Set_OnTerrain(void);

	void			Assemble(void);

	void			Animation_Fire(void);

	void			TransAxisShotgun(void);

public:
	static CShotgun*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void Free(void);
};



