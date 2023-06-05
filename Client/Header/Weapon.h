#pragma once
#include "GameObject.h"
#include "Export_Function.h"

USING(Engine)

class Uzi;
class Shotgun;
class Sniper;

class CWeapon :
	public CGameObject
{
protected:
	explicit CWeapon(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CWeapon();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual void	Render_Object(void) override;

public:
	GUNABILITY* Get_Ability()	  { return m_tAbility; }
	void		Set_MinusBullet() { m_tAbility->fRemainBulletCnt--; }
	_bool		Get_Shoot() { return m_tAbility->bShoot; }
	void        Set_Shoot(_bool bShoot) { m_tAbility->bShoot = bShoot; }
	
	void			Get_Bullet()
	{
		if (m_tAbility->fBulletCount >= m_tAbility->fRemainBulletCnt)
		{
			m_tAbility->fRemainBulletCnt += 10;
			if (m_tAbility->fRemainBulletCnt >= m_tAbility->fBulletCount)
				m_tAbility->fRemainBulletCnt = m_tAbility->fBulletCount;
		}
	}
	CTransform* Get_Transform() { return m_pPart1; }

public:
	HRESULT			Ready_Weapon(LPDIRECT3DDEVICE9 pGraphicDev, const _uint& iIndex);

public:
	virtual void Free(void);

protected:
	//발사
	
	//재장전
		
	//정조준

protected:	
	CTransform*		m_pTransform = nullptr;

	GUNABILITY*		m_tAbility = nullptr;

	CTransform*		m_pPart1 = nullptr;
	CTransform*		m_pPart2 = nullptr;
	CTransform*		m_pPart3 = nullptr;
	CTransform*		m_pPart4 = nullptr;
	CTransform*		m_pPart5 = nullptr;

	CCalculator*	m_pCalculatorCom = nullptr;
	CCubeTex*		m_pBufferCom = nullptr;
	CTexture*		m_pTextureCom = nullptr;
	
	_bool			m_bPartInit = false;
	_float			m_fTimeDelta = 0.f;
	_float			m_fUnit = 0.1f;

	
	list<TCHAR*>		m_liszFinalName;
	
	_float		m_fAtkSpeed;
	_float		m_fReloadSpeed;
	_float		m_fRebound;

	_int		m_iAtkDmg;
	_int		m_iBulletMax;
	_int		m_iScope;

	_float		m_fUziSize = 1.f;
	_float		m_fShotgunSize = 1.f;
	_int		m_iUziTexture = 1;
	_int		m_iShotgunTexture = 1;
	_vec3		m_vWPos;

private:
	list<CWeapon*>	m_WeaponList;
};