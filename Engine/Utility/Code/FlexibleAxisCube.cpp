#include "..\..\Header\FlexibleAxisCube.h"
#include "Export_Function.h"

USING(Engine)

CFlexibleAxisCube::CFlexibleAxisCube(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev), m_vPos(nullptr), m_bClone(false),
	m_AxisX(0.f), m_AxisY(0.f), m_AxisZ(0.f)
{
}


Engine::CFlexibleAxisCube::CFlexibleAxisCube(const CFlexibleAxisCube& rhs)
	: CVIBuffer(rhs), m_vPos(rhs.m_vPos), m_bClone(true),
	m_AxisX(rhs.m_AxisX), m_AxisY(rhs.m_AxisY), m_AxisZ(rhs.m_AxisZ)
{

}

CFlexibleAxisCube::~CFlexibleAxisCube()
{
}

HRESULT CFlexibleAxisCube::Ready_Buffer(void)
{
	m_dwVtxCnt = 8;
	m_vPos = new _vec3[m_dwVtxCnt];
	m_dwTriCnt = 12;

	m_dwVtxSize = sizeof(VTXCUBE);
	m_dwFVF = FVF_CUBE;

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	VTXCUBE*      pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);
	// 3인자 : 배열에 저장된 첫 번째 버텍스의 주소를 얻어옴

	// 앞면
	pVertex[0].vPos = { -1.f + m_AxisX, 1.f + m_AxisY, -1.f + m_AxisZ };
	pVertex[0].vTexUV = { -1.f, 1.f, -1.f };
	pVertex[0].vNormal = pVertex[0].vPos;
	D3DXVec3Normalize(&pVertex[0].vNormal, &pVertex[0].vNormal);
	m_vPos[0] = pVertex[0].vPos;

	pVertex[1].vPos = { 1.f + m_AxisX, 1.f + m_AxisY, -1.f + m_AxisZ };
	pVertex[1].vTexUV = { 1.f, 1.f, -1.f };
	pVertex[1].vNormal = pVertex[1].vPos;
	D3DXVec3Normalize(&pVertex[1].vNormal, &pVertex[1].vNormal);
	m_vPos[1] = pVertex[0].vPos;

	pVertex[2].vPos = { 1.f + m_AxisX, -1.f + m_AxisY, -1.f + m_AxisZ };
	pVertex[2].vTexUV = { 1.f, -1.f, -1.f };
	pVertex[2].vNormal = pVertex[2].vPos;
	D3DXVec3Normalize(&pVertex[2].vNormal, &pVertex[2].vNormal);
	m_vPos[2] = pVertex[2].vPos;

	pVertex[3].vPos = { -1.f + m_AxisX, -1.f + m_AxisY, -1.f + m_AxisZ };
	pVertex[3].vTexUV = { -1.f, -1.f, -1.f };
	pVertex[3].vNormal = pVertex[3].vPos;
	D3DXVec3Normalize(&pVertex[3].vNormal, &pVertex[3].vNormal);
	m_vPos[3] = pVertex[3].vPos;

	// 뒷면
	pVertex[4].vPos = { -1.f + m_AxisX, 1.f + m_AxisY, 1.f + m_AxisZ };
	pVertex[4].vTexUV = { -1.f, 1.f, 1.f };
	pVertex[4].vNormal = pVertex[4].vPos;
	D3DXVec3Normalize(&pVertex[4].vNormal, &pVertex[4].vNormal);
	m_vPos[4] = pVertex[4].vPos;

	pVertex[5].vPos = { 1.f + m_AxisX, 1.f + m_AxisY, 1.f + m_AxisZ };
	pVertex[5].vTexUV = { 1.f, 1.f, 1.f };
	pVertex[5].vNormal = pVertex[5].vPos;
	D3DXVec3Normalize(&pVertex[5].vNormal, &pVertex[5].vNormal);
	m_vPos[5] = pVertex[5].vPos;

	pVertex[6].vPos = { 1.f + m_AxisX, -1.f + m_AxisY, 1.f + m_AxisZ };
	pVertex[6].vTexUV = { 1.f, -1.f, 1.f };
	pVertex[6].vNormal = pVertex[6].vPos;
	D3DXVec3Normalize(&pVertex[6].vNormal, &pVertex[6].vNormal);
	m_vPos[6] = pVertex[6].vPos;

	pVertex[7].vPos = { -1.f + m_AxisX, -1.f + m_AxisY, 1.f + m_AxisZ };
	pVertex[7].vTexUV = { -1.f, -1.f, 1.f };
	pVertex[7].vNormal = pVertex[7].vPos;
	D3DXVec3Normalize(&pVertex[7].vNormal, &pVertex[7].vNormal);
	m_vPos[7] = pVertex[7].vPos;

	m_pVB->Unlock();

	INDEX32*      pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	// X+
	pIndex[0]._0 = 1; pIndex[0]._1 = 5; pIndex[0]._2 = 6;
	pIndex[1]._0 = 1; pIndex[1]._1 = 6; pIndex[1]._2 = 2;

	// X-
	pIndex[2]._0 = 4; pIndex[2]._1 = 0; pIndex[2]._2 = 3;
	pIndex[3]._0 = 4; pIndex[3]._1 = 3; pIndex[3]._2 = 7;

	// Y+
	pIndex[4]._0 = 4; pIndex[4]._1 = 5;   pIndex[4]._2 = 1;
	pIndex[5]._0 = 4; pIndex[5]._1 = 1; pIndex[5]._2 = 0;

	// Y-
	pIndex[6]._0 = 3; pIndex[6]._1 = 2;   pIndex[6]._2 = 6;
	pIndex[7]._0 = 3; pIndex[7]._1 = 6; pIndex[7]._2 = 7;

	// Z+
	pIndex[8]._0 = 7; pIndex[8]._1 = 6; pIndex[8]._2 = 5;
	pIndex[9]._0 = 7; pIndex[9]._1 = 5; pIndex[9]._2 = 4;

	// Z-
	pIndex[10]._0 = 0; pIndex[10]._1 = 1; pIndex[10]._2 = 2;
	pIndex[11]._0 = 0; pIndex[11]._1 = 2; pIndex[11]._2 = 3;

	m_pIB->Unlock();

	return S_OK;
}

void CFlexibleAxisCube::Render_Buffer(void)
{
	CVIBuffer::Render_Buffer();
}

void CFlexibleAxisCube::Update_Axis(void)
{
	VTXCUBE*      pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);
	// 3인자 : 배열에 저장된 첫 번째 버텍스의 주소를 얻어옴

	// 앞면
	pVertex[0].vPos = { -1.f + m_AxisX, 1.f + m_AxisY, -1.f + m_AxisZ };
	m_vPos[0] = pVertex[0].vPos;

	pVertex[1].vPos = { 1.f + m_AxisX, 1.f + m_AxisY, -1.f + m_AxisZ };
	m_vPos[1] = pVertex[0].vPos;

	pVertex[2].vPos = { 1.f + m_AxisX, -1.f + m_AxisY, -1.f + m_AxisZ };
	m_vPos[2] = pVertex[2].vPos;

	pVertex[3].vPos = { -1.f + m_AxisX, -1.f + m_AxisY, -1.f + m_AxisZ };
	m_vPos[3] = pVertex[3].vPos;

	// 뒷면
	pVertex[4].vPos = { -1.f + m_AxisX, 1.f + m_AxisY, 1.f + m_AxisZ };
	m_vPos[4] = pVertex[4].vPos;

	pVertex[5].vPos = { 1.f + m_AxisX, 1.f + m_AxisY, 1.f + m_AxisZ };
	m_vPos[5] = pVertex[5].vPos;

	pVertex[6].vPos = { 1.f + m_AxisX, -1.f + m_AxisY, 1.f + m_AxisZ };
	m_vPos[6] = pVertex[6].vPos;

	pVertex[7].vPos = { -1.f + m_AxisX, -1.f + m_AxisY, 1.f + m_AxisZ };
	m_vPos[7] = pVertex[7].vPos;

	m_pVB->Unlock();
}

CFlexibleAxisCube * CFlexibleAxisCube::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CFlexibleAxisCube*   pInstance = new CFlexibleAxisCube(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CComponent * CFlexibleAxisCube::Clone(void)
{
	return new CFlexibleAxisCube(*this);
}

void CFlexibleAxisCube::Free(void)
{
	CVIBuffer::Free();

	if (m_bClone == false)
		Safe_Delete_Array(m_vPos);
}