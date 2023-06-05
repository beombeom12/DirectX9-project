#pragma once
#include "GameObject.h"

USING(Engine)
class CRcEffect;

class CRcEffect :
	public CGameObject
{
private:
	explicit CRcEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CRcEffect();

public:
	virtual HRESULT Ready_Object(_int iIndex) ;
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual void	Render_Object(void) override;


public:
	void		Maker(CRcEffect* pInstance)
	{
		Engine::Add_GameObject(STAGE_ENVIRONMENT, pInstance, szCntName);
	}

	_tchar*		Get_Name() { return szCntName; }
	_int		Get_RcEffectIdx() { return m_iNumber; }
	void		Set_EffectPos(_float fX, _float fY, _float fZ) { m_fPosX = fX, m_fPosY = fY, m_fPosZ = fZ; }
	void		Set_SingleUse(void){ m_bSingleUse = true; }
	void		Set_Scale(_float _fScale) { m_fScale = _fScale; }
	void		Dead_Timer(_float _fDeadTimer) { m_fDeadTimer = _fDeadTimer; }

private:
	HRESULT		Add_Component();
	void		Index();
	void		Effect_Change();
	void		Dead_Condition();

private:
	CGameObject*	 m_pPlayer = nullptr;
	CTransform*		 m_pTransformCom = nullptr;
	CTexture*		 m_pTextureCom = nullptr;

	CRcTex*			 m_pRcTexCom = nullptr;	

	list<TCHAR*>	 m_listRcEffect;
	_tchar			 szCntName[256];
	_tchar			 szTextureName[256];

private:
	_int			m_iIndex = 0;
	_int			m_iNumber = 0;
	_int			m_iTexIndex = 0;
	_int			m_iTexBegin = 0;
	_int			m_iTexEnd = 0;
	_int			m_iTexRecall = 0;
	_float			m_fTexTimer = 0.f;

	_float			m_fPosX = 0.f;
	_float			m_fPosY = 0.f;
	_float			m_fPosZ = 0.f;

	_float			m_fScale = 2.f;
	_float			m_fTimer = 0.f;
	_float			m_fDeadTimer = 0.f;

	_bool			m_bInit = false;
	_bool			m_bSingleUse = false;
	

public:
	static CRcEffect*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _int iIndex);
	void				Free(void)	override;

};

