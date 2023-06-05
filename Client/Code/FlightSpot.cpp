#include "stdafx.h"
#include "..\Header\FlightSpot.h"

CFlightSpot::CFlightSpot(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CFlightSpot::~CFlightSpot()
{
}

HRESULT CFlightSpot::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	SetCheckFrustum(false);
	m_pTransform->Set_Scale(0.1f, 0.1f, 0.1f);

	return S_OK;
}

_int CFlightSpot::Update_Object(const _float & fTimeDelta)
{
	Positioning();

	Add_RenderGroup(RENDER_NONALPHA, this);

	return CGameObject::Update_Object(fTimeDelta);
}

void CFlightSpot::LateUpdate_Object(void)
{
	CGameObject::LateUpdate_Object();
}

void CFlightSpot::Render_Object(void)
{
	/*m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());
	m_pCube->Render_Buffer();*/
}

void CFlightSpot::Positioning(void)
{
	_vec3 vMouse;
	vMouse = m_pCalculator->Get_Mouse_World(g_hWnd);

	_matrix matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, nullptr, &matView);
	_vec3 vEye = { matView.m[3][0], matView.m[3][1],matView.m[3][2] };

	_vec3 vCameraLook = vMouse - vEye;
	D3DXVec3Normalize(&vCameraLook, &vCameraLook);

	_vec3 vPos = vEye + (vCameraLook * 10.f);

	m_pTransform->Set_Pos(vPos.x, vPos.y - 2.f, vPos.z);
}

HRESULT CFlightSpot::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Clone_Proto(TRANSFORM_COMP));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ TRANSFORM_COMP, pComponent });

	pComponent = m_pCalculator = dynamic_cast<CCalculator*>(Clone_Proto(CALCULATOR_COMP));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ CALCULATOR_COMP, pComponent });

	pComponent = m_pCube = dynamic_cast<CCubeCol*>(Clone_Proto(CUBECOL_COMP));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ CUBECOL_COMP, pComponent });

	return S_OK;
}

CFlightSpot * CFlightSpot::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CFlightSpot* pInstance = new CFlightSpot(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CFlightSpot::Free(void)
{
	CGameObject::Free();
}
