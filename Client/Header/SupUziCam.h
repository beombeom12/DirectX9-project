//#pragma once
//#include "Camera.h"
//#include "Engine_Include.h"
//#include "Export_Function.h"
//#include "FrustumCullMgr.h"
//
//class CSupUziCam :	public CCamera
//{
//private:
//	explicit CSupUziCam(LPDIRECT3DDEVICE9 pGraphicDev);
//	virtual ~CSupUziCam();
//
//public:
//	HRESULT Ready_Object(const _vec3* pEye, const _vec3* pAt, const _vec3* pUp,
//		const _float& fFov,
//		const _float& fAspect,
//		const _float& fNear,
//		const _float& fFar);
//
//	virtual _int Update_Object(const _float& fTimeDelta) override;
//
//	virtual void LateUpdate_Object(void) override;
//
//private:
//	void		Target_Renewal(void);
//
//private:
//	_float			m_fDistance = 10.f;
//	_float			m_fSpeed = 10.f;
//	_float			m_fAngle = 0.f;
//
//public:
//	static CSupUziCam*		Create(LPDIRECT3DDEVICE9 pGraphicDev,
//		const _vec3* pEye,
//		const _vec3* pAt,
//		const _vec3* pUp,
//		const _float& fFov = D3DXToRadian(60.f),
//		const _float& fAspect = (float)WINCX / WINCY,
//		const _float& fNear = 0.1f,
//		const _float& fFar = 1000.f);
//
//private:
//	virtual void Free(void) override;
//};
//
