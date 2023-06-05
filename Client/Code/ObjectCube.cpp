#include "stdafx.h"
#include "..\Header\ObjectCube.h"


CObjectCube::CObjectCube(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CObjectCube::~CObjectCube()
{
}

HRESULT CObjectCube::Ready_Object(int PosX, int PosY, OBJECT_ID eID)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	if (PosX == 0 && PosY == 0) {}
	else
	{
		Set_TransformPositon();
	}

	_vec3 vScale = { 0.5f, 0.5f, 0.5f };
	m_pTransCom->Set_Scale(&vScale);

	switch (eID)
	{
	case Engine::GUN:
		m_iTexIndex = 1;
		break;
	case Engine::SHOP:
		m_iTexIndex = 2;
		break;
	case Engine::LAVA:
		m_iTexIndex = 3;
		break;
	case Engine::ITEMBOX:
		m_iTexIndex = 4;
		break;
	case Engine::THRONE:
		m_iTexIndex = 5;
		break;
	case Engine::SLIME:
		m_iTexIndex = 6;
		break;
	case Engine::FIREMAN:
		m_iTexIndex = 7;
		break;
	case Engine::ZOMBIE:
		m_iTexIndex = 8;
		break;
	case Engine::ILLUSIONER:
		m_iTexIndex = 9;
		break;
	default:
		break;
	}

	return S_OK;
}

_int CObjectCube::Update_Object(const _float & fTimeDelta)
{
	CGameObject::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDER_NONALPHA, this);
	return 0;
}

void CObjectCube::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());

	m_pTextureCom->Set_Texture(m_iTexIndex);
	m_pBufferCom->Render_Buffer();
}

void CObjectCube::Set_TransformPositon()
{
	CTerrainTex*	pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(L"Layer_MapTool", L"TerrainByTool", L"Proto_TerrainTexCom", ID_STATIC));
	NULL_CHECK_RETURN(pTerrainBufferCom, );

	CTransform*		pTerrainTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_MapTool", L"TerrainByTool", TRANSFORM_COMP, ID_DYNAMIC));
	NULL_CHECK_RETURN(pTerrainTransformCom, );

	_vec3 Temp = m_pCalculatorCom->Peek_Target_Vector(g_hWnd, &(_vec3(0.f, 0.f, 0.f)), pTerrainBufferCom, pTerrainTransformCom);

	m_pTransCom->Set_Pos(Temp.x, Temp.y, Temp.z);
}

bool CObjectCube::Set_SelectGizmo()
{
	if (m_pCalculatorCom->Peek_Cube_Target(g_hWnd, &(_vec3(0.f, 0.f, 0.f)), m_pBufferCom, m_pTransCom))
		return true;

	return false;
}

HRESULT CObjectCube::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CCubeTex*>(Clone_Proto(L"Proto_CubeTexCom"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_CubeTexCom", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_CubePlayerTexture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_CubePlayerTexture", pComponent });

	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Clone_Proto(TRANSFORM_COMP));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ TRANSFORM_COMP, pComponent });

	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Clone_Proto(CALCULATOR_COMP));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ CALCULATOR_COMP, pComponent });

	return S_OK;
}

CObjectCube * CObjectCube::Create(LPDIRECT3DDEVICE9 pGraphicDev, OBJECT_ID eID, int Posx, int Posy)
{
	CObjectCube*	pInstance = new CObjectCube(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(Posx, Posy, eID)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CObjectCube::Free()
{
	CGameObject::Free();
}
