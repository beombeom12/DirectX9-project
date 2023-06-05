#include "VIBuffer.h"

USING(Engine)

CVIBuffer::CVIBuffer(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)
	, m_pVB(nullptr)
	, m_pIB(nullptr)
	, m_dwVtxCnt(0)
	, m_dwVtxSize(0)
	, m_dwFVF(0)
	, m_dwTriCnt(0)
	, m_dwIdxSize(0)
{
	
}

Engine::CVIBuffer::CVIBuffer(const CVIBuffer& rhs)
	: CComponent(rhs)
	, m_pVB(rhs.m_pVB)
	, m_pIB(rhs.m_pIB)
	, m_dwVtxCnt(rhs.m_dwVtxCnt)
	, m_dwVtxSize(rhs.m_dwVtxSize)
	, m_dwFVF(rhs.m_dwFVF)
	, m_dwTriCnt(rhs.m_dwTriCnt)
	, m_dwIdxSize(rhs.m_dwIdxSize)

{
	m_pVB->AddRef();
	m_pIB->AddRef();
}

CVIBuffer::~CVIBuffer()
{
}

HRESULT CVIBuffer::Ready_Buffer(void)
{
	if (FAILED(m_pGraphicDev->CreateVertexBuffer(m_dwVtxCnt * m_dwVtxSize,  // 그리고자 하는 도형의 메모리 크기
												0,							// 메모리 버퍼 사용방식 옵션, 0인 경우 정적 버퍼를 사용(D3DUSAGE_DYNAMIC을 삽입할 경우 동적 버퍼 용도로 변경)
												m_dwFVF,					// 정점의 속성 정보	
												D3DPOOL_MANAGED,			// 메모리 저장 방식, 정적 버퍼인 경우엔 MANAGED고정
												&m_pVB,						// 생성할 버텍스 버퍼 컴객체	
												nullptr)))					// 공유할 핸들, 기본값 NULL
		return E_FAIL;
	


	if (FAILED(m_pGraphicDev->CreateIndexBuffer(m_dwIdxSize * m_dwTriCnt,
												0,							// 메모리 버퍼 사용방식 옵션, 0인 경우 정적 버퍼를 사용(D3DUSAGE_DYNAMIC을 삽입할 경우 동적 버퍼 용도로 변경)
												m_IdxFmt,					// 인덱스 속성 정보	
												D3DPOOL_MANAGED,			// 메모리 저장 방식, 정적 버퍼인 경우엔 MANAGED고정
												&m_pIB,						// 생성할 버텍스 버퍼 컴객체	
												nullptr)))
												return E_FAIL;

	return S_OK;
}

void CVIBuffer::Render_Buffer(void)
{
	// 디바이스 컴객체에게 내가 그리고자하는 버텍스 버퍼를 링크시키는 함수
	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);
	// 1. 몇 번 슬롯을 통해 보관, 출력할 것인가
	// 2. 버텍스들을 저장하는 객체
	// 3. 어디서부터 그리기를 수행할 것인가(메모리 단위이기 때문에 0byte부터)
	// 4. 어떤 크기로 그리기를 수행할 것인가

	// 현재 그리려는 버텍스의 속성을 지정하는 함수(매 프레임마다 지정해줘야 함)
	m_pGraphicDev->SetFVF(m_dwFVF);

	// 디바이스 장치와 인덱스 버퍼를 링크하는 함수
	m_pGraphicDev->SetIndices(m_pIB);


	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCnt, 0, m_dwTriCnt);

	/*// 도형 그리기 함수
	m_pGraphicDev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, m_dwTriCnt);
	// 1. 삼각형을 그리는 방식
	// 2. 몇 번째 버텍스부터 그리기를 시작할 것인가
	// 3. 그리기를 수행할 삼각형 개수*/
}

void CVIBuffer::Free(void)
{
	Safe_Release(m_pIB);
	Safe_Release(m_pVB);

	CComponent::Free();
}
