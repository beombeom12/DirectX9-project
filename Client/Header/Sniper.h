#pragma once
#include "Weapon.h"
#include "Wall.h"
#include "SniperPart1.h"
#include "SniperPart2.h"
#include "SniperPart3.h"
#include "SniperPart4.h"
#include "Export_Function.h"
USING(Engine)

class CSniper : public CWeapon
{
private:
	explicit CSniper(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSniper();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual void	Render_Object(void) override;

public:
	void			Set_Sniper(void) { m_bEquiped = true; }
	void			Off_Sniper(void) { m_bEquiped = false; }
	_bool			Get_State(void) { return m_bEquiped; }
private:
	_bool			m_bEquiped = false;

private:
	HRESULT			Add_Component(void);
	HRESULT			Add_Parts();
	HRESULT			Get_Parts(void);
	void			Set_OnTerrain(void);						//	지형 타기

	void			Assemble(void);								//	몸체 조립

	void			Animation_Fire(void);						//	걷는 모션

	void			TransAxis_Sniper(void);

public:
	static CSniper*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void Free(void);
};



