#pragma once
#include "GameObject.h"

USING(Engine)
class CInventory;
class CLetterBox;

class CItemIcon :
	public CGameObject
{
private:
	explicit CItemIcon(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CItemIcon();

public:
	virtual HRESULT Ready_Object(_int iIndex);
	virtual _int   Update_Object(const _float& fTimeDelta) override;
	virtual void   LateUpdate_Object(void) override;
	virtual void   Render_Object(void) override;


public:
	void      Maker(CItemIcon* pInstance)
	{
		Engine::Add_GameObject(STAGE_UI, pInstance, szCntName);
	}
	void      Set_block(_vec3 vBlockPos) { m_vBlockPos = vBlockPos; }
	void      Set_block(_float vBlockPosX, _float vBlockPosY, _float vBlockPosZ) { m_vBlockPos.x = vBlockPosX, m_vBlockPos.y = vBlockPosY, m_vBlockPos.z = vBlockPosZ; }
	void       On_WeaponPart() { m_bWeaponPart = true; }
	void       Off_WeaponPart() { m_bWeaponPart = false; }
	void      Cursor_fix(void) { m_bFix = true; }
	void      Cursor_free(void) { m_bFix = false; }
	_tchar*      Get_Name() { return szCntName; }
	_int      Get_iTemIdx() { return m_iNumber; }

private:
	HRESULT      Add_Component();
	void      Begin_OrthoProj();
	void      End_OrthoProj();
	void      Index();
	void      DeadIndex();
	void      FixOnCursor();
	void      IconCollision();
	void      IconTag();
private:
	CGameObject*    m_pPlayer = nullptr;
	CTransform*       m_pTransformCom = nullptr;
	CTexture*       m_pTextureCom = nullptr;
	CTexture*       m_pCursorTextureCom = nullptr;
	CShader*       m_pShaderCom = nullptr;

	CRcTex*          m_pRcTexCom = nullptr;

	list<TCHAR*>    m_listItemIcon;
	_tchar          szCntName[64];

private:
	_int         m_iIndex = 0;
	_int         m_iNumber = 0;
	_int         m_iTexIndex;
	_int         m_iTagNumber;
	_bool         m_bInit = false;
	_bool         m_bFix = false;
	_bool         m_bWeaponPart = false;
	_bool         m_bIconCollision = false;

	_vec3         m_vBlockPos;
	_vec3         m_vIconPos;
	_float         m_fImgX = 0.f;
	_float         m_fImgY = 0.f;
	_float         m_fImgZ = 0.002f;
public:
	static CItemIcon*   Create(LPDIRECT3DDEVICE9 pGraphicDev, _int iIndex);
	void            Free(void)   override;

};
