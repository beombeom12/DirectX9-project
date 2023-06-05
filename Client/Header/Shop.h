#pragma once
#include "GameObject.h"
#include "Engine_Include.h"


BEGIN(Engine)

class CTransform;
class CTexture;
class CRcTex;


class CShop : public CGameObject
{
	enum WEAPONBUTTON {BUTTON_ONE, BUTTON_TWO, BUTTON_THREE, BUTTON_END };

	enum UPGRADE {UPGRADE_UZI, UPGRADE_SHOTGUN, UPGRADE_SNIPER, UPGRADE_END};

	enum LEVLEUP {LEVEL_LASER, LEVEL_HEAVY, LEVEL_RAIL, LEVEL_BURST, LEVEL_UPGRADETEXT, LEVEL_END };

public:
	explicit CShop(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CShop();

public:
	HRESULT Ready_Object(void) override;
	_int	Update_Object(const _float& fTimeDelta) override;
	void	LateUpdate_Object() override;
	void	Render_Object() override;

	void		Render_Ortho(CTransform* pTransform, CTexture* pTexture, _int iIndex = 0);

	void		Set_Switch() { m_bShopSwitch = !m_bShopSwitch; }

	_bool		Get_Switch(void) { return m_bShopSwitch; }

private:
	HRESULT			Add_Component(void);
	_bool			PointMouse(const _vec3& vPos);		
	_bool			PointTest(const _vec3& vPos);

private:
	 _vec3		m_vCenterPos;		
	 _vec3		m_vPos;					
	 _vec3		m_vPos_ButtonOne;
	 _vec3		m_vPos_ButtonTwo;
	 _vec3		m_vPos_ButtonThree;
	 _vec3		m_vPos_ButtonFour;
	 _vec3		m_vPos_ButtonFive;
	 _vec3		m_vPos_ButtonSix;
	 _vec3		m_vPos_ButtonSeven;
	 _vec3		m_vPos_Uzi;
	 _vec3		m_vPos_UpgradeTexture;
	 
	 _matrix	m_matUziview;
	 _matrix	m_matShotGunview;
	 _matrix	m_matSniperview;
	 _matrix	m_matWorld;
	 _matrix	m_matView;
	 _matrix	m_matProj;

	 _matrix	m_matButtonOneView;
	 _matrix	m_matButtonTwoView;
	 _matrix	m_matButtonThreeView;
	 _matrix	m_bChekingUzi;
	 
	 _matrix	m_matLaserView;
	 _matrix	m_matHeavyView;
	 _matrix	m_matRailGunView;
	 _matrix	m_matBurstView;
	 _matrix	m_matUpgradeString;

	 _vec2		m_Pos;

	 _int		m_iIndex;
	  _int		m_iLetterCnt = 0;

	 wstring	 m_pInformation;
	 wstring	 m_pUziRaffle;
	 wstring	 m_pShotGun;
	 wstring	 m_pSniper;
	 wstring	 m_pUpgrade;
	 wstring	 m_pLaser;
	 wstring	 m_pHeavy;
	 wstring	 m_pRailGun;
	 wstring	 m_pBurst;

	 
	 //enum
	 UPGRADE	  m_eUpgrade;
	 WEAPONBUTTON m_eWeaponButton;
	 LEVLEUP	  m_eLevelUP;

private:
	Engine::CTransform* m_TranformCom = nullptr;

	Engine::CTransform* m_pUziTransformCom = nullptr;
	Engine::CTransform* m_pShotGunformCom = nullptr;
	Engine::CTransform* m_pSniperformCom = nullptr;
	//버튼
	Engine::CTransform* m_pButtonTransformCom = nullptr;
	Engine::CTransform* m_pShotGunTransformCom = nullptr;
	Engine::CTransform* m_pSniperTransformCom = nullptr;
	Engine::CTransform* m_pLaserTransformCom = nullptr;
	Engine::CTransform* m_pHeavyTransformCom = nullptr;
	Engine::CTransform* m_pRailGunTransformCom = nullptr;
	Engine::CTransform* m_pBurstModeTransformCom = nullptr;
	Engine::CTransform* m_pUpgradeTransformCom = nullptr;
	Engine::CRcTex*		m_pBufferCom = nullptr;
	//button
	Engine::CTexture*	m_pButtonOneform = nullptr;
	Engine::CTexture*	m_pButtonTwoform = nullptr;
	Engine::CTexture*	m_pButtonThreeform = nullptr;
	//Gun
	Engine::CTexture*	m_pTextureform = nullptr;
	Engine::CTexture*	m_pUziTextureform = nullptr;
	Engine::CTexture*	m_pShotGunTextureform = nullptr;
	Engine::CTexture*	m_pSniperTextureform = nullptr;

	//Upgrade
	Engine::CTexture*	m_pLaserRaffle = nullptr;
	Engine::CTexture*	m_pHeavyRaffle = nullptr;
	Engine::CTexture*	m_pRailGunScope = nullptr;
	Engine::CTexture*	m_pBurstMode = nullptr;
	Engine::CTexture*	m_pUpgradeString = nullptr;

	CShader*	m_pShaderCom = nullptr;

	CGameObject* m_pPlayer = nullptr;

	_int		m_iGunIndex;
	_bool		m_bCheckUzi = false;
	_float		fScale;
	_float		fScale2;
	//버튼
	_float		fScale3;
	_float		fScale4;
	_float		fScale5;
	//levelup
	_float		fScale6;
	_float		fScale7;
	_float		fScale8;
	_float		fScale9;
	_float		fScale10 = 2.f;
	_float		fScale11;
	_float		fScale12;
	_float		fScale13;

	_bool		m_bShopSwitch = false;

public:
	static CShop*	Create(LPDIRECT3DDEVICE9 pGraphicDev);

	void		Free()	override;

};

END