#pragma once
#include "Weapon.h"
#include "Wall.h"
#include "UziPart1.h"
#include "UziPart2.h"
#include "UziPart3.h"
#include "UziPart4.h"
#include "UziPart5.h"
#include "Export_Function.h"
USING(Engine)

class CUzi : public CWeapon
{
private:
	explicit CUzi(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CUzi();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual void	Render_Object(void) override;

public:
	void			Set_Uzi(void) { m_bEquiped = true; }
	void			Off_Uzi(void) { m_bEquiped = false; }


	_bool			Get_State(void) { return m_bEquiped; }
	void			Get_UziUpgrade() { m_tAbility->fBulletAttack += 10; }

private:
	_bool			m_bEquiped = false;

private:
	void			TransAxisUzi1(void);
	void			TransAxisUzi2(void);

	HRESULT			Add_Component(void);
	HRESULT			Get_Parts(void);

	void			Assemble(void);
	void			Animation_Fire(void);
	
private:
	CTransform*		m_pUziPart1 = nullptr;
	CTransform*		m_pUziPart2 = nullptr;
	CTransform*		m_pUziPart3 = nullptr;
	CTransform*		m_pUziPart4 = nullptr;
	CTransform*		m_pUziPart5 = nullptr;

	CCalculator*	m_pCalculatorCom = nullptr;
	CCubeTex*		m_pBufferCom = nullptr;
	CTexture*		m_pTextureCom = nullptr;

	_float			m_fTimeDelta = 0.f;
	_float			m_fUnit = 0.1f;

	_float			A1 = 0.f;
	_float			A2 = 0.f;
	_float			B1 = 0.f;
	_float			B2 = 0.f;
	_float			C1 = 0.f;
	_float			C2 = 0.f;
	_float			D1 = 0.f;
	_float			D2 = 0.f;

public:
	static CUzi*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void Free(void);
};



