#include "stdafx.h"
#include "..\Header\FlightCamera.h"

#include "Export_Function.h"

CFlightCamera::CFlightCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCamera(pGraphicDev)
{
}

CFlightCamera::~CFlightCamera()
{
}

HRESULT CFlightCamera::Ready_Object(const _vec3* pEye,
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

	m_fAspect = fAspect;
	m_fNear = fNear;
	m_fFov = fFov;
	m_fFar = fFar;

	m_bMainCameraOn = false;

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	FAILED_CHECK_RETURN(CCamera::Ready_Object(), E_FAIL);

	return S_OK;
}

Engine::_int CFlightCamera::Update_Object(const _float& fTimeDelta)
{
	if (!m_bMainCameraOn)
		return 0;

	m_vAt = m_pCalculator->Get_Mouse_World(g_hWnd);

	Move(2.f, fTimeDelta);

	Key_Input(fTimeDelta);

	_int iExit = CCamera::Update_Object(fTimeDelta);
	CCamera::Update_Object(fTimeDelta);

	return 0;
}

void CFlightCamera::LateUpdate_Object(void)
{
	if (!m_bMainCameraOn)
		return;

	CCamera::LateUpdate_Object();

	if (false == m_bFix)
	{
		Mouse_Fix();
		//Mouse_Move();
	}
	
}

void CFlightCamera::Free(void)
{
	CCamera::Free();
}

HRESULT CFlightCamera::Add_Component(void)
{
	CComponent* pInstance = nullptr;

	pInstance = m_pCalculator = dynamic_cast<CCalculator*>(Engine::Clone_Proto(CALCULATOR_COMP));
	NULL_CHECK_RETURN(pInstance, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ CALCULATOR_COMP, pInstance });

	return S_OK;
}

CFlightCamera* CFlightCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFov /*= D3DXToRadian(60.f)*/, const _float& fAspect /*= (float)WINCX / WINCY*/, const _float& fNear /*= 0.1f*/, const _float& fFar /*= 1000.f*/)
{
	CFlightCamera*		pInstance = new CFlightCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pEye, pAt, pUp, fFov, fAspect, fNear, fFar)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CFlightCamera::Key_Input(const _float& fTimeDelta)
{
	_matrix		matCamWorld;
	D3DXMatrixInverse(&matCamWorld, nullptr, &m_matView);

	if (Get_DIKeyState(DIK_W) & 0x80)
	{
		_vec3		vLook;
		memcpy(&vLook, &matCamWorld.m[2][0], sizeof(_vec3));

		_vec3		vLength = *D3DXVec3Normalize(&vLook, &vLook) * 5.f * fTimeDelta;

		m_vEye += vLength;
		m_vAt += vLength;
	}

	if (Get_DIKeyState(DIK_S) & 0x80)
	{
		_vec3		vLook;
		memcpy(&vLook, &matCamWorld.m[2][0], sizeof(_vec3));

		_vec3		vLength = *D3DXVec3Normalize(&vLook, &vLook) * 5.f * fTimeDelta;

		m_vEye -= vLength;
		m_vAt -= vLength;
	}

	if (Get_DIKeyState(DIK_D) & 0x80)
	{
		_vec3		vRight;
		memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

		_vec3		vLength = *D3DXVec3Normalize(&vRight, &vRight) * 5.f * fTimeDelta;

		m_vEye += vLength;
		m_vAt += vLength;
	}

	if (Get_DIKeyState(DIK_A) & 0x80)
	{
		_vec3		vRight;
		memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

		_vec3		vLength = *D3DXVec3Normalize(&vRight, &vRight) * 5.f * fTimeDelta;

		m_vEye -= vLength;
		m_vAt -= vLength;
	}

	if (Get_DIKeyState(DIK_T) & 0x80)
	{
		if (m_bCheck)
			return;

		m_bCheck = true;

		if (m_bFix)
			m_bFix = false;
		else
			m_bFix = true;
	}
	else
		m_bCheck = false;

	if (false == m_bFix)
		return;
}

void CFlightCamera::Mouse_Move(void)
{
	_matrix		matCamWorld;
	D3DXMatrixInverse(&matCamWorld, nullptr, &m_matView);

	_long		dwMouseMove = 0;

	if (dwMouseMove = Engine::Get_DIMouseMove(DIMS_X))
	{
		_vec3		vUp{ 0.f, 1.f, 0.f };

		_vec3		vLook = m_vAt - m_vEye;

		_matrix		matRot;
		D3DXMatrixRotationAxis(&matRot, &vUp, D3DXToRadian(dwMouseMove / 10.f));
		D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

		m_vAt = m_vEye + vLook;
	}

	if (dwMouseMove = Engine::Get_DIMouseMove(DIMS_Y))
	{
		_vec3		vRight;
		memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

		_vec3		vLook = m_vAt - m_vEye;

		_matrix		matRot;
		D3DXMatrixRotationAxis(&matRot, &vRight, D3DXToRadian(dwMouseMove / 10.f));
		D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

		m_vAt = m_vEye + vLook;

	}
}

void CFlightCamera::Mouse_Fix(void)
{
	POINT	pt{ WINCX >> 1 , WINCY >> 1 };

	ClientToScreen(g_hWnd, &pt);
	SetCursorPos(pt.x, pt.y);
}

void CFlightCamera::Move(const _float & fSpeed, const _float& fTimeDelta)
{
	_vec3 vLook = m_vAt - m_vEye;

	_vec3		vLength = *D3DXVec3Normalize(&vLook, &vLook) * fSpeed * fTimeDelta;

	m_vEye += vLength;
	m_vAt += vLength;
}