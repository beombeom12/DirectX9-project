#include "stdafx.h"
#include "..\Header\Wall.h"
#include "Export_Function.h"

USING(Engine)

CWall::CWall(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
	, m_dwWallTextureNum(0)
{
}

CWall::~CWall()
{
}

HRESULT CWall::Ready_Object(_ulong Texture, _vec3 * Position)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_dwWallTextureNum = Texture;
	m_pTransform->Set_Pos(Position->x, 0.5f, Position->z);

	return S_OK;
}

_int CWall::Update_Object(const _float & fTimeDelta)
{
	_uint iResult = CGameObject::Update_Object(fTimeDelta);
	//Wall_Mapping();
	return iResult;
}

void CWall::LateUpdate_Object(void)
{
	Add_RenderGroup(RENDER_MAPSETTING, this);
	CGameObject::LateUpdate_Object();
}

void CWall::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pTexture->Set_Texture(m_dwWallTextureNum);
	m_pCubeTex->Render_Buffer();
}

HRESULT CWall::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::Clone_Proto(TRANSFORM_COMP));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ TRANSFORM_COMP, pComponent });

	pComponent = m_pCubeTex = dynamic_cast<CCubeTex*>(Engine::Clone_Proto(L"Proto_CubeTexCom"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_CubeTexCom", pComponent });

	pComponent = m_pTexture = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_CubePlayerTexture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_CubePlayerTexture", pComponent });

	return S_OK;
}

HRESULT CWall::Wall_Mapping(void)
{
	_vec3		vPos;
	_vec3		vSize;
	m_pTransform->Get_Info(INFO_POS, &vPos);
	m_pTransform->Get_Scale(&vSize);
	if (!m_MappingInit)
	{
		CGameObject*	m_pMapWall = CWallMapping::Create(m_pGraphicDev);
		TCHAR* szCntName = new TCHAR[64];
		wsprintf(szCntName, L"");
		const _tchar*	szNumbering = L"MapWall_%d";
		wsprintf(szCntName, szNumbering, m_iCnt);
		Engine::Add_GameObject(STAGE_WALL, m_pMapWall, szCntName);
		m_listWallCnt.push_back(szCntName);

		m_pWallMapping = dynamic_cast<CTransform*>(Engine::Get_Component(STAGE_WALL, szCntName, TRANSFORM_COMP, ID_DYNAMIC));
		NULL_CHECK_RETURN(m_pWallMapping, E_FAIL);
		++m_iCnt;
		m_MappingInit = true;
	}

	m_pWallMapping->Set_Pos(vPos.x, vPos.y, vPos.z);
	m_pWallMapping->Set_Scale(vSize.x, vSize.y, vSize.z);

	return S_OK;
}

CWall * CWall::Create(LPDIRECT3DDEVICE9 pGraphicDev, _ulong Texture, _vec3 * Position)
{
	CWall*	pInstance = new CWall(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(Texture, Position)))
	{
		MSG_BOX("Wall Object Create Fail");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CWall::Free(void)
{
	CGameObject::Free();
}
