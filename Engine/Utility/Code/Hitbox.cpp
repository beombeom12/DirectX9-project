#include "..\..\Header\HitBox.h"
#include "Export_Function.h"

USING(Engine)

CHitBox::CHitBox(LPDIRECT3DDEVICE9 pGraphicDev)
	:CVIBuffer(pGraphicDev), vPos(nullptr), m_bClone(false)
{
}

CHitBox::CHitBox(const CHitBox & rhs)
	: CVIBuffer(rhs), vPos(rhs.vPos), m_bClone(true)
{
}

CHitBox::~CHitBox()
{
}

void CHitBox::Get_MinMax(_vec3* _vMin, _vec3* _vMax)
{
	VTXCOL*		pVertex = nullptr;

	UINT dwStride = D3DXGetFVFVertexSize(m_dwFVF);

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	D3DXComputeBoundingBox(&pVertex->vPos, m_dwVtxCnt, dwStride, &vMin, &vMax);

	memcpy(_vMin, vMin, sizeof(_vec3));
	memcpy(_vMax, vMax, sizeof(_vec3));

	m_pVB->Unlock();
}

HRESULT CHitBox::Ready_Buffer(void)
{
	m_dwVtxCnt = 8;
	m_dwTriCnt = 12;
	m_dwVtxSize = sizeof(VTXCOL);
	m_dwFVF = FVF_COL;

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	vPos = new _vec3[m_dwVtxCnt];

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	VTXCOL*		pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);
	// 3인자 : 배열에 저장된 첫 번째 버텍스의 주소를 얻어옴

	// 앞면
	pVertex[0].vPos = { -1.f, 1.f, -1.f };
	pVertex[0].dwColor = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
	vPos[0] = pVertex[0].vPos;

	pVertex[1].vPos = { 1.f, 1.f, -1.f };
	pVertex[1].dwColor = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
	vPos[1] = pVertex[1].vPos;

	pVertex[2].vPos = { 1.f, -1.f, -1.f };
	pVertex[2].dwColor = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
	vPos[2] = pVertex[2].vPos;

	pVertex[3].vPos = { -1.f, -1.f, -1.f };
	pVertex[3].dwColor = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
	vPos[3] = pVertex[3].vPos;

	// 뒷면
	pVertex[4].vPos = { -1.f, 1.f, 1.f };
	pVertex[4].dwColor = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
	vPos[4] = pVertex[4].vPos;

	pVertex[5].vPos = { 1.f, 1.f, 1.f };
	pVertex[5].dwColor = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
	vPos[5] = pVertex[5].vPos;

	pVertex[6].vPos = { 1.f, -1.f, 1.f };
	pVertex[6].dwColor = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
	vPos[6] = pVertex[6].vPos;

	pVertex[7].vPos = { -1.f, -1.f, 1.f };
	pVertex[7].dwColor = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
	vPos[7] = pVertex[7].vPos;

	m_pVB->Unlock();


	INDEX32*		pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	// X+
	pIndex[0]._0 = 1; pIndex[0]._1 = 5; pIndex[0]._2 = 6;
	pIndex[1]._0 = 1; pIndex[1]._1 = 6; pIndex[1]._2 = 2;

	// X-
	pIndex[2]._0 = 4; pIndex[2]._1 = 0; pIndex[2]._2 = 3;
	pIndex[3]._0 = 4; pIndex[3]._1 = 3; pIndex[3]._2 = 7;

	// Y+
	pIndex[4]._0 = 4; pIndex[4]._1 = 5;	pIndex[4]._2 = 1;
	pIndex[5]._0 = 4; pIndex[5]._1 = 1; pIndex[5]._2 = 0;

	// Y-
	pIndex[6]._0 = 3; pIndex[6]._1 = 2;	pIndex[6]._2 = 6;
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

void CHitBox::Render_Buffer(void)
{
	CVIBuffer::Render_Buffer();
}

CHitBox * CHitBox::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CHitBox*	pInstance = new CHitBox(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CComponent * CHitBox::Clone(void)
{
	return new CHitBox(*this);
}

void CHitBox::Free(void)
{
	CVIBuffer::Free();
	if (m_bClone == false)
		Safe_Delete_Array(vPos);
}
