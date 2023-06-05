#pragma once

#include "Camera.h"
#include "Engine_Include.h"
#include "Export_Function.h"
#include "FrustumCullMgr.h"

class CLetterBox;

class CStaticCamera : public Engine::CCamera
{
private:
	explicit CStaticCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStaticCamera();

public:
	HRESULT Ready_Object(const _vec3* pEye, const _vec3* pAt, const _vec3* pUp,
		const _float& fFov,
		const _float& fAspect,
		const _float& fNear,
		const _float& fFar);

	void Update_NullCheck();
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object(void) override;

	const _matrix* GetViewmatrix()  { return &m_matView; }
	const _matrix* GetProjmatrix()  { return &m_matProj; }

	void		HitPlayer() { m_bPlayerHit = true; }
	void		CameraShaking() { m_bEarthQuake = true; }

	_bool		Get_bNpc() { return m_bFirst; }

public:
	void		Set_MainCam(_bool is) { m_bMainCameraOn = is; }
	_bool		Get_MainCam(void) { return m_bMainCameraOn; }

private:
	_bool		m_bMainCameraOn = true;

private:
	void		Key_Input(const _float& fTimeDelta);
	void		Mouse_Fix();
	void		Look_Target(const _float& _fTimeDelta);
	void		Camera_Shaking(const _float& _fTimeDelta);

private:
	CTransform*		m_pTransform_Target = nullptr;
	CTransform*		m_pBombTransform	= nullptr;
	CTransform*		m_pFlightTransform  = nullptr;
	CTransform*		m_pShuttleTransform = nullptr;

	CGameObject*	m_pSupUzi = nullptr;
	CGameObject*	m_pSupShotgun = nullptr;
	CGameObject*	m_pSupSniper = nullptr;

	CTransform*		m_pSupporterUziTransform = nullptr;
	CTransform*		m_pSupporterShotgunTransform = nullptr;
	CTransform*		m_pSupporterSniperTransform = nullptr;

	_float			m_fDistance;
	_float			m_fSpeed = 10.f;
	_float			m_fAngle = 0.f;
	_bool			m_bChangePOV = false;
	_float			m_fBombFrame = 0.f;
	_bool			m_bLetterBox = false;

	CLetterBox*		m_pLetterBox;

	_int			m_iReverse = 1;
	_float			m_fFrame = 0.f;
	_float			m_fEndingFrame = 0.f;
	_float			m_fFlightFrame = 0.f;
	_float			m_fShuttleFrame = 0.f;
	_float			m_fPlayerFrame = 0.f;
	_bool			m_bPlayerHit = false;
	_bool			m_bEarthQuake = false;
	_bool			m_bFirst = false;

	_float			m_fPlayerFrame2 = 0.f;
	_bool			m_bEnding = false;
	_bool			m_bFirstSound = false;
public:
	static CStaticCamera*		Create(LPDIRECT3DDEVICE9 pGraphicDev,
		const _vec3* pEye,
		const _vec3* pAt,
		const _vec3* pUp,
		const _float& fFov = D3DXToRadian(60.f),
		const _float& fAspect = (float)WINCX / WINCY,
		const _float& fNear = 0.1f,
		const _float& fFar = 1000.f);

private:
	virtual void Free(void) override;
};