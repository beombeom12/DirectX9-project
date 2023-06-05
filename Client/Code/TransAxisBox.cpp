#include "stdafx.h"
#include "..\Header\TransAxisBox.h"

#include "Export_Function.h"

USING(Engine)

CTransAxisBox::CTransAxisBox(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev), m_bIsRoot(false)
{
}

CTransAxisBox::~CTransAxisBox()
{
}

HRESULT CTransAxisBox::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransCom->Set_Pos(0.f, 0.f, 0.f);
	m_pTransCom->Set_Scale(1.f, 1.f, 1.f);
	m_pTransCom->Static_Update();

	m_fX = 0.f;
	m_fY = 0.f;
	m_fZ = 0.f;

	return S_OK;
}

_int CTransAxisBox::Update_Object(const _float & fTimeDelta)
{
	if (m_bDead)
	{
		return -1;
	}

	Engine::CGameObject::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDER_ANIOBJ, this);

	_matrix matWorld, matParent;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matParent);

	m_pTransCom->Worldspace_By_Quarternion();

	m_pTransCom->Get_WorldMatrix(&matWorld);

	D3DXMatrixTranslation(&m_AxisMoveMatrix, m_fX, m_fY, m_fZ);

	matWorld = m_AxisMoveMatrix * matWorld;

	for (auto& iter : m_ParentList)
	{
		_matrix matTemp;
		iter->Get_Transform()->Get_WorldMatrix(&matTemp);

		_vec3 vScale, vPos;
		D3DXQUATERNION qRot;

		D3DXMatrixDecompose(&vScale, &qRot, &vPos, &matTemp);

		_matrix matRot, matTrans, matResult;
		D3DXMatrixRotationQuaternion(&matRot, &qRot);

		D3DXMatrixTranslation(&matTrans, vPos.x, vPos.y, vPos.z);

		matResult = matRot * matTrans;

		matParent *= matResult;
	}

	m_FinalWorldMatrix = matWorld * matParent;

	return 0;
}

void CTransAxisBox::LateUpdate_Object(void)
{
	Engine::CGameObject::LateUpdate_Object();
}

void CTransAxisBox::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_FinalWorldMatrix);
	m_pTextureCom->Set_Texture(m_iTexIndex);
	FAILED_CHECK_RETURN(Set_Material(), );
	m_pBufferCom->Render_Buffer();
}

bool CTransAxisBox::Set_SelectGizmo(void)
{
	_matrix matCamera;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamera);
	D3DXMatrixInverse(&matCamera, nullptr, &matCamera);

	_vec3	vEye = { matCamera.m[3][0], matCamera.m[3][1], matCamera.m[3][2] };

	return false;
}

HRESULT CTransAxisBox::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CFlexibleAxisCube*>(Clone_Proto(L"Proto_FlexibleAxisCubeCom"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_FlexibleAxisCubeCom", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_CubePlayerTexture"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_CubePlayerTexture", pComponent });

	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_TransformCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_TransformCom", pComponent });

	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Clone_Proto(L"Proto_CalculatorCom"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_CalculatorCom", pComponent });

	pComponent = m_pQuaternion = dynamic_cast<CQuarternion*>(Clone_Proto(L"Proto_QuaternionCom"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_QuaternionCom", pComponent });

	return S_OK;
}

HRESULT CTransAxisBox::Set_Material()
{
	D3DMATERIAL9 Material;
	ZeroMemory(&Material, sizeof(D3DMATERIAL9));

	Material.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	Material.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	Material.Ambient = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.f);
	Material.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
	Material.Power = 0.f;

	m_pGraphicDev->SetMaterial(&Material);

	return S_OK;
}

void CTransAxisBox::AxisMatrixCreate(void)
{
	D3DXMatrixTranslation(&m_AxisMoveMatrix, m_fX, m_fY, m_fZ);

	m_pTransCom->m_matWorld *= m_AxisMoveMatrix;
}

CTransAxisBox * CTransAxisBox::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTransAxisBox* pInstance = new CTransAxisBox(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("TransAxisBox Create Fail");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CTransAxisBox::Free(void)
{
	CGameObject::Free();
}
