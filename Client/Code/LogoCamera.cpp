#include "stdafx.h"
#include "..\Header\LogoCamera.h"

#include "Export_Function.h"

CLogoCamera::CLogoCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCamera(pGraphicDev)
{
}


CLogoCamera::~CLogoCamera()
{
}

HRESULT CLogoCamera::Ready_Object(const _vec3* pEye,
	const _vec3* pAt,
	const _vec3* pUp,
	const _float& fFov,
	const _float& fAspect,
	const _float& fNear,
	const _float& fFar)
{
	m_vEye = *pEye;
	m_vAt = *pAt;
	m_vUp = *pUp;

	m_fFov = fFov;
	m_fAspect = fAspect;
	m_fNear = fNear;
	m_fFar = fFar;

	FAILED_CHECK_RETURN(CCamera::Ready_Object(), E_FAIL);

	return S_OK;
}

Engine::_int CLogoCamera::Update_Object(const _float& fTimeDelta)
{
	RightCamera(fTimeDelta);

	_int iExit = CCamera::Update_Object(fTimeDelta);

	return iExit;
}

void CLogoCamera::LateUpdate_Object(void)
{
	CCamera::LateUpdate_Object();
}

void CLogoCamera::Free(void)
{
	CCamera::Free();
}

CLogoCamera* CLogoCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFov /*= D3DXToRadian(60.f)*/, const _float& fAspect /*= (float)WINCX / WINCY*/, const _float& fNear /*= 0.1f*/, const _float& fFar /*= 1000.f*/)
{
	CLogoCamera*		pInstance = new CLogoCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pEye, pAt, pUp, fFov, fAspect, fNear, fFar)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CLogoCamera::RightCamera(const _float& fTimeDelta)
{
	_matrix		matCamWorld;
	D3DXMatrixInverse(&matCamWorld, nullptr, &m_matView);

	_vec3		vRight;
	memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

	_vec3		vLength = *D3DXVec3Normalize(&vRight, &vRight) * fTimeDelta;

	m_vEye.y = 0.f;
	m_vEye += vLength;
}