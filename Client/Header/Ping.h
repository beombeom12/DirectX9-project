#pragma once
#include "GameObject.h"
class CPing : public CGameObject
{
private:
	explicit CPing(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPing();

public:
	HRESULT Ready_Object(const _vec3 & vPos);
	_int	Update_Object(const _float& fTimeDelta) override;
	void	LateUpdate_Object(void) override;
	void	Render_Object(void) override;

private:
	HRESULT Add_Component();

private:
	CRcTex*		m_pBufferCom = nullptr;
	CTexture*	m_pTextureCom = nullptr;
	CTransform* m_pTransCom = nullptr;

	_float		m_fFrame = 0.f;
	_float		m_fKillTime = 0.f;
	_float		m_fReverseDir = 1.f;
	_float		m_fYSpeed = 0.f;

public:
	static CPing*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos);
	void Free(void) override;
};

