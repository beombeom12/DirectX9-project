#pragma once

#include "Camera.h"
#include "Engine_Include.h"

class CDynamicCamera : public Engine::CCamera
{
private:
	explicit CDynamicCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CDynamicCamera();

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

public:
	void		Save_Position();
	void		Load_Position();
	void		SaveBtn() { m_bSave = !m_bSave; }
	void		LoadBtn() { m_bLoad = !m_bLoad; }
	void		Set_MainCam() { m_bMainCameraOn = !m_bMainCameraOn; }

public:
	static CDynamicCamera*		Create(LPDIRECT3DDEVICE9 pGraphicDev,
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
	_bool			m_bSave = false;
	_bool			m_bLoad = false;
	_bool			m_bMainCameraOn = false;

	list<pair<_vec3, _vec3>>  m_liPos;

	_float			m_fDistnace = 17.f;
private:
	virtual void Free(void) override;
};

