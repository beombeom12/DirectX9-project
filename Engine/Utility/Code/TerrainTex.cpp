#include "..\..\Header\TerrainTex.h"
#include "Export_Function.h"

USING(Engine)

CTerrainTex::CTerrainTex(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev) , m_hFile(nullptr), m_bClone(false), m_pPos(nullptr)
{
	ZeroMemory(&m_fH, sizeof(BITMAPFILEHEADER));
	ZeroMemory(&m_iH, sizeof(BITMAPINFOHEADER));
}


Engine::CTerrainTex::CTerrainTex(const CTerrainTex& rhs)
	: CVIBuffer(rhs) , m_hFile(rhs.m_hFile) , m_bClone(true), m_pPos(rhs.m_pPos)
{
	memcpy(&m_fH, &rhs.m_fH, sizeof(BITMAPFILEHEADER));
	memcpy(&m_iH, &rhs.m_iH, sizeof(BITMAPINFOHEADER));
}

CTerrainTex::~CTerrainTex()
{
}

// 만약에 선생님께서 이거 다르게 해결하지 않는다면 해결할 예정 경로 받아서 그때그떄 처리할수있도록
HRESULT CTerrainTex::Ready_Buffer(const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv)
{
	m_dwVtxCnt = dwCntX * dwCntZ;
	m_pPos = new _vec3[m_dwVtxCnt];
	m_dwTriCnt = (dwCntX - 1) * (dwCntZ - 1) * 2;
	m_dwVtxSize = sizeof(VTXTEX);
	m_dwFVF = FVF_TEX;

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	_ulong	dwByte = 0;

	m_hFile = CreateFile(L"../Bin/Resource/Texture/Terrain/Height.bmp", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NO_SCRUB_DATA, 0);

	ReadFile(m_hFile, &m_fH, sizeof(BITMAPFILEHEADER), &dwByte, nullptr);
	ReadFile(m_hFile, &m_iH, sizeof(BITMAPINFOHEADER), &dwByte, nullptr);

	_ulong*		pPixel = new _ulong[m_iH.biHeight * m_iH.biWidth];

	ReadFile(m_hFile, pPixel, sizeof(_ulong) * m_iH.biHeight * m_iH.biWidth, &dwByte, nullptr);

	CloseHandle(m_hFile);

	_ulong	dwIndex = 0;

	VTXTEX*		pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	for (_ulong i = 0; i < dwCntZ; ++i)
	{
		for (_ulong j = 0; j < dwCntX; ++j)
		{
			dwIndex = i * dwCntX + j;

			pVertex[dwIndex].vPos = { _float(j) * dwVtxItv,  0.f/*(pPixel[dwIndex] & 0x000000ff) / 20.f*/, _float(i) * dwVtxItv };
			m_pPos[dwIndex] = pVertex[dwIndex].vPos;
			pVertex[dwIndex].vNormal = { 0.f, 0.f, 0.f };
			pVertex[dwIndex].vTexUV = { _float(j) / (dwCntX - 1) * 128.f , _float(i) / (dwCntZ - 1) * 128.f };
		}
	}


	Safe_Delete_Array(pPixel);

	_ulong dwTriCnt = 0;

	INDEX32*		pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);


	for (_ulong i = 0; i < dwCntZ - 1; ++i)
	{
		for (_ulong j = 0; j < dwCntX - 1; ++j)
		{
			dwIndex = i * dwCntX + j;
			_vec3	vLook, vRight, vNormal;

			// 오른쪽 위
			pIndex[dwTriCnt]._0 = dwIndex + dwCntX;
			pIndex[dwTriCnt]._1 = dwIndex + dwCntX + 1;
			pIndex[dwTriCnt]._2 = dwIndex + 1;

			vLook = pVertex[pIndex[dwTriCnt]._1].vPos - pVertex[pIndex[dwTriCnt]._2].vPos;
			vRight = pVertex[pIndex[dwTriCnt]._1].vPos - pVertex[pIndex[dwTriCnt]._0].vPos;
			D3DXVec3Cross(&vNormal, &vLook, &vRight);

			pVertex[pIndex[dwTriCnt]._0].vNormal += vNormal;
			pVertex[pIndex[dwTriCnt]._1].vNormal += vNormal;
			pVertex[pIndex[dwTriCnt]._2].vNormal += vNormal;

			++dwTriCnt;

			// 왼쪽 아래
			pIndex[dwTriCnt]._0 = dwIndex + dwCntX;
			pIndex[dwTriCnt]._1 = dwIndex + 1;
			pIndex[dwTriCnt]._2 = dwIndex;

			vLook = pVertex[pIndex[dwTriCnt]._0].vPos - pVertex[pIndex[dwTriCnt]._2].vPos;
			vRight = pVertex[pIndex[dwTriCnt]._1].vPos - pVertex[pIndex[dwTriCnt]._2].vPos;
			D3DXVec3Cross(&vNormal, &vLook, &vRight);

			pVertex[pIndex[dwTriCnt]._0].vNormal += vNormal;
			pVertex[pIndex[dwTriCnt]._1].vNormal += vNormal;
			pVertex[pIndex[dwTriCnt]._2].vNormal += vNormal;

			++dwTriCnt;
		}
	}

	for (_uint i = 0; i < m_dwVtxCnt; ++i)
		D3DXVec3Normalize(&pVertex[i].vNormal, &pVertex[i].vNormal);

	m_pVB->Unlock();

	m_pIB->Unlock();

	return S_OK;
}

void CTerrainTex::Render_Buffer(void)
{
	CVIBuffer::Render_Buffer();
}

void CTerrainTex::CreateTextureFile()
{
	LPDIRECT3DTEXTURE9		pTexture = nullptr;
	D3DXCreateTexture(m_pGraphicDev, 129, 129, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &pTexture);

	D3DLOCKED_RECT		LockRect;

	pTexture->LockRect(0, &LockRect, nullptr, 0);

	for (_int i = 0; i < 129; ++i)
	{
		for (_int j = 0; j < 129; ++j)
		{
			_int	iIndex = i * 129 + j;
			*(((_ulong*)LockRect.pBits) + iIndex) = D3DCOLOR_ARGB(255, 255, 0, 0);
		}
	}

	pTexture->UnlockRect(0);

	D3DXSaveTextureToFile(L"../Bin/Resource/Texture/Test.bmp", D3DXIFF_BMP, pTexture, nullptr);
	// 빨갛게 칠해서 내보내겠다.
}

CTerrainTex * CTerrainTex::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv)
{
	CTerrainTex*	pInstance = new CTerrainTex(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer(dwCntX, dwCntZ, dwVtxItv)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CComponent * CTerrainTex::Clone(void)
{
	return new CTerrainTex(*this);
}

void CTerrainTex::Free(void)
{
	CVIBuffer::Free();

	if (m_bClone == false)
		Safe_Delete_Array(m_pPos);
}
