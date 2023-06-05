#pragma once
#include "GameObject.h"

class CCubeShop : public CGameObject
{
private:
	explicit CCubeShop(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCubeShop();

public:
	virtual		HRESULT		Ready_Object(const _vec3& vPos) ;
	virtual _int			Update_Object(const _float& fTimeDelta) override;
	virtual void			LateUpdate_Object() override;
	virtual  void			Render_Object() override;
	
private:
	HRESULT			Add_component(void);
	CTransform*		m_pTransform = nullptr;

	CTexture*		m_pTexture = nullptr;
	CCubeTex*		m_pCubeTexture = nullptr;
	HRESULT			Interact();

private:
	CGameObject*		m_pShop = nullptr;
	CGameObject*		m_pDialogBox = nullptr;
	CHitBox*			m_pHitBox = nullptr;

	_bool				m_bSwitch = false;
	_bool				m_bApproaching = false;
	_bool				m_bDialogInit = false;
	_bool				m_bShoot = false;
	wstring				m_strPressing;
	_int				m_iTexIndex = 0;

	list<TCHAR*>		m_listDialogCnt;

public:
	static CCubeShop*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos);
	virtual void Free(void);

};

