#pragma once
#include "Component.h"
#include "Engine_Include.h"

BEGIN(Engine)

/* 외부에서 작성된 셰이더코드(파일)를 번역하여 객체(ID3DXEffect)화 시킨다. */
class ENGINE_DLL CShader : public CComponent
{
public:
	explicit CShader(LPDIRECT3DDEVICE9 pDevice);
	explicit CShader(const CShader& rhs);
	virtual ~CShader() = default;

public:
	HRESULT Ready_Shader(const _tchar* pShaderFilePath);

	/* 이제부터 셰이더로 렌더링한다. */
	HRESULT Begin_Shader(_uint iPassIndex);

	/* 정점버퍼의 렌더함수를 호출한다면 Transform에 대한 기능으 ㄴ사라진다. */

	HRESULT End_Shader();

public:
	HRESULT Set_RawValue(D3DXHANDLE hHandle, const void* pData, _uint iLength);
	HRESULT Set_Texture(D3DXHANDLE hHandle, IDirect3DBaseTexture9* pTexture);

private:
	LPD3DXEFFECT				m_pEffect = nullptr;

public:
	static CShader*		Create(LPDIRECT3DDEVICE9 pDevice, const _tchar* pShaderFilePath);
	virtual CComponent*	Clone(void);

private:
	virtual void Free(void);
};

END