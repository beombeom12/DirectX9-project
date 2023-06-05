#pragma once
#include "GameObject.h"
#include "StaticCamera.h"
#include "Export_Function.h"

USING(Engine)


class CPI_Default : public CGameObject
{

private:
	explicit CPI_Default(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPI_Default();

public:
	virtual HRESULT Ready_Object() override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object(void) override;
	virtual void Render_Object(void) override;
	
private:
	HRESULT				Add_Component(void);

	

private:
	CRcTex*			m_RcTexCom = nullptr;
	CTransform*		m_TransformCom = nullptr;
	CTransform*		m_TransformCom2 = nullptr;

	CTexture*		m_TextureCom = nullptr;
	CCamera*		m_CameraCom = nullptr;
	CStaticCamera*	m_StaticCamera = nullptr;

	_float		m_fFov, m_fAspect, m_fNear, m_fFar;
	_matrix		matWorld, matView, matProj;

public:
	static CPI_Default*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void Free(void);
};