#include "GraphicDev.h"

USING(Engine)
IMPLEMENT_SINGLETON(CGraphicDev)

CGraphicDev::CGraphicDev()
	: m_pGraphicDev(nullptr)
	, m_pSDK(nullptr)
{
}


CGraphicDev::~CGraphicDev()
{
	Free();
}

HRESULT CGraphicDev::Ready_GraphicDev(HWND hWnd, WINMODE eMode, 
									const _uint & iSizeX, const _uint & iSizeY,
									CGraphicDev** ppGraphicDev)
{
	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION);
	NULL_CHECK_RETURN(m_pSDK, E_FAIL);

	D3DCAPS9		DeviceCaps;
	ZeroMemory(&DeviceCaps, sizeof(D3DCAPS9));

	FAILED_CHECK_RETURN(m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &DeviceCaps), E_FAIL);

	_ulong	dwFlag = 0;

	if (DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		dwFlag |= D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;

	else
		dwFlag |= D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;

	D3DPRESENT_PARAMETERS		Present_Parameters;
	ZeroMemory(&Present_Parameters, sizeof(D3DPRESENT_PARAMETERS));

	Present_Parameters.BackBufferWidth = iSizeX;
	Present_Parameters.BackBufferHeight = iSizeY;
	Present_Parameters.BackBufferFormat = D3DFMT_A8R8G8B8;
	Present_Parameters.BackBufferCount = 1;

	Present_Parameters.MultiSampleType = D3DMULTISAMPLE_NONE;
	Present_Parameters.MultiSampleQuality = 0;

	Present_Parameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	Present_Parameters.hDeviceWindow = hWnd;

	Present_Parameters.Windowed = eMode;

	Present_Parameters.EnableAutoDepthStencil = TRUE;
	Present_Parameters.AutoDepthStencilFormat = D3DFMT_D24S8;

	Present_Parameters.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	Present_Parameters.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	FAILED_CHECK_RETURN(m_pSDK->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, dwFlag, &Present_Parameters, &m_pGraphicDev), E_FAIL);

	*ppGraphicDev = this;

	return S_OK;
}

void CGraphicDev::Render_Begin(D3DXCOLOR Color)
{
	m_pGraphicDev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER, Color, 1.f, 0);
	m_pGraphicDev->BeginScene();
}

void CGraphicDev::Render_End(HWND hWnd)
{
	m_pGraphicDev->EndScene();
	m_pGraphicDev->Present(nullptr, nullptr, hWnd, nullptr);
}

void Engine::CGraphicDev::Free(void)
{
	_ulong	dwRefCnt = 0;

	if (dwRefCnt = Safe_Release<LPDIRECT3DDEVICE9>(m_pGraphicDev))
		MSG_BOX("m_pGraphicDev Release Failed");

	if (dwRefCnt = Safe_Release<LPDIRECT3D9>(m_pSDK))
		MSG_BOX("m_pSDK Release Failed");
}
