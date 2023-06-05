#include "stdafx.h"
#include "..\Header\Inventory.h"
#include "Uzi.h"
#include "ShotGun.h"
#include "Sniper.h"
#include "CubePlayer.h"
#include "LetterBox.h"
CInventory::CInventory(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CInventory::~CInventory()
{
}

HRESULT CInventory::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_iItemCnt = 0;
	m_vecContents.resize(26);
	m_vecEquipments.resize(5);
	m_vecWeapon.resize(9);
	m_vecParts.resize(12);
	m_fInvPosX = 400.f;
	m_fInvPosY = 50.f;
	return S_OK;
}

_int CInventory::Update_Object(const _float & fTimeDelta)
{
	if (!m_pPlayer)
		m_pPlayer = Engine::Get_GameObject(STAGE_CHARACTER, L"PLAYER");

	CGameObject::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDER_UI, this);
	Sorting();
	Equipment_Sorting();
	Weapon_Sorting();
	Mouse();
	Key_Input();
	Enhancement();
	Syncronize();
	ItemTag();
	return 0;
}

void CInventory::LateUpdate_Object(void)
{
	if (!m_bInit)
	{
		m_bInit = true;
		CItemIcon* pGameObject = CItemIcon::Create(m_pGraphicDev, 0);

		m_pLetterBox1 = CLetterBox::Create(m_pGraphicDev, L"Upgrade : Can Use DefensiveMatrix", sizeof(L"Upgrade : Can Use DefensiveMatrix"), 2);
		m_pLetterBox2 = CLetterBox::Create(m_pGraphicDev, L"Weapon Item : Uzi", sizeof(L"Weapon Item : Uzi"), 2);
		m_pLetterBox3 = CLetterBox::Create(m_pGraphicDev, L"Weapon Item : Shotgun", sizeof(L"Weapon Item : Shotgun"), 2);
		m_pLetterBox4 = CLetterBox::Create(m_pGraphicDev, L"Weapon Item : Sniper", sizeof(L"Weapon Item : Sniper"), 2);
		m_pLetterBox5 = CLetterBox::Create(m_pGraphicDev, L"Equipment Item : Speed+", sizeof(L"Equipment Item : Speed+"), 2);
		m_pLetterBox6 = CLetterBox::Create(m_pGraphicDev, L"Equipment Item : Damage+", sizeof(L"Equipment Item : Damage+"), 2);
		m_pLetterBox7 = CLetterBox::Create(m_pGraphicDev, L"Equipment Item : WeaponSkill+", sizeof(L"Equipment Item : WeaponSkill+"), 2);
		m_pLetterBox8 = CLetterBox::Create(m_pGraphicDev, L"Upgrade : Can Use Static Field", sizeof(L"Upgrade : Can Use Static Field"), 2);

		dynamic_cast<CLetterBox*>(m_pLetterBox1)->Off_Switch();
		dynamic_cast<CLetterBox*>(m_pLetterBox2)->Off_Switch();
		dynamic_cast<CLetterBox*>(m_pLetterBox3)->Off_Switch();
		dynamic_cast<CLetterBox*>(m_pLetterBox4)->Off_Switch();
		dynamic_cast<CLetterBox*>(m_pLetterBox5)->Off_Switch();
		dynamic_cast<CLetterBox*>(m_pLetterBox6)->Off_Switch();
		dynamic_cast<CLetterBox*>(m_pLetterBox7)->Off_Switch();
		dynamic_cast<CLetterBox*>(m_pLetterBox8)->Off_Switch();
	}
}

void CInventory::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());

	if (m_bInvSwitch)
	{
		Begin_OrthoProj();
		m_pRcTexCom->Render_Buffer();
		End_OrthoProj();
	}
}

HRESULT CInventory::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pRcTexCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_RcTexCom"));
	NULL_CHECK_RETURN(m_pRcTexCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTexCom", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Clone_Proto(TRANSFORM_COMP));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ TRANSFORM_COMP, pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_InventoryTexture"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_InventoryTexture", pComponent });

	pComponent = m_pShaderCom = dynamic_cast<CShader*>(Clone_Proto(RCTEX_SHADER));
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ RCTEX_SHADER, pComponent });

	return S_OK;
}

void CInventory::Begin_OrthoProj()
{
	_matrix matWorld, matView, matOrtho;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);

	matView.m[0][0] = WINCX / 3.f; // 이미지 가로
	matView.m[1][1] = WINCY / 1.75f;   // 이미지 세로
	matView.m[2][2] = 1.f;
	matView.m[3][0] = m_fInvPosX; //* (WINCX / WINCY);
	matView.m[3][1] = m_fInvPosY; //* (WINCX / WINCY);
	matView.m[3][2] = 0.003f;
	D3DXMatrixOrthoLH(&matOrtho, WINCX, WINCY, 0.f, 1.f);

	D3DXMatrixTranspose(&matWorld, &matWorld);
	D3DXMatrixTranspose(&matView, &matView);
	D3DXMatrixTranspose(&matOrtho, &matOrtho);

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &matWorld, sizeof(_matrix))))
		return;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &matView, sizeof(_matrix))))
		return;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &matOrtho, sizeof(_matrix))))
		return;

	m_pTextureCom->Set_Texture(m_pShaderCom, "g_DefaultTexture", 0);
	m_pShaderCom->Begin_Shader(0);
}

void CInventory::End_OrthoProj()
{
	m_pShaderCom->End_Shader();
}


void CInventory::Enhancement()
{
	for (auto& iter : m_vecEquipments)
	{
		if (iter != nullptr)
		{
			if (dynamic_cast<CItemIcon*>(m_vecEquipments[0])->Get_iTemIdx() == 2) //우지일때
			{
				if (dynamic_cast<CItemIcon*>(iter)->Get_iTemIdx() == 5)
				{
					m_iWeaponSpeedCnt++;
				}
				if (dynamic_cast<CItemIcon*>(iter)->Get_iTemIdx() == 6)
				{
					m_iWeaponDmgCnt++;
				}
				if (dynamic_cast<CItemIcon*>(iter)->Get_iTemIdx() == 7)
				{
					m_iEnforceCheck = 1;
				}
				if (dynamic_cast<CItemIcon*>(iter)->Get_iTemIdx() == 8)
				{
					m_iEnforceCheck = 2;
				}
			}
			if (dynamic_cast<CItemIcon*>(m_vecEquipments[0])->Get_iTemIdx() == 3) //샷건일때
			{
				if (dynamic_cast<CItemIcon*>(iter)->Get_iTemIdx() == 5)
				{
					m_iWeaponSpeedCnt++;
				}
				if (dynamic_cast<CItemIcon*>(iter)->Get_iTemIdx() == 6)
				{
					m_iWeaponDmgCnt++;
				}
				if (dynamic_cast<CItemIcon*>(iter)->Get_iTemIdx() == 7)
				{
					m_iEnforceCheck = 3;
				}
				if (dynamic_cast<CItemIcon*>(iter)->Get_iTemIdx() == 8)
				{
					m_iEnforceCheck = 4;
				}
			}
			if (dynamic_cast<CItemIcon*>(m_vecEquipments[0])->Get_iTemIdx() == 4) //스나일때
			{
				if (dynamic_cast<CItemIcon*>(iter)->Get_iTemIdx() == 5)
				{
					m_iWeaponSpeedCnt++;
				}
				if (dynamic_cast<CItemIcon*>(iter)->Get_iTemIdx() == 6)
				{
					m_iWeaponDmgCnt++;
				}
				if (dynamic_cast<CItemIcon*>(iter)->Get_iTemIdx() == 7)
				{
					m_iEnforceCheck = 5;
				}
				if (dynamic_cast<CItemIcon*>(iter)->Get_iTemIdx() == 8)
				{
					m_iEnforceCheck = 6;
				}
			}
		}
	}

	m_iWeaponSpeed = m_iWeaponSpeedCnt;
	m_iWeaponDmg = m_iWeaponDmgCnt;
	m_iWeaponSpeedCnt = 0;
	m_iWeaponDmgCnt = 0;

	if (m_vecEquipments[1] == nullptr || dynamic_cast<CItemIcon*>(m_vecEquipments[1])->Get_iTemIdx() < 7)
	{
		if (m_vecEquipments[2] == nullptr || dynamic_cast<CItemIcon*>(m_vecEquipments[2])->Get_iTemIdx() < 7)
		{
			if (m_vecEquipments[3] == nullptr || dynamic_cast<CItemIcon*>(m_vecEquipments[3])->Get_iTemIdx() < 7)
			{
				if (m_vecEquipments[4] == nullptr || dynamic_cast<CItemIcon*>(m_vecEquipments[4])->Get_iTemIdx() < 7)
				{
					m_iEnforceCheck = 0;
				}
			}
		}
	}


}

void CInventory::Sorting()
{
	if (m_bItemCreate)
	{
		m_bItemCreate = false;
		if (m_iItemCnt < 27)
		{
			//생성
			m_bSorting = true;
			m_bNullSorting = true;
			m_pItemIcon = CItemIcon::Create(m_pGraphicDev, m_iItemIndex);
			m_vecContents.push_back(m_pItemIcon);
			m_iItemCnt++;
		}
	}

	_vec3   vPos;
	_int   iCnt = 0;
	CGameObject* pGameObject = nullptr;
	if (m_bSorting)
	{
		for (auto& iter : m_vecContents)
		{
			if (iter != nullptr) // 위치 지정
			{
				_float fDefaultX = 175.f;
				_float fDefaultY = 75.f;
				_float fIntervalX = (50.f) * (iCnt % 9);
				_float fIntervalY = (67.5f) * (iCnt / 9);
				dynamic_cast<CItemIcon*>(iter)->Set_block(fDefaultX + fIntervalX, fDefaultY - fIntervalY, 0.f);
			}

			if (m_bNullSorting && iter != m_vecContents.back() && iter == nullptr && m_vecContents.back() != nullptr) // 빈곳부터 채우기
			{
				iter = m_vecContents.back();
				m_bNull = true;
				m_bNullSorting = false;
				break;
			}
			iCnt++;
			iCnt %= 27;
		}
	}
	if (m_bNull)
	{
		m_vecContents.pop_back();
		m_bNull = false;
	}
}

void CInventory::Equipment_Sorting()
{
	if (m_vecEquipments[0] != nullptr)
	{
		dynamic_cast<CItemIcon*>(m_vecEquipments[0])->Set_block(435.f *WINCY / WINCX, 365.f * WINCY / WINCX, 0.1f);
	}
	if (m_vecEquipments[1] != nullptr)
	{
		dynamic_cast<CItemIcon*>(m_vecEquipments[1])->Set_block(535.f * WINCY / WINCX, 455.f * WINCY / WINCX, 0.1f);
	}
	if (m_vecEquipments[2] != nullptr)
	{
		dynamic_cast<CItemIcon*>(m_vecEquipments[2])->Set_block(535.f *WINCY / WINCX, 275.f*WINCY / WINCX, 0.1f);
	}
	if (m_vecEquipments[3] != nullptr)
	{
		dynamic_cast<CItemIcon*>(m_vecEquipments[3])->Set_block(800.f *WINCY / WINCX, 455.f * WINCY / WINCX, 0.1f);
	}
	if (m_vecEquipments[4] != nullptr)
	{
		dynamic_cast<CItemIcon*>(m_vecEquipments[4])->Set_block(800.f *WINCY / WINCX, 275.f * WINCY / WINCX, 0.1f);
	}


}

void CInventory::Weapon_Sorting()
{
	_int   iCnt = 0;

	_float fDefaultX = 175;//175.f;
	_float fDefaultY = -145.5f;//-125.5f;
	_float fIntervalX = 50.f;


	//생성(1.우지 2.샷건 3. 스나)
	if (m_bWeaponCreate)
	{
		m_bWeaponCreate = false;
		if (m_iWeaponCnt < 9)
		{
			m_pItemIcon = CItemIcon::Create(m_pGraphicDev, m_iItemIndex);
			m_vecWeapon.push_back(m_pItemIcon);
			m_vecWeapon[dynamic_cast<CItemIcon*>(m_pItemIcon)->Get_iTemIdx() - 2] = m_pItemIcon;
			m_pItemIcon = nullptr;

			m_iWeaponCnt++;
		}
	}
	//정렬   
	for (_int i = 0; i < 9; ++i)
	{
		if (m_vecWeapon[i] != nullptr)
			dynamic_cast<CItemIcon*>(m_vecWeapon[i])->Set_block(fDefaultX + (fIntervalX *i), fDefaultY, 0.f);
	}

	for (auto& iter : m_vecParts)
	{
		if (iter != nullptr)
			dynamic_cast<CItemIcon*>(iter)->On_WeaponPart();
	}
}

void CInventory::Syncronize()
{
	_int iBlock = dynamic_cast<CCubePlayer*>(m_pPlayer)->GetWeaponState() - 2;

	if (m_vecEquipments[0] == nullptr)
	{
		m_iWeaponState = 0;
	}

	if (m_vecEquipments[0] != nullptr && m_bInvSwitch)
	{
		dynamic_cast<CCubePlayer*>(m_pPlayer)->SetWeaponState(dynamic_cast<CItemIcon*>(m_vecEquipments[0])->Get_iTemIdx());
	}

	if (dynamic_cast<CCubePlayer*>(m_pPlayer)->GetWeaponState() != m_iWeaponState)
	{

		if (m_vecEquipments[0] != nullptr)
		{
			for (_int i = 1; i < 5; i++)
			{
				if (m_vecEquipments[i] != nullptr)
				{
					m_vecParts[(((dynamic_cast<CItemIcon*>(m_vecEquipments[0])->Get_iTemIdx()) - 2) * 4) + (i - 1)] = m_vecEquipments[i];
					m_vecEquipments[i] = nullptr;
				}
			}
			m_vecWeapon[dynamic_cast<CItemIcon*>(m_vecEquipments[0])->Get_iTemIdx() - 2] = m_vecEquipments[0];
			m_vecEquipments[0] = nullptr;

		}
		if (m_vecEquipments[0] == nullptr)
		{
			m_vecEquipments[0] = m_vecWeapon[dynamic_cast<CCubePlayer*>(m_pPlayer)->GetWeaponState() - 2]; // m_vecWeapon 벡터위치 수정
			m_vecWeapon[dynamic_cast<CCubePlayer*>(m_pPlayer)->GetWeaponState() - 2] = nullptr; // 벡터위치 수정

			for (_int i = 0; i < 4; ++i)
			{
				if (m_vecParts[4 * iBlock + i] != nullptr)
				{
					m_vecEquipments[i + 1] = m_vecParts[4 * iBlock + i];
					m_vecParts[4 * iBlock + i] = nullptr;
					dynamic_cast<CItemIcon*>(m_vecEquipments[i + 1])->Off_WeaponPart();
				}
			}
			m_iWeaponState = dynamic_cast<CCubePlayer*>(m_pPlayer)->GetWeaponState();
		}
		m_iWeaponNumb = m_iWeaponState;
	}
}

void CInventory::ItemTag()
{
	if (m_pIconGrab)
	{
		dynamic_cast<CLetterBox*>(m_pLetterBox1)->Off_Switch();
		dynamic_cast<CLetterBox*>(m_pLetterBox2)->Off_Switch();
		dynamic_cast<CLetterBox*>(m_pLetterBox3)->Off_Switch();
		dynamic_cast<CLetterBox*>(m_pLetterBox4)->Off_Switch();
		dynamic_cast<CLetterBox*>(m_pLetterBox5)->Off_Switch();
		dynamic_cast<CLetterBox*>(m_pLetterBox6)->Off_Switch();
		dynamic_cast<CLetterBox*>(m_pLetterBox7)->Off_Switch();
		dynamic_cast<CLetterBox*>(m_pLetterBox8)->Off_Switch();
	}
	if (m_bTagSwitch && !m_pIconGrab)
	{
		dynamic_cast<CLetterBox*>(m_pLetterBox1)->Off_Switch();
		dynamic_cast<CLetterBox*>(m_pLetterBox2)->Off_Switch();
		dynamic_cast<CLetterBox*>(m_pLetterBox3)->Off_Switch();
		dynamic_cast<CLetterBox*>(m_pLetterBox4)->Off_Switch();
		dynamic_cast<CLetterBox*>(m_pLetterBox5)->Off_Switch();
		dynamic_cast<CLetterBox*>(m_pLetterBox6)->Off_Switch();
		dynamic_cast<CLetterBox*>(m_pLetterBox7)->Off_Switch();
		dynamic_cast<CLetterBox*>(m_pLetterBox8)->Off_Switch();

		if (m_iTagIndex == 1)
		{
			dynamic_cast<CLetterBox*>(m_pLetterBox1)->On_Switch();
		}
		if (m_iTagIndex == 2)
		{
			dynamic_cast<CLetterBox*>(m_pLetterBox2)->On_Switch();
		}
		if (m_iTagIndex == 3)
		{
			dynamic_cast<CLetterBox*>(m_pLetterBox3)->On_Switch();
		}
		if (m_iTagIndex == 4)
		{
			dynamic_cast<CLetterBox*>(m_pLetterBox4)->On_Switch();
		}
		if (m_iTagIndex == 5)
		{
			dynamic_cast<CLetterBox*>(m_pLetterBox5)->On_Switch();
		}
		if (m_iTagIndex == 6)
		{
			dynamic_cast<CLetterBox*>(m_pLetterBox6)->On_Switch();
		}
		if (m_iTagIndex == 7)
		{
			dynamic_cast<CLetterBox*>(m_pLetterBox7)->On_Switch();
		}
		if (m_iTagIndex == 8)
		{
			dynamic_cast<CLetterBox*>(m_pLetterBox8)->On_Switch();
		}
		m_iTagIndex = 0;
	}
}

void CInventory::Key_Input()
{
	if (Key_Down(DIK_I))
	{
		m_bInvSwitch = !m_bInvSwitch;
	}
}

void CInventory::Mouse()
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	_float DefaultX = 955.f;
	_float DefaultY = 345.f;
	_float SizeX = 45.f;
	_float SizeY = 60.f;
	_float IntervalX = 50.f;
	_float IntervalY = 67.5f;
	//1405,565   ~   1450 630 << 나가기버튼
	if (1405 < pt.x && pt.x < 1450 && 565 < pt.y && pt.y < 630 && m_bInvSwitch && Mouse_Down(DIM_LB))
	{
		m_bInvSwitch = false;
	}
	if (!m_bInvSwitch)
	{
		if (m_pIconGrab != nullptr)
		{
			m_pIconGrab->Cursor_free();
			m_pIconGrab = nullptr;
		}
	}

	//인벤토리 기능
	for (size_t i = 0; i < 9; ++i)
	{
		for (size_t j = 0; j < 9; ++j)
		{
			for (auto& iter : m_vecContents)
			{
				if (DefaultX + IntervalX * j < pt.x && pt.x < DefaultX + SizeX + IntervalX * j)
				{
					if (DefaultY + IntervalY * i < pt.y && pt.y < DefaultY + SizeY + IntervalY * i)
					{
						if (m_vecContents.size() > 9 * i + j && iter == m_vecContents[9 * i + j] && iter != nullptr)
						{
							if (m_pIconGrab == nullptr && Mouse_Down(DIM_LB))
							{
								m_iVectorNumb = 9 * i + j;

								m_pIconGrab = dynamic_cast<CItemIcon*>(iter);
								m_pIconGrab->Cursor_fix();
								_float fSound = 1.f;
								Engine::PlaySoundGun(L"Button.wav", SOUND_EFFECT, fSound);
							}
							if (Mouse_Down(DIM_RB) && m_pIconGrab == nullptr)
							{
								m_iItemCnt--;
								iter->Kill_Obj();
								iter = nullptr;
							}
						}
					}
				}
			}
		}
	}
	//무기창 기능
	for (_int i = 0; i < 9; ++i)
	{
		for (auto& iter : m_vecWeapon)
		{
			if (DefaultX + IntervalX * i < pt.x && pt.x < DefaultX + SizeX + IntervalX * i)
			{
				if (DefaultY + 220.5f < pt.y && pt.y < DefaultY + 220.5f + SizeY)
				{
					if (iter == m_vecWeapon[i] && iter != nullptr)
					{
						if (m_pIconGrab == nullptr && Mouse_Down(DIM_LB))
						{
							m_iWeaponNumb = i;

							m_pIconGrab = dynamic_cast<CItemIcon*>(iter);
							m_pIconGrab->Cursor_fix();
						}
						if (Mouse_Down(DIM_RB) && m_pIconGrab == nullptr)
						{

						}
					}
				}
			}
		}
	}
	//좌클릭 기능 
	//좌클릭 장비 장착

	for (auto& iter : m_vecWeapon)
	{
		if (iter != nullptr&& iter == m_vecWeapon[m_iWeaponNumb] && m_pIconGrab != nullptr && 2 <= m_pIconGrab->Get_iTemIdx() && m_pIconGrab->Get_iTemIdx() <= 4)
		{
			if (Mouse_Down(DIM_LB))
			{
				m_pIconGrab->Cursor_free();
				if (1025 < pt.x && pt.x < 1075 && 215 < pt.y && pt.y < 280) // 맨왼쪽인벤토리
				{
					_float fSound = 1.f;
					Engine::PlaySoundGun(L"Equipped.wav", SOUND_EFFECT, fSound);
					if (m_vecEquipments[0] != nullptr)
					{
						for (_int i = 1; i < 5; i++)
						{
							if (m_vecEquipments[i] != nullptr)
							{
								m_vecParts[(((dynamic_cast<CItemIcon*>(m_vecEquipments[0])->Get_iTemIdx()) - 2) * 4) + (i - 1)] = m_vecEquipments[i];
								m_vecEquipments[i] = nullptr;

							}
						}
						m_vecWeapon[dynamic_cast<CItemIcon*>(m_vecEquipments[0])->Get_iTemIdx() - 2] = m_vecEquipments[0];
						m_vecEquipments[0] = nullptr;
					}
					if (m_vecEquipments[0] == nullptr)
					{
						if (2 <= m_pIconGrab->Get_iTemIdx() && m_pIconGrab->Get_iTemIdx() <= 4)
						{
							m_pIconGrab->Set_block(435.f *WINCY / WINCX, 365.f * WINCY / WINCX, 0.1f);
							m_vecEquipments[0] = m_pIconGrab;
							for (_int i = 0; i < 4; ++i)
							{
								if (m_vecParts[(4 * m_iWeaponNumb) + i] != nullptr)
								{
									m_vecEquipments[i + 1] = m_vecParts[(4 * m_iWeaponNumb) + i];
									m_vecParts[(4 * m_iWeaponNumb) + i] = nullptr;
									dynamic_cast<CItemIcon*>(m_vecEquipments[i + 1])->Off_WeaponPart();
								}
							}
							iter = nullptr;
							m_pIconGrab = nullptr;
							//break;
						}
					}
				}
				m_pIconGrab = nullptr;
			}
		}
	}
	for (auto& iter : m_vecContents)
	{
		if (iter != nullptr&& iter == m_vecContents[m_iVectorNumb] && m_vecEquipments[0] != nullptr&& m_pIconGrab != nullptr && Mouse_Down(DIM_LB))
		{
			m_pIconGrab->Cursor_free();

			if (!(2 <= m_pIconGrab->Get_iTemIdx() && m_pIconGrab->Get_iTemIdx() <= 4))
			{
				if (1080 < pt.x && pt.x < 1125 && 160 < pt.y && pt.y < 225) // 0,0
				{
					if (m_vecEquipments[1] == nullptr)
					{
						m_pIconGrab->Set_block(535.f * WINCY / WINCX, 455.f * WINCY / WINCX, 0.1f);
						m_vecEquipments[1] = m_pIconGrab;
						iter = nullptr;
						m_pIconGrab = nullptr;
						m_iItemCnt--;
						m_bSorting = true;
					}
				}
				if (1080 < pt.x && pt.x < 1125 && 265 < pt.y && pt.y < 330) // 0,1
				{
					if (m_vecEquipments[2] == nullptr)
					{
						m_pIconGrab->Set_block(535.f *WINCY / WINCX, 275.f*WINCY / WINCX, 0.1f);
						m_vecEquipments[2] = m_pIconGrab;
						iter = nullptr;
						m_pIconGrab = nullptr;
						m_iItemCnt--;
						m_bSorting = true;
					}
				}
				if (1230 < pt.x && pt.x < 1275 && 160 < pt.y && pt.y < 225) // 1,0
				{
					if (m_vecEquipments[3] == nullptr)
					{
						m_pIconGrab->Set_block(800.f *WINCY / WINCX, 455.f * WINCY / WINCX, 0.1f);
						m_vecEquipments[3] = m_pIconGrab;
						iter = nullptr;
						m_pIconGrab = nullptr;
						m_iItemCnt--;

					}
				}
				if (1230 < pt.x && pt.x < 1275 && 265 < pt.y && pt.y < 330) // 1,1
				{
					if (m_vecEquipments[4] == nullptr)
					{
						m_pIconGrab->Set_block(800.f *WINCY / WINCX, 275.f * WINCY / WINCX, 0.1f);
						m_vecEquipments[4] = m_pIconGrab;
						iter = nullptr;
						m_pIconGrab = nullptr;
						m_iItemCnt--;
					}
				}
			}
			m_pIconGrab = nullptr;
			m_bNullSorting = false;
			m_bSorting = true;

		}
	}

	//파츠 장착해제 관련
	if (1025 < pt.x && pt.x < 1075 && 215 < pt.y && pt.y < 280) // 맨왼쪽인벤토리
	{
		if (Mouse_Down(DIM_RB))
		{
			for (int i = 1; i < 5; i++)
			{
				if (m_vecEquipments[i] != nullptr)
				{
					if (m_iItemCnt < 27)
					{
						m_iItemCnt++;
						m_vecContents.push_back(m_vecEquipments[i]);
						m_bNullSorting = true;
						Sorting();
						m_vecEquipments[i] = nullptr;
						//m_bSorting = true;         
					}
				}
			}
		}
		if (m_vecEquipments[0] != nullptr && Mouse_Down(DIM_LB))
		{

			for (_int i = 1; i < 5; i++)
			{
				if (m_vecEquipments[i] != nullptr)
				{
					m_vecParts[(((dynamic_cast<CItemIcon*>(m_vecEquipments[0])->Get_iTemIdx()) - 2) * 4) + (i - 1)] = m_vecEquipments[i];
					m_vecEquipments[i] = nullptr;
				}
			}
			m_vecWeapon[dynamic_cast<CItemIcon*>(m_vecEquipments[0])->Get_iTemIdx() - 2] = m_vecEquipments[0];
			m_vecEquipments[0] = nullptr;
		}

	}
	if (m_iItemCnt < 27)
	{
		if (1080 < pt.x && pt.x < 1125 && 160 < pt.y && pt.y < 225) // 0,0
		{
			if (m_vecEquipments[1] != nullptr && Mouse_Down(DIM_LB))
			{
				m_iItemCnt++;
				m_vecContents.push_back(m_vecEquipments[1]);
				m_vecEquipments[1] = nullptr;
				m_bNullSorting = true;
				m_bSorting = true;
			}
		}
		if (1080 < pt.x && pt.x < 1125 && 265 < pt.y && pt.y < 330) // 1,0
		{
			if (m_vecEquipments[2] != nullptr && Mouse_Down(DIM_LB))
			{
				m_iItemCnt++;
				m_vecContents.push_back(m_vecEquipments[2]);
				m_vecEquipments[2] = nullptr;
				m_bNullSorting = true;
				m_bSorting = true;
			}
		}
		if (1230 < pt.x && pt.x < 1275 && 160 < pt.y && pt.y < 225) // 0,1
		{
			if (m_vecEquipments[3] != nullptr && Mouse_Down(DIM_LB))
			{
				m_iItemCnt++;
				m_vecContents.push_back(m_vecEquipments[3]);
				m_vecEquipments[3] = nullptr;
				m_bNullSorting = true;
				m_bSorting = true;
			}
		}
		if (1230 < pt.x && pt.x < 1275 && 265 < pt.y && pt.y < 330) // 1,1
		{
			if (m_vecEquipments[4] != nullptr && Mouse_Down(DIM_LB))
			{
				m_iItemCnt++;
				m_vecContents.push_back(m_vecEquipments[4]);
				m_vecEquipments[4] = nullptr;
				m_bNullSorting = true;
				m_bSorting = true;
			}
		}
	}
}

CInventory * CInventory::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CInventory*   pInstance = new CInventory(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CInventory::Free(void)
{
	CGameObject::Free();
}