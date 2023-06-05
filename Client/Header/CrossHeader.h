#pragma once
#include "GameObject.h"
class CCrossHeader :public CGameObject
{
	enum GUN_ID { GUN_RIFLE, GUN_SHOTGUN, GUN_SNIPER_ZOOMIN, GUN_SNIPER_ZOOMOUT, GUN_END };

private:
	explicit CCrossHeader(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCrossHeader();

public:
	HRESULT Ready_Object(void) override;
	_int	Update_Object(const _float& fTimeDelta) override;
	void	LateUpdate_Object(void) override;
	void	Render_Object(void) override;

private:
	void		Begin_OrthoProj(GUN_ID eID);
	void		End_OrthoProj();

private:
	HRESULT Add_Component();

private:
	CRcTex*	m_pBufferCom = nullptr;
	CTransform* m_pTransCom = nullptr;
	
	CTexture* m_pRifleTextureCom = nullptr;
	CTexture* m_pShotGunTextureCom = nullptr;
	CTexture* m_pSniperTextureCom = nullptr;
	CShader*	m_pShaderCom = nullptr;
	GUN_ID		m_eGunID;

	_matrix		m_matWorld;
	_matrix		m_matView;
	_matrix		m_matProj;

public:
	static CCrossHeader*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	void Free(void) override;
};

