#include "..\..\Header\Texture.h"

USING(Engine)
#include "Export_Function.h"

CTexture::CTexture(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)
{
}


Engine::CTexture::CTexture(const CTexture& rhs)
	: CComponent(rhs)
{
	_uint iSize = rhs.m_vecTexture.size();
	m_vecTexture.reserve(iSize);

	m_vecTexture = rhs.m_vecTexture;

	for (_uint i = 0; i < iSize; ++i)
		m_vecTexture[i]->AddRef();

	_uint iSize2 = rhs.m_vecImageInfo.size();
	m_vecImageInfo.reserve(iSize2);

	m_vecImageInfo = rhs.m_vecImageInfo;
}

CTexture::~CTexture()
{
}

HRESULT CTexture::Ready_Texture(const _tchar * pPath, TEXTUREID eType, const _uint & iCnt)
{
	/*m_vecTexture.reserve(iCnt);

	IDirect3DBaseTexture9*         pTexture = nullptr;

	for (_uint i = 0; i < iCnt; ++i)
	{
	TCHAR      szFileName[256] = L"";

	wsprintf(szFileName, pPath, i);

	switch (eType)
	{
	case TEX_NORMAL:
	if (FAILED(D3DXCreateTextureFromFile(m_pGraphicDev, szFileName, (LPDIRECT3DTEXTURE9*)&pTexture)))
	return E_FAIL;

	break;
	*/


	m_vecTexture.reserve(iCnt);
	m_vecImageInfo.reserve(iCnt);

	D3DXIMAGE_INFO   tImgInfo; // 이미지 정보를 보관하기 위한 구조체 변수.
	ZeroMemory(&tImgInfo, sizeof(D3DXIMAGE_INFO));

	IDirect3DBaseTexture9*      pTexture = nullptr;

	for (_uint i = 0; i < iCnt; ++i)
	{
		TCHAR      szFileName[256] = L"";
		wsprintf(szFileName, pPath, i);

		// 이미지 정보 얻어오는 함수.
		FAILED_CHECK_RETURN_MSG(D3DXGetImageInfoFromFile(szFileName, &tImgInfo), E_FAIL, L"D3DXGetImageInfoFromFile Failed");

		switch (eType)
		{
		case TEX_NORMAL:
			//FAILED_CHECK_RETURN(D3DXCreateTextureFromFile(m_pGraphicDev, szFileName, (LPDIRECT3DTEXTURE9*)&pTexture), E_FAIL);

			FAILED_CHECK_RETURN(
				D3DXCreateTextureFromFileEx(m_pGraphicDev, szFileName, tImgInfo.Width, tImgInfo.Height, tImgInfo.MipLevels, 0, tImgInfo.Format, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, nullptr, nullptr, (LPDIRECT3DTEXTURE9*)&pTexture), E_FAIL);
			break;

		case TEX_CUBE:
			if (FAILED(D3DXCreateCubeTextureFromFile(m_pGraphicDev, szFileName, (LPDIRECT3DCUBETEXTURE9*)&pTexture)))
				return E_FAIL;
			break;
		}

		m_vecTexture.push_back(pTexture);
		m_vecImageInfo.push_back(tImgInfo);
	}

	return S_OK;
}

void CTexture::Set_Texture(const _uint & iIndex)
{
	if (m_vecTexture.size() <= iIndex)
		return;

	m_pGraphicDev->SetTexture(0, m_vecTexture[iIndex]);
}

void CTexture::Set_Texture(CShader * pShaderCom, D3DXHANDLE hHandle, const _uint & iIndex)
{
	if (nullptr == pShaderCom)
		return;

	pShaderCom->Set_Texture(hHandle, m_vecTexture[iIndex]);
}

const D3DXIMAGE_INFO & CTexture::Get_ImageInfo(const _uint & iIndex) const
{
	if (iIndex < 0 || iIndex >= m_vecImageInfo.size())
		return m_vecImageInfo[0];

	return m_vecImageInfo[iIndex];
}

CTexture * CTexture::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar * pPath, TEXTUREID eType, const _uint & iCnt)
{
	CTexture *      pInstance = new CTexture(pGraphicDev);


	if (FAILED(pInstance->Ready_Texture(pPath, eType, iCnt)))
	{
		MSG_BOX("Texture Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CComponent * CTexture::Clone(void)
{
	return new CTexture(*this);
}

void CTexture::Free(void)
{
	for (auto& iter : m_vecTexture)
		Safe_Release(iter);

	m_vecTexture.clear();

	CComponent::Free();
}