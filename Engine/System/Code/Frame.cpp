#include "Frame.h"
#include "Export_Function.h"

USING(Engine)

CFrame::CFrame()
	: m_fCallLimit(0.f), m_fAccTimeDelta(0.f)
{
}


CFrame::~CFrame()
{
	Free();
}

_bool CFrame::IsPermit_Call(const _float & fTimeDelta)
{
	m_fAccTimeDelta += fTimeDelta;

	if (m_fAccTimeDelta >= m_fCallLimit)
	{
		m_fAccTimeDelta = 0.f;
		return true;
	}

	return false;
}

HRESULT CFrame::Ready_Frame(const _float & fCallLimit)
{
	m_fCallLimit = 1.f / fCallLimit;

	return S_OK;
}

CFrame* CFrame::Create(const _float & fCallLimit)
{
	CFrame*		pInstance = new CFrame;

	if (FAILED(pInstance->Ready_Frame(fCallLimit)))
	{
		MSG_BOX("Frame Create Failed");
		Safe_Release(pInstance);
	}
	
	return pInstance;
}

void CFrame::Free(void)
{
}
