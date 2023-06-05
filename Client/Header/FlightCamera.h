#pragma once

#include "Camera.h"
#include "Engine_Include.h"

class CFlightCamera : public Engine::CCamera
{
private:
	explicit CFlightCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFlightCamera();

public:
	HRESULT Ready_Object(const _vec3* pEye, const _vec3* pAt, const _vec3* pUp,
		const _float& fFov,
		const _float& fAspect,
		const _float& fNear,
		const _float& fFar);

	virtual _int Update_Object(const _float& fTimeDelta) override;

	virtual void LateUpdate_Object(void) override;

private:
	void		Key_Input(const _float& fTimeDelta);
	void		Mouse_Move(void);
	void		Mouse_Fix(void);
	void		Move(const _float& fSpeed, const _float& fTimeDelta);

public:
	void		Set_MainCam(_bool is) { m_bMainCameraOn = is; }
	_bool		Get_Maincam(void) { return m_bMainCameraOn; }
	void		Set_Eye(_vec3* vEye) { m_vEye = *vEye; }
	void		Set_At(_vec3* vAt) { m_vAt = *vAt; }

private:
	HRESULT			Add_Component(void);
	CCalculator*	m_pCalculator = nullptr;

public:
	static CFlightCamera*		Create(LPDIRECT3DDEVICE9 pGraphicDev,
		const _vec3* pEye,
		const _vec3* pAt,
		const _vec3* pUp,
		const _float& fFov = D3DXToRadian(60.f),
		const _float& fAspect = (float)WINCX / WINCY,
		const _float& fNear = 0.1f,
		const _float& fFar = 1000.f);

private:
	_bool			m_bFix = false;
	_bool			m_bCheck = true;
	_bool			m_bMainCameraOn = false;

private:
	virtual void Free(void) override;
};

