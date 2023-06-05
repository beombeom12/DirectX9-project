#include "..\..\Header\SphereTex.h"
#include "Export_Function.h"

USING(Engine)

static WORD sphere_vertex(UINT slices, int slice, int stack)
{
	return stack * slices + slice + 1;
}

CSphereTex::CSphereTex(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{
}


Engine::CSphereTex::CSphereTex(const CSphereTex& rhs)
	: CVIBuffer(rhs)
{

}

CSphereTex::~CSphereTex()
{
}

HRESULT CSphereTex::Ready_Buffer(_float _radius)
{
	m_dwVtxCnt = 0;
	m_dwTriCnt = 0;
	m_dwVtxSize = sizeof(VTXTEX);
	m_dwFVF = FVF_TEX;
	
	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	float radius = _radius; // 구의 반지름
	unsigned __int32 stackCount = 20; // 가로 분할
	unsigned __int32 sliceCount = 20; // 세로 분할

	vector<VTXTEX> vecVtxSphere;

	VTXTEX v;

	v.vPos = _vec3(0.0f, radius, 0.0f);
	v.vTexUV = _vec2(0.5f, 0.0f);
	v.vNormal = v.vPos;
	D3DXVec3Normalize(&(v.vNormal), &(v.vNormal));
	vecVtxSphere.push_back(v);
	m_dwVtxCnt++;

	float stackAngle = D3DX_PI / stackCount;
	float sliceAngle = (D3DX_PI * 2.f) / sliceCount;

	float deltaU = 1.f / static_cast<float>(sliceCount);
	float deltaV = 1.f / static_cast<float>(stackCount);

	for (unsigned __int32 y = 1; y <= stackCount - 1; ++y)
	{
		float phi = y * stackAngle;

		// 고리에 위치한 정점
		for (unsigned __int32 x = 0; x <= sliceCount; ++x)
		{
			float theta = x * sliceAngle;

			v.vPos.x = radius * sinf(phi) * cosf(theta);
			v.vPos.y = radius * cosf(phi);
			v.vPos.z = radius * sinf(phi) * sinf(theta);

			v.vTexUV = _vec2(deltaU * x, deltaV * y);

			v.vNormal = v.vPos;
			D3DXVec3Normalize(&(v.vNormal), &(v.vNormal));

			vecVtxSphere.push_back(v);

			m_dwVtxCnt++;
		}
	}

	v.vPos = _vec3(0.0f, -radius, 0.0f);
	v.vTexUV = _vec2(0.5f, 1.0f);
	v.vNormal = v.vPos;
	D3DXVec3Normalize(&(v.vNormal), &(v.vNormal));
	vecVtxSphere.push_back(v);
	m_dwVtxCnt++;

	vector<INDEX32>  vecIdx;

	// 북극 인덱스
	for (unsigned __int32 i = 0; i <= sliceCount; ++i)
	{
		//  [0]
		//   |  \
		//  [i+1]-[i+2]
		vecIdx.push_back({0, i+2, i+1});
		m_dwTriCnt++;
	}

	unsigned __int32 ringVertexCount = sliceCount + 1;
	for (unsigned __int32 y = 0; y < stackCount - 2; ++y)
	{
		for (unsigned __int32 x = 0; x < sliceCount; ++x)
		{
			//  [y, x]-[y, x+1]
			//  |		/
			//  [y+1, x]
			vecIdx.push_back({  1 + (y) * ringVertexCount + (x), 
								1 + (y) * ringVertexCount + (x + 1), 
								1 + (y + 1) * ringVertexCount + (x) });
			//		 [y, x+1]
			//		 /	  |
			//  [y+1, x]-[y+1, x+1]
			vecIdx.push_back({  1 + (y + 1) * ringVertexCount + (x),
								1 + (y) * ringVertexCount + (x + 1),
								1 + (y + 1) * ringVertexCount + (x + 1) });

			m_dwTriCnt+= 2;
		}
	}

	unsigned __int32 bottomIndex = static_cast<unsigned __int32>(vecVtxSphere.size()) - 1;
	unsigned __int32 lastRingStartIndex = bottomIndex - ringVertexCount;
	for (unsigned __int32 i = 0; i < sliceCount; ++i)
	{
		//  [last+i]-[last+i+1]
		//  |      /
		//  [bottom]
		vecIdx.push_back({ bottomIndex,
							lastRingStartIndex + i ,
							lastRingStartIndex + i + 1 });

		m_dwTriCnt++;
	}

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	VTXTEX* pVertex = nullptr;
	INDEX32* pIndex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);
	memcpy(pVertex, vecVtxSphere.data(), sizeof(VTXTEX) * vecVtxSphere.size());
	m_pVB->Unlock();

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);
	memcpy(pIndex, vecIdx.data(), sizeof(INDEX32) * vecIdx.size());
	m_pIB->Unlock();

	return S_OK;
}

void CSphereTex::Render_Buffer(void)
{
	CVIBuffer::Render_Buffer();
}

CSphereTex * CSphereTex::Create(LPDIRECT3DDEVICE9 pGraphicDev, _float _radius)
{
	CSphereTex*   pInstance = new CSphereTex(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer(_radius)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CComponent * CSphereTex::Clone(void)
{
	return new CSphereTex(*this);
}

void CSphereTex::Free(void)
{
	CVIBuffer::Free();
}