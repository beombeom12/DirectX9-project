#include "stdafx.h"
#include "..\Header\Supporter.h"
#include "ShotParticle.h"
#include "BulletParticle.h"

CSupporter::CSupporter(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CSupporter::~CSupporter()
{
}

HRESULT CSupporter::Ready_Object(const _vec3 & vPos, wstring _strObjTag)
{
	return S_OK;
}

_int CSupporter::Update_Object(const _float & fTimeDelta)
{

	Engine::CGameObject::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDER_NONALPHA, this);

	return 0;
}

void CSupporter::LateUpdate_Object(void)
{
}

void CSupporter::Render_Object(void)
{
}

void CSupporter::Set_On_Terrain(void)
{
	_vec3		vPos;
	m_pTransform->Get_BeforeInfo(INFO_POS, &vPos);

	Engine::CTerrainTex*	pTerrainTexCom = dynamic_cast<Engine::CTerrainTex*>(Engine::Get_Component(STAGE_ENVIRONMENT, L"Terrain", TERRAINTEX_COMP, ID_STATIC));
	NULL_CHECK(pTerrainTexCom);

	_float fHeight = m_pCalculatorCom->HeightOnTerrain(&vPos, pTerrainTexCom->Get_VtxPos(), VTXCNTX, VTXCNTZ) + 0.6f;

	m_pTransform->Set_Pos(vPos.x, fHeight, vPos.z);
}

void CSupporter::Free(void)
{
	CGameObject::Free();
}
