#pragma once
#include "GameObject.h"
class CLogoBilboard : public CGameObject
{
private:
	explicit CLogoBilboard(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLogoBilboard();

public:
	HRESULT Ready_Object();
	_int	Update_Object(const _float& fTimeDelta) override;
	void	LateUpdate_Object(void) override;
	void	Render_Object(void) override;

private:
	HRESULT Add_Component();

private:
	CRcTex*	m_pBufferCom = nullptr;
	CTransform* m_pTransCom = nullptr;
	CTexture* m_pTextureCom = nullptr;



	CTransform* m_pMeMorialTransCom = nullptr;
	CTexture*	m_pMeMorialTexture = nullptr;
	CRcTex*		m_pMeMorialBufferCom = nullptr;


	CTransform*	m_pAnubisTransCom = nullptr;
	CTexture*	m_pAnubisTexture = nullptr;
	CRcTex*		m_pAnubisBufferCom = nullptr;


	CTransform* m_pMageTransCom = nullptr;
	CTexture*	m_pMageTexture = nullptr;
	CRcTex*		m_pMageBufferCom = nullptr;


	CTransform* m_pTreeTransCom = nullptr;
	CTexture*	m_pTreeTexture = nullptr;
	CRcTex*		m_pTreeBufferCom = nullptr;


	CTransform* m_pSocerTransCom = nullptr;
	CTexture*	m_pSocerTexture = nullptr;
	CRcTex*		m_pSocerBufferCom = nullptr;


	CTransform* m_pNewTransCom = nullptr;
	CTexture*   m_pNewTexture = nullptr;
	CRcTex*		m_pNewBufferCom = nullptr;

	CTransform*	m_pGlcierTransCom = nullptr;
	CTexture*	m_pGlcierTexture = nullptr;
	CRcTex*		m_pGlcierBufferCom = nullptr;

	CTransform* m_pProTransCom = nullptr;
	CTexture*	m_pProTexture = nullptr;
	CRcTex*		m_pProBufferCom = nullptr;

	CTransform*	m_pHandTransCom = nullptr;
	CTexture*	m_pHandTexture = nullptr;
	CRcTex*		m_pHandBufferCom = nullptr;


	CTransform*	m_pEgyptTransCom = nullptr;
	CTexture*	m_pEgyptTexture = nullptr;
	CRcTex*		m_pEgyptBufferCom = nullptr;

public:
	static CLogoBilboard*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	void Free(void) override;
};

