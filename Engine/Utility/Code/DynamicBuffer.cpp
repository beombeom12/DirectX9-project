#include "..\..\Header\DynamicBuffer.h"
#include "Export_Function.h"

#define	STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

USING(Engine)

CDynamicBuffer::CDynamicBuffer(LPDIRECT3DDEVICE9 pGraphicDev)
	:CVIBuffer(pGraphicDev)
{
}

CDynamicBuffer::CDynamicBuffer(const CDynamicBuffer & rhs)
	:CVIBuffer(rhs)
{
}

CDynamicBuffer::~CDynamicBuffer()
{
}

HRESULT CDynamicBuffer::Ready_Buffer(const _tchar* _path, _float fCellSize, _float fUVX, _float fUVY)
{
	char* szFileName;
	_int strSize = WideCharToMultiByte(CP_ACP, 0, _path, -1, NULL, 0, NULL, NULL);
	szFileName = new char[strSize];
	WideCharToMultiByte(CP_ACP, 0, _path, -1, szFileName, strSize, 0, 0);

	CVIBuffer::Ready_Buffer();

	_int iWidth, iHeight, iComp;
	unsigned char* pPixel = stbi_load(szFileName, &iWidth, &iHeight, &iComp, 0);

	if (iComp < 4)
	{
		MSG_BOX("CDynamicBuffer Texture is not png");
		return E_FAIL;
	}

	m_dwVtxCnt = 0;
	m_dwTriCnt = 0;
	m_dwVtxSize = sizeof(VTXCUBE2);
	m_dwFVF = FVF_CUBE2;

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	enum Cube { LOOK, BACK, LEFT, RIGHT, UP, DOWN, CUBE_END};

	_float w2 = 0.5f;
	_float h2 = 0.5f;
	_float d2 = 0.5f;

	VTXCUBE2 tVtxCubeDefault[CUBE_END][4]
	{
		{
			{ { -w2, -h2, +d2 },{} },
			{ { +w2, -h2, +d2 },{} },
			{ { +w2, +h2, +d2 },{} },
			{ { -w2, +h2, +d2 },{} }
		}, // look
		{
			{ { -w2, -h2, -d2 },{} },
			{ { -w2, +h2, -d2 },{} },
			{ { +w2, +h2, -d2 },{} },
			{ { +w2, -h2, -d2 },{} }
		}, // back
		{
			{ { -w2, -h2, +d2 },{} },
			{ { -w2, +h2, +d2 },{} },
			{ { -w2, +h2, -d2 },{} },
			{ { -w2, -h2, -d2 },{} }
		}, // left
		{
			{ { +w2, -h2, -d2 },{} },
			{ { +w2, +h2, -d2 },{} },
			{ { +w2, +h2, +d2 },{} },
			{ { +w2, -h2, +d2 },{} }
		}, // right
		{
			{ { -w2, +h2, -d2 },{} },
			{ { -w2, +h2, +d2 },{} },
			{ { +w2, +h2, +d2 },{} },
			{ { +w2, +h2, -d2 },{} }
		}, // up
		{
			{ { -w2, -h2, -d2 },{} },
			{ { +w2, -h2, -d2 },{} },
			{ { +w2, -h2, +d2 },{} },
			{ { -w2, -h2, +d2 },{} }
		}, // down
	};

	// 사이즈 조절
	for (_int i = 0; i < CUBE_END; ++i)
	{
		for (_int j = 0; j < 4; ++j)
		{
			tVtxCubeDefault[i][j].vPos.x *= fCellSize;
			tVtxCubeDefault[i][j].vPos.y *= fCellSize;
			tVtxCubeDefault[i][j].vPos.z *= fCellSize;
		}
	}

	vector<VTXCUBE2> vecVtxCube2;
	vector<INDEX32>  vecIdx;

	for (_int i = 0; i < iHeight; ++i) // x 축
	{
		for (_int j = 0; j < iWidth; ++j) // z 축
		{
			const _int iIdx = iComp * (i * iWidth + j);
			const _int iAlpha = pPixel[iIdx + 3];
			if (iAlpha == 0) 
				continue;

			_vec2 vUV[4]
			{
				{ (_float)j / iHeight * fUVX,		(_float)i / iWidth * fUVY},
				{ (_float)j / iHeight * fUVX,		(_float)(i + 1) / iWidth * fUVY},
				{ (_float)(j + 1) / iHeight * fUVX, (_float)(i + 1) / iWidth * fUVY },
				{ (_float)(j + 1) / iHeight * fUVX, (_float)i / iWidth * fUVY }
			};

			VTXCUBE2 tVtxTmp[CUBE_END][4];
			::memcpy(tVtxTmp, tVtxCubeDefault, sizeof(tVtxTmp));

			for (_int w = 0; w < CUBE_END; ++w)
			{
				for (size_t k = 0; k < 4; ++k)
				{
					tVtxTmp[w][k].vPos.x += (_float)i * fCellSize;
					tVtxTmp[w][k].vPos.z += (_float)j * fCellSize;
					tVtxTmp[w][k].vTexUV = vUV[k];
					vecVtxCube2.push_back(tVtxTmp[w][k]);
				}
				vecIdx.push_back({ m_dwVtxCnt, m_dwVtxCnt + 1, m_dwVtxCnt + 2 });
				vecIdx.push_back({ m_dwVtxCnt, m_dwVtxCnt + 2, m_dwVtxCnt + 3 });
				m_dwVtxCnt += 4;
				m_dwTriCnt += 2;
			}
		}
	}

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	VTXCUBE2* pVertex = nullptr;
	INDEX32* pIndex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);
	memcpy(pVertex, vecVtxCube2.data(), sizeof(VTXCUBE2) * vecVtxCube2.size());
	m_pVB->Unlock();

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);
	memcpy(pIndex, vecIdx.data(), sizeof(INDEX32) * vecIdx.size());
	m_pIB->Unlock();

	stbi_image_free(pPixel);
	delete[] szFileName;
	return S_OK;
}

void CDynamicBuffer::Render_Buffer()
{
	CVIBuffer::Render_Buffer();
}

CDynamicBuffer * CDynamicBuffer::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* _path, _float fCellSize, _float fUVX, _float fUVY)
{
	CDynamicBuffer*	pInstance = new CDynamicBuffer(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer(_path, fCellSize, fUVX, fUVY)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CComponent* CDynamicBuffer::Clone()
{
	return new CDynamicBuffer(*this);
}

void CDynamicBuffer::Free()
{
	CVIBuffer::Free();
}
