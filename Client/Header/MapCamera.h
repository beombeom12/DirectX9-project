#pragma once

#include "Camera.h"
#include "GameObject.h"
#include "Export_Function.h"

class CMapCamera : public Engine::CCamera
{
private:
	explicit CMapCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMapCamera();

public:
	HRESULT Ready_Object(const _vec3* pEye, const _vec3* pAt, const _vec3* pUp,
		const _float& fFov,
		const _float& fAspect,
		const _float& fNear,
		const _float& fFar);

	virtual _int Update_Object(const _float& fTimeDelta) override;

	virtual void LateUpdate_Object(void) override;
	const _matrix* GetViewmatrix() { return &m_matView; }
	const _matrix* GetProjmatrix() { return &m_matProj; }

private:
	void		Key_Input(const _float& fTimeDelta);
	void		Look_Taget(void);	

private:
	CTransform*		m_pTransform_Target = nullptr;
	_float			m_fDistance = 10.f;
	_float			m_fSpeed = 10.f;
	_float			m_fAngle = 0.f;


private:
	CGameObject* m_pPlayer = nullptr;

	CRcTex*		m_pBufferCom = nullptr;
	CTransform* m_pTransCom = nullptr;

	CTexture*	m_pTexture_100 = nullptr;
	CTexture*	m_pTexture_75 = nullptr;
	CTexture*	m_pTexture_50 = nullptr;
	CTexture*	m_pTexture_25 = nullptr;
	CTexture*	m_pTexture_0 = nullptr;

	_matrix m_matWorld;
	_matrix m_matView;
	_matrix m_matProj;

	_float m_fFrame = 0.f;


public:
	static CMapCamera*		Create(LPDIRECT3DDEVICE9 pGraphicDev,
		const _vec3* pEye,
		const _vec3* pAt,
		const _vec3* pUp,
		const _float& fFov = D3DXToRadian(60.f),
		const _float& fAspect = (float)WINCX / WINCY,
		const _float& fNear = 0.1f,
		const _float& fFar = 1000.f);

private:
	CTransform* m_pPlayerTransform = nullptr;

private:
	virtual void Free(void) override;

};