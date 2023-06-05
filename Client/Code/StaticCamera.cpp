#include "stdafx.h"
#include "..\Header\StaticCamera.h"
#include "CubePlayer.h"
#include "Weapon.h"
#include "Inventory.h"
#include "Shop.h"
#include "PlayerMapping.h"
#include "Flight.h"
#include "FlightCamera.h"
#include "Supporter_Uzi.h"
#include "Supporter_Shotgun.h"
#include "Supporter_Sniper.h"
#include "LetterBox.h"

CStaticCamera::CStaticCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCamera(pGraphicDev)
{
}

CStaticCamera::~CStaticCamera()
{
}

HRESULT CStaticCamera::Ready_Object(const _vec3* pEye,
	const _vec3* pAt,
	const _vec3* pUp,
	const _float& fFov,
	const _float& fAspect,
	const _float& fNear,
	const _float& fFar)
{
	m_vEye = *pEye;
	m_vAt = *pAt;
	m_vUp = *pUp;

	m_fFov = fFov;
	m_fAspect = fAspect;
	m_fNear = fNear;
	m_fFar = fFar;

	m_fPlayerFrame = 0.f;

	m_bMainCameraOn = true;

	m_fDistance = 10.f;

	FAILED_CHECK_RETURN(CCamera::Ready_Object(), E_FAIL);

	return S_OK;
}

void CStaticCamera::Update_NullCheck()
{
	if (nullptr == m_pTransform_Target)
	{
		m_pTransform_Target = dynamic_cast<CTransform*>(Engine::Get_Component(STAGE_CHARACTER, L"HEAD", TRANSFORM_COMP, ID_DYNAMIC));
		NULL_CHECK(m_pTransform_Target);
	}

	if (nullptr == m_pBombTransform)
	{
		m_pBombTransform = dynamic_cast<CTransform*>(Engine::Get_Component(STAGE_MAPPING, L"Map", L"Bomb_TransCom", ID_DYNAMIC));
		NULL_CHECK(m_pBombTransform);
	}

	if (nullptr == m_pFlightTransform)
	{
		m_pFlightTransform = dynamic_cast<CTransform*>(Engine::Get_Component(STAGE_FLIGHTPLAYER, L"FLIGHTPLAYER", TRANSFORM_COMP, ID_STATIC));
		NULL_CHECK(m_pFlightTransform);
	}

	if (nullptr == m_pShuttleTransform)
	{
		m_pShuttleTransform = dynamic_cast<CTransform*>(Engine::Get_Component(STAGE_FLIGHTPLAYER, L"FLIGHTSHUTTLE", TRANSFORM_COMP, ID_STATIC));
		NULL_CHECK(m_pShuttleTransform);
	}

	if (nullptr == m_pSupporterUziTransform)
	{
		m_pSupporterUziTransform = dynamic_cast<CTransform*>(Engine::Get_Component(STAGE_SUPPORTER, L"SUPPORT_UZI", TRANSFORM_COMP, ID_DYNAMIC));
		m_pSupUzi = dynamic_cast<CSupporter_Uzi*>(Get_GameObject(STAGE_SUPPORTER, L"SUPPORT_UZI"));
	}

	if (nullptr == m_pSupporterShotgunTransform)
	{
		m_pSupporterShotgunTransform = dynamic_cast<CTransform*>(Engine::Get_Component(STAGE_SUPPORTER, L"SUPPORT_SHOTGUN", TRANSFORM_COMP, ID_DYNAMIC));
		m_pSupShotgun = dynamic_cast<CSupporter_Shotgun*>(Get_GameObject(STAGE_SUPPORTER, L"SUPPORT_SHOTGUN"));
	}

	if (nullptr == m_pSupporterSniperTransform)
	{
		m_pSupporterSniperTransform = dynamic_cast<CTransform*>(Engine::Get_Component(STAGE_SUPPORTER, L"SUPPORT_SNIPER", TRANSFORM_COMP, ID_DYNAMIC));
		m_pSupSniper = dynamic_cast<CSupporter_Sniper*>(Get_GameObject(STAGE_SUPPORTER, L"SUPPORT_SNIPER"));
	}
}

Engine::_int CStaticCamera::Update_Object(const _float& fTimeDelta)
{
	Update_NullCheck();

	if (!m_bMainCameraOn)
		return 0;

	if (!(dynamic_cast<CInventory*>(Engine::Get_GameObject(STAGE_UI, L"InventoryUI"))->Get_Switch())
		&& !(dynamic_cast<CShop*>(Engine::Get_GameObject(STAGE_UI, L"Shop"))->Get_Switch()))
	{
		Key_Input(fTimeDelta);

		Look_Target(fTimeDelta);

		Camera_Shaking(fTimeDelta);

		Mouse_Fix();
	}

	//m_fFrame += fTimeDelta * 0.5f;
	//m_fFlightFrame += fTimeDelta * 0.5f;
	//m_fBombFrame += fTimeDelta * 0.5f;
	//m_fShuttleFrame += fTimeDelta * 0.15f;
	m_fPlayerFrame += fTimeDelta * 0.05f;
	m_fPlayerFrame2 += fTimeDelta * 0.1f;
	m_fFlightFrame += fTimeDelta;
	m_fBombFrame += fTimeDelta;
	m_fShuttleFrame += fTimeDelta;
	m_fEndingFrame += fTimeDelta * 0.1f;
	//m_fPlayerFrame += fTimeDelta;
	_int   iExit = CCamera::Update_Object(fTimeDelta);

	return iExit;
}

void CStaticCamera::LateUpdate_Object(void)
{
	if (!m_bMainCameraOn)
		return;

	CCamera::LateUpdate_Object();
}

void CStaticCamera::Free(void)
{
	CCamera::Free();
}

CStaticCamera* CStaticCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFov /*= D3DXToRadian(60.f)*/, const _float& fAspect /*= (float)WINCX / WINCY*/, const _float& fNear /*= 0.1f*/, const _float& fFar /*= 1000.f*/)
{
	CStaticCamera*      pInstance = new CStaticCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pEye, pAt, pUp, fFov, fAspect, fNear, fFar)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CStaticCamera::Key_Input(const _float& fTimeDelta)
{
	if (Key_Down(DIK_V))
		m_bChangePOV = !m_bChangePOV;
}

void CStaticCamera::Mouse_Fix(void)
{
	if (!(dynamic_cast<CInventory*>(Engine::Get_GameObject(STAGE_UI, L"InventoryUI"))->Get_Switch()) && !(dynamic_cast<CShop*>(Engine::Get_GameObject(STAGE_UI, L"Shop"))->Get_Switch()))
	{
		ShowCursor(false);
		POINT   pt{ WINCX >> 1 , WINCY >> 1 };

		ClientToScreen(g_hWnd, &pt);
		SetCursorPos(pt.x, pt.y);
	}
}

void CStaticCamera::Look_Target(const _float& _fTimeDelta)
{
	CGameObject* pPlayer = nullptr;
	pPlayer = Engine::Get_GameObject(STAGE_CHARACTER, L"PLAYER");

	CGameObject* pBomb = nullptr;
	pBomb = Engine::Get_GameObject(STAGE_MAPPING, L"Map");

	CGameObject* pFlight = nullptr;
	pFlight = Engine::Get_GameObject(STAGE_FLIGHTPLAYER, L"FLIGHTPLAYER");

	CGameObject* pShuttle = nullptr;
	pShuttle = Engine::Get_GameObject(STAGE_FLIGHTPLAYER, L"FLIGHTSHUTTLE");

	CGameObject* pUZI = nullptr;
	pUZI = Engine::Get_GameObject(STAGE_SUPPORTER, L"FLIGHTSHUTTLE");

	if (dynamic_cast<CFlight*>(pShuttle)->Get_Ending() == true && Get_Scene()->Get_SceneId() == STAGE_SCENE)
	{
		_vec3 vLook;
		m_pShuttleTransform->Get_Info(INFO_LOOK, &vLook);

		_vec3 vRight;
		m_pShuttleTransform->Get_Info(INFO_RIGHT, &vRight);

		_vec3 vUp;
		m_pShuttleTransform->Get_Info(INFO_UP, &vUp);

		m_vEye = (vRight * 1.f);
		D3DXVec3Normalize(&m_vEye, &m_vEye);
		m_vEye *= 5.f;

		_vec3 vShuttlePos;
		m_pShuttleTransform->Get_Info(INFO_POS, &vShuttlePos);

		_vec3 vPlayerPos;
		m_pTransform_Target->Get_Info(INFO_POS, &vPlayerPos);

		_vec3 vTransLerp;
		D3DXVec3Lerp(&vTransLerp, &vPlayerPos, &vShuttlePos, m_fEndingFrame);

		if (m_fEndingFrame < 1.f)
		{
			m_fFov = D3DXToRadian(60.f);
			m_vEye += vTransLerp + (vRight * m_fShuttleFrame * 10.f) + (vUp * m_fShuttleFrame * 5.f);
			m_vAt = vShuttlePos;//vTransLerp + (vRight* m_fShuttleFrame * 2.f);
		}
		else
		{
			m_fFov = D3DXToRadian(60.f);
			m_vEye += vShuttlePos + (vRight * 10.f) + (vUp * 5.f);
			m_vAt = vShuttlePos + (vRight * 2.f);
		}
	}
	else if (dynamic_cast<CFlight*>(pShuttle)->Get_Ending() == true && Get_Scene()->Get_SceneId() == FINAL_SCENE)
	{
		_vec3 vLook;
		m_pShuttleTransform->Get_Info(INFO_LOOK, &vLook);

		_vec3 vRight;
		m_pShuttleTransform->Get_Info(INFO_RIGHT, &vRight);

		_vec3 vUp;
		m_pShuttleTransform->Get_Info(INFO_UP, &vUp);

		if (!m_bEnding)
		{
			m_vEye = (vRight * 1.f);
			D3DXVec3Normalize(&m_vEye, &m_vEye);
			m_vEye *= 5.f;
		}
		else
		{
			m_vEye = (-vLook * 1.f);
			D3DXVec3Normalize(&m_vEye, &m_vEye);
			m_vEye *= 15.f;
		}

		_vec3 vShuttlePos;
		m_pShuttleTransform->Get_Info(INFO_POS, &vShuttlePos);

		_vec3 vPlayerPos;
		m_pTransform_Target->Get_Info(INFO_POS, &vPlayerPos);

		_vec3 vTransLerp;
		D3DXVec3Lerp(&vTransLerp, &vPlayerPos, &vShuttlePos, m_fEndingFrame);

		if (m_fEndingFrame < 1.f)
		{
			m_fFov = D3DXToRadian(60.f);
			m_vEye += vTransLerp + (vRight * m_fShuttleFrame * 10.f) + (vUp * m_fShuttleFrame * 5.f);
			m_vAt = vShuttlePos;//vTransLerp + (vRight* m_fShuttleFrame * 2.f);
		}
		else
		{
			m_bEnding = true;
			m_fFov = D3DXToRadian(60.f);
			m_vEye += vShuttlePos + (vRight * 10.f) + (vUp * 5.f);
			m_vAt = vShuttlePos + (vRight * 2.f);
		}
	}
	else if (m_pSupporterUziTransform != nullptr && m_pSupUzi != nullptr
		&& dynamic_cast<CSupporter_Uzi*>(m_pSupUzi)->Get_setcam() == true)
	{
		_vec3 vLook;
		m_pSupporterUziTransform->Get_Info(INFO_LOOK, &vLook);

		_vec3 vRight;
		m_pSupporterUziTransform->Get_Info(INFO_RIGHT, &vRight);

		_vec3 vUp;
		m_pSupporterUziTransform->Get_Info(INFO_UP, &vUp);

		m_vEye = (vLook * 1.f);
		D3DXVec3Normalize(&m_vEye, &m_vEye);
		m_vEye *= 2.f;

		_vec3 vSupUziPos;
		m_pSupporterUziTransform->Get_Info(INFO_POS, &vSupUziPos);

		m_fFov = D3DXToRadian(60.f);
		m_vEye += vSupUziPos + (vLook * 10.f);
		m_vAt = vSupUziPos + (vLook * 2.f);
	}
	else if (m_pSupporterShotgunTransform != nullptr && m_pSupShotgun != nullptr
		&& dynamic_cast<CSupporter_Shotgun*>(m_pSupShotgun)->Get_setcam() == true)
	{
		_vec3 vLook;
		m_pSupporterShotgunTransform->Get_Info(INFO_LOOK, &vLook);

		_vec3 vRight;
		m_pSupporterShotgunTransform->Get_Info(INFO_RIGHT, &vRight);

		_vec3 vUp;
		m_pSupporterShotgunTransform->Get_Info(INFO_UP, &vUp);

		m_vEye = (vLook * 1.f);
		D3DXVec3Normalize(&m_vEye, &m_vEye);
		m_vEye *= 2.f;

		_vec3 vSupShotgunPos;
		m_pSupporterShotgunTransform->Get_Info(INFO_POS, &vSupShotgunPos);

		m_fFov = D3DXToRadian(60.f);
		m_vEye += vSupShotgunPos + (vLook * 10.f);
		m_vAt = vSupShotgunPos + (vLook * 2.f);
	}
	else if (m_pSupporterSniperTransform != nullptr && m_pSupSniper != nullptr
		&& dynamic_cast<CSupporter_Sniper*>(m_pSupSniper)->Get_setcam() == true)
	{
		_vec3 vLook;
		m_pSupporterSniperTransform->Get_Info(INFO_LOOK, &vLook);

		_vec3 vRight;
		m_pSupporterSniperTransform->Get_Info(INFO_RIGHT, &vRight);

		_vec3 vUp;
		m_pSupporterSniperTransform->Get_Info(INFO_UP, &vUp);

		m_vEye = (vLook * 1.f);
		D3DXVec3Normalize(&m_vEye, &m_vEye);
		m_vEye *= 2.f;

		_vec3 vSupSniperPos;
		m_pSupporterSniperTransform->Get_Info(INFO_POS, &vSupSniperPos);

		m_fFov = D3DXToRadian(60.f);
		m_vEye += vSupSniperPos + (vLook * 10.f);
		m_vAt = vSupSniperPos + (vLook * 2.f);
	}
	else if (!m_bChangePOV && (dynamic_cast<CCubePlayer*>(pPlayer)->Get_Weapon() == dynamic_cast<CWeapon*>(Engine::Get_GameObject(STAGE_GUN, L"UZI1")) ||
		dynamic_cast<CCubePlayer*>(pPlayer)->Get_Weapon() == dynamic_cast<CWeapon*>(Engine::Get_GameObject(STAGE_GUN, L"SHOTGUN")))
		&& dynamic_cast<CPlayerMapping*>(pBomb)->Get_WorldMap() == false && dynamic_cast<CFlight*>(pFlight)->Get_Control() == false
		&& dynamic_cast<CFlight*>(pShuttle)->Get_Shuttle() == false)
	{
		_vec3 vLook;
		m_pTransform_Target->Get_Info(INFO_LOOK, &vLook);

		_vec3 vRight;
		m_pTransform_Target->Get_Info(INFO_RIGHT, &vRight);

		_vec3 vUp;
		m_pTransform_Target->Get_Info(INFO_UP, &vUp);

		m_vEye = (vLook * -1.f);
		D3DXVec3Normalize(&m_vEye, &m_vEye);
		m_vEye *= 0.1f;

		_vec3 vPos;
		m_pTransform_Target->Get_Info(INFO_POS, &vPos);

		m_fFov = D3DXToRadian(60.f);
		m_vEye += vPos;
		m_vAt = vPos;
	}
	else if (m_bChangePOV && (dynamic_cast<CCubePlayer*>(pPlayer)->Get_Weapon() == dynamic_cast<CWeapon*>(Engine::Get_GameObject(STAGE_GUN, L"UZI1")) ||
		dynamic_cast<CCubePlayer*>(pPlayer)->Get_Weapon() == dynamic_cast<CWeapon*>(Engine::Get_GameObject(STAGE_GUN, L"SHOTGUN")))
		&& dynamic_cast<CPlayerMapping*>(pBomb)->Get_WorldMap() == false && dynamic_cast<CFlight*>(pFlight)->Get_Control() == false
		&& dynamic_cast<CFlight*>(pShuttle)->Get_Shuttle() == false)
	{
		_vec3 vLook;
		m_pTransform_Target->Get_Info(INFO_LOOK, &vLook);

		_vec3 vRight;
		m_pTransform_Target->Get_Info(INFO_RIGHT, &vRight);

		_vec3 vUp;
		m_pTransform_Target->Get_Info(INFO_UP, &vUp);

		m_vEye = (vLook * -1.f);
		D3DXVec3Normalize(&m_vEye, &m_vEye);
		m_vEye *= 1.5f;

		_vec3 vPos;
		m_pTransform_Target->Get_Info(INFO_POS, &vPos);

		D3DXVec3Normalize(&vRight, &vRight);

		m_fFov = D3DXToRadian(60.f);
		m_vEye += vPos + (vRight * 0.5f);
		m_vAt = vPos + (vRight * 0.5f);
	}
	else if (dynamic_cast<CCubePlayer*>(pPlayer)->Get_SniperZoom() == true
		&& dynamic_cast<CPlayerMapping*>(pBomb)->Get_WorldMap() == false
		&& dynamic_cast<CFlight*>(pFlight)->Get_Control() == false
		&& dynamic_cast<CFlight*>(pShuttle)->Get_Shuttle() == false)
	{
		_vec3 vLook;
		m_pTransform_Target->Get_Info(INFO_LOOK, &vLook);

		_vec3 vRight;
		m_pTransform_Target->Get_Info(INFO_RIGHT, &vRight);

		_vec3 vUp;
		m_pTransform_Target->Get_Info(INFO_UP, &vUp);

		m_vEye = (vLook * -1.f);

		_vec3 vPos;
		m_pTransform_Target->Get_Info(INFO_POS, &vPos);

		D3DXVec3Normalize(&vLook, &vLook);

		m_fFov = D3DXToRadian(15.f);
		m_vEye += vPos;
		m_vAt = vPos + (vLook * 10.f);
	}
	else if (dynamic_cast<CPlayerMapping*>(pBomb)->Get_WorldMap() == true
		&& dynamic_cast<CFlight*>(pFlight)->Get_Control() == false
		&& dynamic_cast<CFlight*>(pShuttle)->Get_Shuttle() == false)
	{
		_vec3 vLook;
		m_pBombTransform->Get_Info(INFO_LOOK, &vLook);

		_vec3 vRight;
		m_pBombTransform->Get_Info(INFO_RIGHT, &vRight);

		_vec3 vUp;
		m_pBombTransform->Get_Info(INFO_UP, &vUp);

		m_vEye = (vUp * -1.f);
		D3DXVec3Normalize(&m_vEye, &m_vEye);
		m_vEye *= 13.f;

		_vec3 vBombPos;
		m_pBombTransform->Get_Info(INFO_POS, &vBombPos);

		_vec3 vPlayerPos;
		m_pTransform_Target->Get_Info(INFO_POS, &vPlayerPos);

		_vec3 vTransLerp;
		D3DXVec3Lerp(&vTransLerp, &vPlayerPos, &vBombPos, m_fBombFrame);

		if (m_fBombFrame < 1.f)
		{
			m_fFov = D3DXToRadian(60.f);
			m_vEye += vTransLerp + (-vLook * m_fBombFrame * 20.f);
			m_vAt = vTransLerp;
		}
		else
		{
			m_fFov = D3DXToRadian(60.f);
			m_vEye += vBombPos + (-vLook * 20.f);
			m_vAt = vBombPos;
		}
	}
	else if (dynamic_cast<CFlight*>(pFlight)->Get_Control() == true
		&& dynamic_cast<CFlight*>(pShuttle)->Get_Shuttle() == false)
	{
		_vec3 vLook;
		m_pFlightTransform->Get_Info(INFO_LOOK, &vLook);

		_vec3 vRight;
		m_pFlightTransform->Get_Info(INFO_RIGHT, &vRight);

		_vec3 vUp;
		m_pFlightTransform->Get_Info(INFO_UP, &vUp);

		m_vEye = (vUp * 1.f);
		D3DXVec3Normalize(&m_vEye, &m_vEye);
		m_vEye *= 5.f;

		_vec3 vFlightPos;
		m_pFlightTransform->Get_Info(INFO_POS, &vFlightPos);

		_vec3 vPlayerPos;
		m_pTransform_Target->Get_Info(INFO_POS, &vPlayerPos);

		_vec3 vTransLerp;
		D3DXVec3Lerp(&vTransLerp, &vPlayerPos, &vFlightPos, m_fFlightFrame);

		if (m_fFlightFrame < 1.f)
		{
			m_fFov = D3DXToRadian(60.f);
			m_vEye += vTransLerp + (vLook * m_fFlightFrame * 10.f);
			m_vAt = vFlightPos;//vTransLerp + (vUp* m_fFlightFrame * 2.f) + (-vLook * m_fFlightFrame * 1.f);
		}
		else
		{
			Set_MainCam(false);
			static_cast<CFlightCamera*>(Engine::Get_GameObject(STAGE_ENVIRONMENT, L"FlightCamera"))->Set_MainCam(true);
			//static_cast<CFlightCamera*>(Engine::Get_GameObject(STAGE_ENVIRONMENT, L"FlightCamera"))->Set_At(&(vFlightPos + (vUp * 2.f) + (-vLook * 1.f)));
			static_cast<CFlightCamera*>(Engine::Get_GameObject(STAGE_ENVIRONMENT, L"FlightCamera"))->Set_Eye(&(vFlightPos + (vLook * -10.f) + (vUp * 3.f)));
		}
	}
	else if (dynamic_cast<CFlight*>(pShuttle)->Get_Shuttle() == true)
	{
		_vec3 vLook;
		m_pShuttleTransform->Get_Info(INFO_LOOK, &vLook);

		_vec3 vRight;
		m_pShuttleTransform->Get_Info(INFO_RIGHT, &vRight);

		_vec3 vUp;
		m_pShuttleTransform->Get_Info(INFO_UP, &vUp);

		m_vEye = (vRight * 1.f);
		D3DXVec3Normalize(&m_vEye, &m_vEye);
		m_vEye *= 5.f;

		_vec3 vShuttlePos;
		m_pShuttleTransform->Get_Info(INFO_POS, &vShuttlePos);

		_vec3 vPlayerPos;
		m_pTransform_Target->Get_Info(INFO_POS, &vPlayerPos);

		_vec3 vTransLerp;
		D3DXVec3Lerp(&vTransLerp, &vPlayerPos, &vShuttlePos, m_fShuttleFrame);

		if (m_fShuttleFrame < 1.f)
		{
			m_fFov = D3DXToRadian(60.f);
			m_vEye += vTransLerp + (vRight * m_fShuttleFrame * 10.f) + (vUp * m_fShuttleFrame * 5.f);
			m_vAt = vShuttlePos;//vTransLerp + (vRight* m_fShuttleFrame * 2.f);
		}
		else
		{
			m_fFov = D3DXToRadian(60.f);
			m_vEye += vShuttlePos + (vRight * 10.f) + (vUp * 5.f);
			m_vAt = vShuttlePos + (vRight * 2.f);
		}
	}
	else
	{
		if (Get_Scene()->Get_SceneId() == STAGE_SCENE)
		{
			_vec3 vLook;
			m_pTransform_Target->Get_Info(INFO_LOOK, &vLook);

			_vec3 vRight;
			m_pTransform_Target->Get_Info(INFO_RIGHT, &vRight);

			_vec3 vUp;
			m_pTransform_Target->Get_Info(INFO_UP, &vUp);

			m_vEye = (vLook * -1.f);
			D3DXVec3Normalize(&m_vEye, &m_vEye);
			m_vEye *= 0.1f;

			_vec3 vPos;
			m_pTransform_Target->Get_Info(INFO_POS, &vPos);

			_vec3 vTransLerp;
			D3DXVec3Lerp(&vTransLerp, &_vec3(0.f, 50.f, -10.f), &vPos, m_fPlayerFrame);

			if (m_fPlayerFrame < 1.f)
			{
				if (!m_bLetterBox)
				{
					if (0.f < m_fPlayerFrame && m_fPlayerFrame < 0.3f)
					{
						m_pLetterBox = CLetterBox::Create(m_pGraphicDev, L"Where am I?", sizeof(L"Where am I?"), 0);
						m_bLetterBox = true;
					}
				}
				else if (0.3f < m_fPlayerFrame && m_fPlayerFrame < 0.6f)
				{
					m_pLetterBox->ChangeLetterContents(L"Is it a dream or a reality?", sizeof(L"Is it a dream or a reality?"));
				}
				else if (0.89f > m_fPlayerFrame && m_fPlayerFrame > 0.6f)
				{
					m_pLetterBox->ChangeLetterContents(L"Oh my god!!!", sizeof(L"Oh my god!!!"));
					
				}
				else if (m_fPlayerFrame > 0.9f)
				{
					m_pLetterBox->LetterDead();
					m_bFirst = true;
				}

				m_fFov = D3DXToRadian(60.f);
				m_vEye += vTransLerp * m_fPlayerFrame;
				m_vAt = vTransLerp + (-vUp * m_fPlayerFrame * 2.f);
			}
			else
			{
				if (!m_bFirstSound)
				{
					Engine::PlaySoundW(L"NPC.wav", SOUND_EFFECT, 1.f);
					m_bFirstSound = true;
				}

				m_fFov = D3DXToRadian(60.f);
				m_vEye += vPos;
				m_vAt = vPos;
			}
		}
		else
		{
			_vec3 vLook;
			m_pTransform_Target->Get_Info(INFO_LOOK, &vLook);

			_vec3 vRight;
			m_pTransform_Target->Get_Info(INFO_RIGHT, &vRight);

			_vec3 vUp;
			m_pTransform_Target->Get_Info(INFO_UP, &vUp);

			m_vEye = (vLook * -1.f);
			D3DXVec3Normalize(&m_vEye, &m_vEye);
			m_vEye *= 0.1f;

			_vec3 vPos;
			m_pTransform_Target->Get_Info(INFO_POS, &vPos);

			_vec3 vTransLerp;
			D3DXVec3Lerp(&vTransLerp, &_vec3(-10.f, 50.f, 0.f), &vPos, m_fPlayerFrame2);

			if (m_fPlayerFrame2 < 1.f)
			{
				if (!m_bLetterBox)
				{
					if (0.f < m_fPlayerFrame2 && m_fPlayerFrame2 < 0.3f)
					{
						m_pLetterBox = CLetterBox::Create(m_pGraphicDev, L"What? Cracken this time?", sizeof(L"What? Cracken this time?"), 0);
						m_bLetterBox = true;
					}
				}
				else if (0.3f < m_fPlayerFrame2 && m_fPlayerFrame2 < 0.6f)
				{
					m_pLetterBox->ChangeLetterContents(L"Okay, let's go again", sizeof(L"Okay, let's go again"));
				}
				else if (0.89f > m_fPlayerFrame2 && m_fPlayerFrame2 > 0.6f)
					m_pLetterBox->ChangeLetterContents(L"Let's do this!!!", sizeof(L"Let's do this!!!"));
				else if (m_fPlayerFrame2 > 0.9f)
				{
					m_pLetterBox->LetterDead();
				}

				m_fFov = D3DXToRadian(60.f);
				m_vEye += vTransLerp * m_fPlayerFrame2;
				m_vAt = vTransLerp + (-vUp * m_fPlayerFrame2 * 2.f);
			}
			else
			{
				m_fFov = D3DXToRadian(60.f);
				m_vEye += vPos;
				m_vAt = vPos;
			}
		}
	}

	if (dynamic_cast<CPlayerMapping*>(pBomb)->Get_WorldMap() == false)
		m_fBombFrame = 0.f;
	if (dynamic_cast<CFlight*>(pFlight)->Get_Control() == false)
		m_fFlightFrame = 0.f;
	if (dynamic_cast<CFlight*>(pShuttle)->Get_Shuttle() == false)
		m_fShuttleFrame = 0.f;
}

void CStaticCamera::Camera_Shaking(const _float& _fTimeDelta)
{
	if (m_bPlayerHit)
	{
		m_fFrame += 0.1f * _fTimeDelta;
		m_iReverse *= -1;
		m_vEye.y = m_vEye.y + (_float(m_iReverse) * 0.03f * _fTimeDelta);

		if (m_fFrame >= 0.025f)
		{
			m_fFrame = 0.f;
			m_bPlayerHit = false;
			_float fSound = 1.f;
			PlaySoundGun(L"Player_Hit.mp3", SOUND_MB, fSound);
			
		}
	}

	if (m_bEarthQuake)
	{
		m_fFrame += 0.1f * _fTimeDelta;
		m_iReverse *= -1;
		m_vEye.y = m_vEye.y + (_float(m_iReverse) * 0.1f * _fTimeDelta);

		if (m_fFrame >= 0.15f)
		{
			m_fFrame = 0.f;
			m_bEarthQuake = false;
		}
	}
}