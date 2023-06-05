#include "stdafx.h"
#include "..\Header\Loading.h"

#include "Export_Function.h"
#include "..\..\Engine\Header\Engine_Macro.h"

CLoading::CLoading(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
	, m_bFinish(false)
	, m_eID(LOADING_END)
{
	m_pGraphicDev->AddRef();
}

CLoading::~CLoading()
{
}

HRESULT CLoading::Ready_Loading(LOADINGID eID)
{
	m_iMax = 381;
	InitializeCriticalSection(&m_Crt);
	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, Thread_Main, this, 0, nullptr);
	m_eID = eID;
	return S_OK;
}

_uint CLoading::Loading_ForStage(void)
{
	// 버퍼 컴포넌트들
	{
		FAILED_CHECK_RETURN(Engine::Ready_Proto(RCTEX_HP_COMP, CRcTex::Create(m_pGraphicDev)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(RCTEX_DEFENCE_COMP, CRcTex::Create(m_pGraphicDev)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(RCTEX_BULLET_COMP, CRcTex::Create(m_pGraphicDev)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(RCTEX_MONTER_HP_COMP, CRcTex::Create(m_pGraphicDev)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(RCTEX_GENERAL_MONSTER_HP_COMP, CRcTex::Create(m_pGraphicDev)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(RCTEX_GENERAL_MONSTER_HUD_COMP, CRcTex::Create(m_pGraphicDev)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(RCTEX_COMBO_COMP, CRcTex::Create(m_pGraphicDev)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(RCTEX_COMBOBAR_HUD_COMP, CRcTex::Create(m_pGraphicDev)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(FLEXIBLEAXISCUBE_COMP, CFlexibleAxisCube::Create(m_pGraphicDev)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(CUBECOL_COMP, CCubeCol::Create(m_pGraphicDev)), E_FAIL);
	}
	// 파티클
	{
		FAILED_CHECK_RETURN(Engine::Ready_Proto(SHOTPARTICLE_TEX, CTexture::Create(m_pGraphicDev, SHOTPARTICLE_TEX_PATH, TEX_NORMAL)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(BULLETPARTICLE_TEX, CTexture::Create(m_pGraphicDev, BULLETPARTICLE_TEX_PATH, TEX_NORMAL)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(PROJECTILEPARTICLE_TEX, CTexture::Create(m_pGraphicDev, PROJECTILEPARTICLE_TEX_PATH, TEX_NORMAL)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(STATICPARTICLE_TEX, CTexture::Create(m_pGraphicDev, STATICPARTICLE_TEX_PATH, TEX_NORMAL,9)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MonsterParticle", CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Particle/blood.png", TEX_NORMAL)), E_FAIL);
	}
	//이펙트
	{
		FAILED_CHECK_RETURN(Engine::Ready_Proto(BLUEBOMB_TEX, CTexture::Create(m_pGraphicDev, BLUEBOMB_TEX_PATH, TEX_NORMAL,3)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(REDEFFECT_TEX, CTexture::Create(m_pGraphicDev, REDEFFECT_TEX_PATH, TEX_NORMAL, 10)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(BLUEEFFECT_TEX, CTexture::Create(m_pGraphicDev, BLUEEFFECT_TEX_PATH, TEX_NORMAL, 10)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(CASTINGEFFECT_TEX, CTexture::Create(m_pGraphicDev, CASTINGEFFECT_TEX_PATH, TEX_NORMAL, 10)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(STAREFFECT_TEX, CTexture::Create(m_pGraphicDev, STAREFFECT_TEX_PATH, TEX_NORMAL, 9)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(COIN_TEX, CTexture::Create(m_pGraphicDev, COIN_TEX_PATH, TEX_NORMAL, 8)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(CREATEEFFECT_TEX, CTexture::Create(m_pGraphicDev, CREATEEFFECT_TEX_PATH, TEX_NORMAL, 11)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(EXPLOSION_TEX, CTexture::Create(m_pGraphicDev, EXPLOSION_TEX_PATH, TEX_NORMAL, 12)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(LIGHTNING_TEX, CTexture::Create(m_pGraphicDev, LIGHTNING_TEX_PATH, TEX_NORMAL, 4)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(REDBOMB_TEX, CTexture::Create(m_pGraphicDev, REDBOMB_TEX_PATH, TEX_NORMAL, 8)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(STUN_TEX, CTexture::Create(m_pGraphicDev, STUN_TEX_PATH, TEX_NORMAL, 4)), E_FAIL);
	}

	{
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CubePlayerTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/CubeTile/CubeTile_%d.dds", TEX_CUBE, 121)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CubeMonsterTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Texture.dds", TEX_CUBE, 4)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"DOOMGUY", CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Doomguy/Doom_%d.dds", TEX_CUBE, 10)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"BLUEMARINE", CTexture::Create(m_pGraphicDev, L"../Bin/Resources/BlueMarine/BlueMarine_%d.dds", TEX_CUBE, 10)), E_FAIL);
	}

	{
		FAILED_CHECK_RETURN(Engine::Ready_Proto(COLLISION_COMP, CCollision::Create(m_pGraphicDev)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(CALCULATOR_COMP, CCalculator::Create(m_pGraphicDev)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(HITBOX_COMP, CHitBox::Create(m_pGraphicDev)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(QUATERNION_COMP, CQuarternion::Create(m_pGraphicDev)), E_FAIL);
	}

	// PLAYER UI
	{
		// HUD
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"theHUDui_7", CTexture::Create(m_pGraphicDev, L"../Bin/Resources/UI/Player/theHUDui_7.png", TEX_NORMAL)), E_FAIL);
		// 얼굴
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"HP_100_Face", CTexture::Create(m_pGraphicDev, HP_100_FACE_PATH, TEX_NORMAL, 5)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"HP_75_Face", CTexture::Create(m_pGraphicDev, HP_75_FACE_PATH, TEX_NORMAL, 5)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"HP_50_Face", CTexture::Create(m_pGraphicDev, HP_50_FACE_PATH, TEX_NORMAL, 5)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"HP_25_Face", CTexture::Create(m_pGraphicDev, HP_25_FACE_PATH, TEX_NORMAL, 5)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"HP_0_Face", CTexture::Create(m_pGraphicDev, HP_0_FACE_PATH, TEX_NORMAL)), E_FAIL);
		// HITBAR
		FAILED_CHECK_RETURN(Engine::Ready_Proto(HITBAR_TEX, CTexture::Create(m_pGraphicDev, HITBAR_TEX_PATH, TEX_NORMAL)), E_FAIL);
	}

	{
		// HP
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"HP_Gage", CTexture::Create(m_pGraphicDev, L"../Bin/Resources/UI/Player/Gage/theHUDui_10.png", TEX_NORMAL)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Monster_General_HP", CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Texture2D/Palette Texture.png", TEX_NORMAL)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(MONSTER_HP_TEX, CTexture::Create(m_pGraphicDev, MONSTER_HP_TEX_PATH, TEX_NORMAL)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(MONSTER_HP_HUD_TEX, CTexture::Create(m_pGraphicDev, MONSTER_HP_HUD_TEX_PATH, TEX_NORMAL)), E_FAIL);
		// COMBO
		FAILED_CHECK_RETURN(Engine::Ready_Proto(COMBOBAR_TEX, CTexture::Create(m_pGraphicDev, COMBOBAR_TEX_PATH, TEX_NORMAL)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(COMBOBAR_HUD_TEX, CTexture::Create(m_pGraphicDev, COMBOBAR_HUD_TEX_PATH, TEX_NORMAL)), E_FAIL);
		// DEFENCE
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"DEFENSE_Gage", CTexture::Create(m_pGraphicDev, L"../Bin/Resources/UI/Player/Gage/theHUDui_9.png", TEX_NORMAL)), E_FAIL);
	}

	// WEAPON UI
	{
		// HUD
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"theHUDui_11", CTexture::Create(m_pGraphicDev, L"../Bin/Resources/UI/Weapon/theHUDui_11.png", TEX_NORMAL)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"BULLET_Gage", CTexture::Create(m_pGraphicDev, L"../Bin/Resources/UI/Weapon/theHUDui_12.png", TEX_NORMAL)), E_FAIL);
		// GUN
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Gun_UI", CTexture::Create(m_pGraphicDev, L"../Bin/Resources/UI/Gun/Gun_%d.png", TEX_NORMAL, 6)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"BULLET_TEX", CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Texture2D/Bullet.png", TEX_NORMAL)), E_FAIL);
	}

	//인벤토리
	{
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_InventoryTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resources/UI/Inventory/QuickSlot9.png", TEX_NORMAL)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ItemIconTexture", CTexture::Create(m_pGraphicDev, L"../Bin/Resources/UI/Inventory/Icon%d.png", TEX_NORMAL, 9)), E_FAIL);
	}

	//ITEM
	{
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Hp", CTexture::Create(m_pGraphicDev, L"../Bin/Resources/UI/Sprite/Hp2.jpg", TEX_NORMAL)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MaxHp", CTexture::Create(m_pGraphicDev, L"../Bin/Resources/UI/Sprite/Hp.png", TEX_NORMAL)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Defense", CTexture::Create(m_pGraphicDev, L"../Bin/Resources/UI/Sprite/DSC04709.jpg", TEX_NORMAL)), E_FAIL);
	}

	// Map
	{
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Minimap", CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Texture2D/hud_minimap_bg.png", TEX_NORMAL)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Bomb_TEX", CTexture::Create(m_pGraphicDev, L"../Bin/Resources/TargetAim.png", TEX_NORMAL)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Missile_TEX", CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Blue_Aim.png", TEX_NORMAL)), E_FAIL);
	}

	// PNGtoCube
	{
		FAILED_CHECK_RETURN(Engine::Ready_Proto(RIFLE_ITEM_BUFFER, CDynamicBuffer::Create(m_pGraphicDev, RIFLE_ITEM_TEX_PATH, 0.01f, 1.f, 1.f)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(RIFLE_ITEM_TEX, CTexture::Create(m_pGraphicDev, RIFLE_ITEM_TEX_PATH, TEX_NORMAL)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(SHOTGUN_ITEM_BUFFER, CDynamicBuffer::Create(m_pGraphicDev, SHOTGUN_ITEM_TEX_PATH, 0.01f, 1.f, 1.f)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(SHOTGUN_ITEM_TEX, CTexture::Create(m_pGraphicDev, SHOTGUN_ITEM_TEX_PATH, TEX_NORMAL)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(SNIPER_ITEM_BUFFER, CDynamicBuffer::Create(m_pGraphicDev, SNIPER_ITEM_TEX_PATH, 0.01f, 1.f, 1.f)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(SNIPER_ITEM_TEX, CTexture::Create(m_pGraphicDev, SNIPER_ITEM_TEX_PATH, TEX_NORMAL)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(ADDHP_ITEM_BUFFER, CDynamicBuffer::Create(m_pGraphicDev, ADDHP_ITEM_TEX_PATH, 0.01f, 1.f, 1.f)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(ADDHP_ITEM_TEX, CTexture::Create(m_pGraphicDev, ADDHP_ITEM_TEX_PATH, TEX_NORMAL)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(ADDDEFENCE_ITEM_BUFFER, CDynamicBuffer::Create(m_pGraphicDev, ADDDEFENCE_ITEM_TEX_PATH, 0.01f, 1.f, 1.f)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(ADDDEFENCE_ITEM_TEX, CTexture::Create(m_pGraphicDev, ADDDEFENCE_ITEM_TEX_PATH, TEX_NORMAL)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(ADDMAXHP_ITEM_BUFFER, CDynamicBuffer::Create(m_pGraphicDev, ADDMAXHP_ITEM_TEX_PATH, 0.01f, 1.f, 1.f)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(ADDMAXHP_ITEM_TEX, CTexture::Create(m_pGraphicDev, ADDMAXHP_ITEM_TEX_PATH, TEX_NORMAL)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(BULLET_ITEM_BUFFER, CDynamicBuffer::Create(m_pGraphicDev, BULLET_ITEM_TEX_PATH, 0.01f, 1.f, 1.f)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(BULLET_ITEM_TEX, CTexture::Create(m_pGraphicDev, BULLET_ITEM_TEX_PATH, TEX_NORMAL)), E_FAIL);
		// 키 3개
		FAILED_CHECK_RETURN(Engine::Ready_Proto(BLUEKEY_ITEM_BUFFER, CDynamicBuffer::Create(m_pGraphicDev, BLUEKEY_ITEM_TEX_PATH, 0.01f, 1.f, 1.f)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(BLUEKEY_ITEM_TEX, CTexture::Create(m_pGraphicDev, BLUEKEY_ITEM_TEX_PATH, TEX_NORMAL)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(REDKEY_ITEM_BUFFER, CDynamicBuffer::Create(m_pGraphicDev, REDKEY_ITEM_TEX_PATH, 0.01f, 1.f, 1.f)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(REDKEY_ITEM_TEX, CTexture::Create(m_pGraphicDev, REDKEY_ITEM_TEX_PATH, TEX_NORMAL)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(YELLOWKEY_ITEM_BUFFER, CDynamicBuffer::Create(m_pGraphicDev, YELLOWKEY_ITEM_TEX_PATH, 0.01f, 1.f, 1.f)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(YELLOWKEY_ITEM_TEX, CTexture::Create(m_pGraphicDev, YELLOWKEY_ITEM_TEX_PATH, TEX_NORMAL)), E_FAIL);
	}

	// ITEM
	{
		FAILED_CHECK_RETURN(Engine::Ready_Proto(ITEM_TRANSFORM_COMP, CTransform::Create(m_pGraphicDev)), E_FAIL);
	}
		FAILED_CHECK_RETURN(Engine::Ready_Proto(CROSSHAIR_RIFLE_TEX, CTexture::Create(m_pGraphicDev, CROSSHAIR_RIFLE_TEX_PATH, TEX_NORMAL)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(CROSSHAIR_SHOTGUN_TEX, CTexture::Create(m_pGraphicDev, CROSSHAIR_SHOTGUN_PATH, TEX_NORMAL)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(CROSSHAIR_SNIPER_TEX, CTexture::Create(m_pGraphicDev, CROSSHAIR_SNIPER_TEX_PATH, TEX_NORMAL)), E_FAIL);
	{
		//Shop
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ShopTextureCom", CTexture::Create(m_pGraphicDev, L"../Bin/Resources/UI/Sprite/UI upgrade system - sheet_2.png", TEX_NORMAL)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UziTextureCom", CTexture::Create(m_pGraphicDev, L"../Bin/Resources/UI/Gun/Gun_0.png", TEX_NORMAL)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ShotGunTextureCom", CTexture::Create(m_pGraphicDev, L"../Bin/Resources/UI/Gun/Gun_2.png", TEX_NORMAL)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_SniperTextureCom", CTexture::Create(m_pGraphicDev, L"../Bin/Resources/UI/Gun/Gun_4.png", TEX_NORMAL)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ButtonOneformCom", CTexture::Create(m_pGraphicDev, L"../Bin/Resources/UI/ShopUI/UI%d.png", TEX_NORMAL, 2)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ButtonTwoformCom", CTexture::Create(m_pGraphicDev, L"../Bin/Resources/UI/ShopUI/UI%d.png", TEX_NORMAL, 2)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ButtonThreeformCom", CTexture::Create(m_pGraphicDev, L"../Bin/Resources/UI/ShopUI/UI%d.png", TEX_NORMAL, 2)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_LevelLaserCom", CTexture::Create(m_pGraphicDev, L"../Bin/Resources/UI/ShopUI/Level%d.png", TEX_NORMAL, 2)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_HeavyCom", CTexture::Create(m_pGraphicDev, L"../Bin/Resources/UI/ShopUI/Level%d.png", TEX_NORMAL, 2)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_RailGunCom", CTexture::Create(m_pGraphicDev, L"../Bin/Resources/UI/ShopUI/Level%d.png", TEX_NORMAL, 2)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BurstModeCom", CTexture::Create(m_pGraphicDev, L"../Bin/Resources/UI/ShopUI/Level%d.png", TEX_NORMAL, 2)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UpgradeStringCom", CTexture::Create(m_pGraphicDev, L"../Bin/Resources/UI/ShopUI/UIupgrade.png", TEX_NORMAL)), E_FAIL);
	}

	{
		// Skiil
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Lava_Tex", CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Skill/Lava.png", TEX_NORMAL)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Ping_Tex", CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Skill/locationPin.png", TEX_NORMAL)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Shield_Tex", CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Particle/StaticParticle/Shield.png", TEX_NORMAL)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Shield2_Tex", CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Particle/StaticParticle/Shield2.png", TEX_NORMAL)), E_FAIL);
	
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"LetterBox_Tex", CTexture::Create(m_pGraphicDev, L"../Bin/Resources/UI/Quest/Quest_01_%d.png", TEX_NORMAL, 53)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"LetterBox_Tex_2", CTexture::Create(m_pGraphicDev, L"../Bin/Resources/UI/Quest2/Quest_02_%d.png", TEX_NORMAL, 61)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"LetterBox_Tex_3", CTexture::Create(m_pGraphicDev, L"../Bin/Resources/UI/Quest3/Quest_03_%d.png", TEX_NORMAL, 36)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"LetterBox_Tex_4", CTexture::Create(m_pGraphicDev, L"../Bin/Resources/UI/Quest4/Quest_04_%d.png", TEX_NORMAL, 62)), E_FAIL);
		
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Black_Tex", CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Particle/StaticParticle/Black.png", TEX_NORMAL)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Brown_Tex", CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Particle/StaticParticle/Brown.png", TEX_NORMAL)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"LightGreen_Tex", CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Particle/StaticParticle/LightGreen.png", TEX_NORMAL)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"White_Tex", CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Particle/StaticParticle/White.png", TEX_NORMAL)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Red_Tex", CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Particle/StaticParticle/Red.png", TEX_NORMAL)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Orange_Tex", CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Particle/StaticParticle/Orange.png", TEX_NORMAL)), E_FAIL);

		// 장판
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Veneer_Tex", CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Blue_Aim.png", TEX_NORMAL)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Black_Kraken", CTexture::Create(m_pGraphicDev, L"../Bin/Resources/UI/Sprite/Black.jpg", TEX_NORMAL)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Quest_Tex", CTexture::Create(m_pGraphicDev, L"../Bin/Resources/UI/Sprite/character_select_UI_2.png", TEX_NORMAL)), E_FAIL);
	
		FAILED_CHECK_RETURN(Engine::Ready_Proto(WARNING_TEX, CTexture::Create(m_pGraphicDev, WARNING_TEX_PATH, TEX_NORMAL, 2)), E_FAIL);
	}

	m_bFinish = true;
	return 0;
}

_uint CLoading::Loading_ForBoss(void)
{
	return _uint();
}



unsigned int CLoading::Thread_Main(void * pArg)
{
	CLoading*		pLoading = (CLoading*)pArg;

	_uint iFlag = 0;

	EnterCriticalSection(&(pLoading->Get_Crt()));

	switch (pLoading->Get_LoadingID())
	{
	case LOADING_STAGE:
		iFlag = pLoading->Loading_ForStage();
		break;

	case LOADING_BOSS:
		// iFlag = pLoading->Loading_ForBoss();
		break;
	}

	LeaveCriticalSection(&(pLoading->Get_Crt()));

	return iFlag;
}

CLoading * CLoading::Create(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eID)
{
	CLoading*		pInstance = new CLoading(pGraphicDev);

	if (FAILED(pInstance->Ready_Loading(eID)))
	{
		MSG_BOX("Loading Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CLoading::Free(void)
{
	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);
	DeleteCriticalSection(&m_Crt);
	Safe_Release(m_pGraphicDev);
}
