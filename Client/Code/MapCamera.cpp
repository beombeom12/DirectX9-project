#include "stdafx.h"
#include "..\Header\MapCamera.h"



CMapCamera::CMapCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCamera(pGraphicDev)
{
}


CMapCamera::~CMapCamera()
{
}

HRESULT CMapCamera::Ready_Object(const _vec3* pEye,
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

Engine::_int CMapCamera::Update_Object(const _float& fTimeDelta)
{
	Key_Input(fTimeDelta);

	Look_Taget();


	_int iExit = CCamera::Update_Object(fTimeDelta);

	return 0;//iExit;
}

void CMapCamera::LateUpdate_Object(void)
{
	CCamera::LateUpdate_Object();
}

void CMapCamera::Free(void)
{
	CCamera::Free();
}

CMapCamera* CMapCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFov /*= D3DXToRadian(60.f)*/, const _float& fAspect /*= (float)WINCX / WINCY*/, const _float& fNear /*= 0.1f*/, const _float& fFar /*= 1000.f*/)
{
	CMapCamera*		pInstance = new CMapCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pEye, pAt, pUp, fFov, fAspect, fNear, fFar)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CMapCamera::Key_Input(const _float& fTimeDelta)
{
	// ¸¶¿ì½º ÈÙÀ» ÅëÇÑ ÁÜÀÎ ÁÜ ¾Æ¿ô
	if (m_fDistance >= 1)
	{
		if (Get_DIMouseMove(DIMS_Z) > 0)
			m_fDistance -= fTimeDelta * m_fSpeed * 5.f;
	}

	if (m_fDistance <= 20)
	{
		if (Get_DIMouseMove(DIMS_Z) < 0)
			m_fDistance += fTimeDelta * m_fSpeed * 5.f;
	}	
}

void CMapCamera::Look_Taget(void)
{
	
	_vec3 vLook;
	_vec3 vRight;	
	_vec3 vUp;	
	_vec3 vPos;
	_float fStaticPointX = 13.f;
	_float fStaticPointZ = 26.f;
	if (nullptr == m_pTransform_Target)
	{
		m_pTransform_Target = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_Character", L"BODY", L"Proto_TransformCom", ID_DYNAMIC));
		NULL_CHECK(m_pTransform_Target);
	}

	m_pTransform_Target->Get_Info(INFO_LOOK, &vLook);
	m_pTransform_Target->Get_Info(INFO_RIGHT, &vRight);
	m_pTransform_Target->Get_Info(INFO_UP, &vUp);
	m_pTransform_Target->Get_Info(INFO_POS, &vPos);
	if (vPos.x >  fStaticPointZ)
	{	
		if (vPos.z > fStaticPointX)
			{
				m_vEye = { 0.f, 1.f,-0.001f };
				D3DXVec3Normalize(&m_vEye, &m_vEye);
				m_vEye *= m_fDistance*2.5f;
				vPos.y = 1.f;
				m_vEye += vPos;
				m_vAt = vPos;
			}		
	}
	if(vPos.x <  fStaticPointZ)				// Ä«¸Þ¶ó xÁÂÇ¥ ÃÖ¼Ò°ª
	{
		vPos.x =  fStaticPointZ;
		m_vEye = { 0.f, 1.f,-0.001f };
		D3DXVec3Normalize(&m_vEye, &m_vEye);
		m_vEye *= m_fDistance*2.5f;
		vPos.y = 1.f;
		m_vEye += vPos;
		m_vAt = vPos;
	}

	if ( vPos.x > 128.f - fStaticPointZ )
	{
		vPos.x = 128.f - fStaticPointZ;
		m_vEye = { 0.f, 1.f,-0.001f };
		D3DXVec3Normalize(&m_vEye, &m_vEye);
		m_vEye *= m_fDistance*2.5f;
		vPos.y = 1.f;
		m_vEye += vPos;
		m_vAt = vPos;
	}


	if (vPos.z < fStaticPointX)
	{
		vPos.z = fStaticPointX;

		m_vEye = { 0.f, 1.f,-0.001f };
		D3DXVec3Normalize(&m_vEye, &m_vEye);
		m_vEye *= m_fDistance*2.5f;
		vPos.y = 1.f;
		m_vEye += vPos;
		m_vAt = vPos;
	}
	if (vPos.z > 127.f - fStaticPointX)
	{
		vPos.z = 127.f - fStaticPointX;

		m_vEye = { 0.f, 1.f,-0.001f };
		D3DXVec3Normalize(&m_vEye, &m_vEye);
		m_vEye *= m_fDistance*2.5f;
		vPos.y = 1.f;
		m_vEye += vPos;
		m_vAt = vPos;
	}
}
