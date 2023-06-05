#pragma once
#include "ItemIcon.h"
#include "RcEffect.h"
#include "GameObject.h"
#include "Weapon.h"
USING(Engine)

class CInventory : public CGameObject
{
private:
	explicit CInventory(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CInventory();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int   Update_Object(const _float& fTimeDelta) override;
	virtual void   LateUpdate_Object(void) override;
	virtual void   Render_Object(void) override;
	_bool         Get_Switch(void) { return m_bInvSwitch; }
	_int         Get_EnforceCheck(void) { return m_iEnforceCheck; }
	_int         Get_WeaponDmg(void) { return m_iWeaponDmg; }
	_int         Get_WeaponSpeed(void) { return m_iWeaponSpeed; }
	void         Set_TagIndex(_int iIndex) { m_iTagIndex = iIndex; }
	void         Tag_On() { m_bTagSwitch = true; }
	void         Tag_Off() { m_bTagSwitch = false; }
	void         ItemCreate(_int iItemIndex)
	{
		m_bItemCreate = true;
		m_iItemIndex = iItemIndex;
	}
	void         WeaponCreate(_int iItemIndex)
	{
		m_bWeaponCreate = true;
		m_iItemIndex = iItemIndex;
	}

private:
	HRESULT      Add_Component();
	void      Begin_OrthoProj();
	void      End_OrthoProj();
	void      Key_Input();
	void      Mouse();
	void      Sorting();
	void      Equipment_Sorting();
	void      Weapon_Sorting();
	void      Enhancement();
	void      Syncronize();
	void      ItemTag();

private:
	CGameObject*      m_pPlayer = nullptr;
	CGameObject*      m_pItemIcon = nullptr;
	CItemIcon*         m_pIconGrab = nullptr;
	CTransform*         m_pTransformCom = nullptr;
	CTexture*         m_pTextureCom = nullptr;
	CRcTex*            m_pRcTexCom = nullptr;
	CWeapon*         m_pWeapon = nullptr;
	CShader*         m_pShaderCom = nullptr;

	CLetterBox*      m_pLetterBox1 = nullptr;
	CLetterBox*      m_pLetterBox2 = nullptr;
	CLetterBox*      m_pLetterBox3 = nullptr;
	CLetterBox*      m_pLetterBox4 = nullptr;
	CLetterBox*      m_pLetterBox5 = nullptr;
	CLetterBox*      m_pLetterBox6 = nullptr;
	CLetterBox*      m_pLetterBox7 = nullptr;
	CLetterBox*      m_pLetterBox8 = nullptr;

	vector<CGameObject*>  m_vecContents;
	vector<CGameObject*>  m_vecEquipments;
	vector<CGameObject*>  m_vecWeapon;
	vector<CGameObject*>  m_vecParts;
private:
	_bool            m_bInit = false;
	_bool            m_bNull = false;
	_bool            m_bInvSwitch = false;
	_bool            m_bSorting = false;
	_bool            m_bNullSorting = false;
	_bool            m_bItemCreate = false;
	_bool            m_bWeaponCreate = false;

	_bool            m_bTagSwitch = false;

	_int            m_iItemCnt = 0;
	_int            m_iWeaponCnt = 0;
	_int            m_iEnforceCheck = 0;
	_int            m_iVectorNumb = 0;
	_int            m_iWeaponNumb = 0;
	_int            m_iWeaponState = 0;
	_int            m_iWeaponSpeed = 0;
	_int            m_iWeaponDmg = 0;
	_int            m_iWeaponSpeedCnt = 0;
	_int            m_iWeaponDmgCnt = 0;
	_int            m_iItemIndex = 1;
	_int            m_iTagIndex = 0;
	_float            m_fInvPosX = 0.f;
	_float            m_fInvPosY = 0.f;

	// m_fInvPosX + WINCX / 2  인벤토리 원점의 위치

public:
	static CInventory*   Create(LPDIRECT3DDEVICE9 pGraphicDev);
	void            Free(void)   override;

};
