#pragma once

// Shader Name
#define RCTEX_SHADER   L"Proto_ShaderRect"
#define CUBETEX_SHADER   L"Proto_ShaderCube"

// 컴포넌트 이름
#define RCTEX_COMP       L"Proto_RcTexCom"
#define   CUBETEX_COMP   L"Proto_CubeTexCom"
#define TERRAINTEX_COMP L"Proto_TerrainTexCom"
#define SPHERETEX_COMP   L"Proto_SphereTexCom"
#define FLEXIBLEAXISCUBE_COMP L"Proto_FlexibleAxisCubeCom"
#define CUBECOL_COMP      L"Proto_CubeColCom"

// Loading
#define RCTEX_HP_COMP         L"RcTex_HP"
#define RCTEX_DEFENCE_COMP      L"RcTex_Defense"
#define RCTEX_BULLET_COMP      L"RcTex_Bullet"
#define RCTEX_MONTER_HP_COMP   L"Proto_MonsterHpCom"
#define RCTEX_GENERAL_MONSTER_HP_COMP L"RcTex_MonsterHp"
#define RCTEX_GENERAL_MONSTER_HUD_COMP L"RcTex_MonsterHUDHp"
#define RCTEX_COMBO_COMP            L"RcTex_ComboBar"
#define RCTEX_COMBOBAR_HUD_COMP          L"RcTex_ComboBarHUD"

#define   TRANSFORM_COMP  L"Proto_TransformCom"
#define ITEM_TRANSFORM_COMP L"ITEM_HITBOX_TransformCom"

#define COLLISION_COMP   L"Proto_CollisionCom"
#define CALCULATOR_COMP L"Proto_CalculatorCom"

#define HITBOX_COMP      L"Proto_HitboxCom"

#define QUATERNION_COMP L"Proto_QuaternionCom"

// FONT
#define ROBOTO_BOLD      L"Roboto-Bold"

// 레이어 이름
#define LOGO_ENVIRONMENT L"Logo_Environment_Layer"
#define LOGO_UI          L"Logo_UI_Layer"

// LOGO UI
#define LOGO_UI_TEX      L"Proto_LogoUITexture"
#define OPTION_BTN_TEX   L"Proto_OptionButton"
#define PLAY_BTN_TEX   L"Proto_ButtonPlayTexture"
#define   EXIT_BTN_TEX   L"Proto_ExitButton"

// PATH
#define LOGO_UI_TEX_PATH    L"../Bin/Resources/Title/TitleMenu.png"
#define OPTION_BTN_TEX_PATH    L"../Bin/Resources/UI/MenuUI/UI%d.png"
#define PLAY_BTN_TEX_PATH    L"../Bin/Resources/UI/MenuUI/UI%d.png"
#define   EXIT_BTN_TEX_PATH    L"../Bin/Resources/UI/MenuUI/UI%d.png"

// TERRAIN
#define LOGO_TERRAIN_TEX      L"Proto_TerrainTexture_Stage_1"
#define LOGO_TERRAIN_TEX_PATH  L"../Bin/Resources/Texture2D/tex_stone_3.png"

// SKYBOX
#define LOGO_SKYBOX_TEX         L"Proto_CubeTexture"
#define LOGO_SKYBOX_TEX_PATH   L"../Bin/Resource/Texture/SkyBox/burger%d.dds"

#define STAGE_ENVIRONMENT  L"STAGE_Environment_Layer"
#define   STAGE_GAMELOGIC    L"STAGE_GameLogic_Layer"
#define   STAGE_UI         L"STAGE_UI_Layer"
#define STAGE_CHARACTER    L"STAGE_CHARACTER_Layer"
#define STAGE_MONSTER      L"STAGE_MONSTER_Layer"   
#define STAGE_MAPPING      L"STAGE_MAPPING_Layer"
#define STAGE_BULLET      L"STAGE_BULLET_Layer"   
#define STAGE_WALL         L"STAGE_WALL_Layer"
#define STAGE_ITEM         L"STAGE_ITEM_Layer"   
#define STAGE_GUN         L"STAGE_GUN_Layer"
#define STAGE_GUNITEM      L"STAGE_GUNITEM_Layer"
#define STAGE_PLAYER      L"STAGE_PLAYER_Layer"
#define   STAGE_ZOMBIE      L"STAGE_ZOMBIE_Layer"
#define STAGE_DESTORYWALL    L"STAGE_DESTROYWALL_Layer"
#define STAGE_SKILL         L"STAGE_SKILL_Layer"
#define STAGE_SKILLCRUISER   L"STAGE_SKILLCRUISER_Layer"

#define STAGE_TRAP         L"STAGE_TRAP_Layer"
#define STAGE_SUPPORTER      L"STAGE_SUPPORTER_Layer"
#define STAGE_BULLETPLAYER L"STAGE_PLAYERBULLET_Layer"
#define STAGE_CREATURE      L"STAGE_CREATURE_Layer"
#define STAGE_FLIGHTPLAYER L"STAGE_PLAYERFLIGHT_Layer"
#define STAGE_EXBULLET      L"STAGE_EXBULLET_Layer"
#define STAGE_LASER      L"STAGE_LASER_Layer"
#define	STAGE_KRAKENBULLET	L"STAGE_KRAKENBULLET_Layer"
#define STAGE_TENTACLE      L"STAGE_TENTACLE_Layer"

// Particle
#define SHOTPARTICLE_TEX         L"SHOTPARTICLE_TEX"
#define  SHOTPARTICLE_TEX_PATH      L"../Bin/Resources/Particle/fireball_hit_big0.png"
#define BULLETPARTICLE_TEX         L"BULLETPARTICLE_TEX"
#define BULLETPARTICLE_TEX_PATH      L"../Bin/Resources/Particle/BulletParticle.png"
#define PROJECTILEPARTICLE_TEX      L"PROJECTILEPARTICLE_TEX"
#define PROJECTILEPARTICLE_TEX_PATH   L"../Bin/Resources/Particle/BulletParticle.png"
#define STATICPARTICLE_TEX         L"STATICPARTICLE_TEX"
#define STATICPARTICLE_TEX_PATH      L"../Bin/Resources/Particle/StaticParticle/StaticParticle%d.png"

//Effect
#define BLUEBOMB_TEX            L"BLUEBOMB_TEX"
#define BLUEBOMB_TEX_PATH         L"../Bin/Resources/Effect/BlueBomb/BlueBomb%d.png"

#define BLUEEFFECT_TEX            L"BLUEEFFECT_TEX"
#define BLUEEFFECT_TEX_PATH         L"../Bin/Resources/Effect/BlueEffect/BlueEffect%d.png"
#define CASTINGEFFECT_TEX         L"CASTINGEFFECT_TEX"
#define CASTINGEFFECT_TEX_PATH      L"../Bin/Resources/Effect/CastingEffect/CastingEffect%d.png"
#define STAREFFECT_TEX            L"STAREFFECT_TEX"
#define STAREFFECT_TEX_PATH         L"../Bin/Resources/Effect/StarEffect/StarEffect%d.png"
#define COIN_TEX               L"COIN_TEX"
#define COIN_TEX_PATH            L"../Bin/Resources/Effect/Coin/Coin%d.png"
#define CREATEEFFECT_TEX         L"CREATEEFFECT_TEX"
#define CREATEEFFECT_TEX_PATH      L"../Bin/Resources/Effect/CreateEffect/CreateEffect%d.png"

#define EXPLOSION_TEX         L"EXPLOSION_TEX"
#define EXPLOSION_TEX_PATH      L"../Bin/Resources/Effect/Explosion/Explosion%d.png"
#define LIGHTNING_TEX         L"LIGHTNING_TEX"
#define LIGHTNING_TEX_PATH      L"../Bin/Resources/Effect/Lightning/Lightning%d.png"
#define REDBOMB_TEX            L"REDBOMB_TEX"
#define REDBOMB_TEX_PATH      L"../Bin/Resources/Effect/RedBomb/RedBomb%d.png"
#define REDEFFECT_TEX         L"REDEFFECT_TEX"
#define REDEFFECT_TEX_PATH      L"../Bin/Resources/Effect/RedEffect/RedEffect%d.png"
#define STUN_TEX            L"STUN_TEX"
#define STUN_TEX_PATH         L"../Bin/Resources/Effect/Stun/Stun%d.png"


// CrossHair

#define CROSSHAIR_RIFLE_TEX          L"Proto_CrossHeader_Rifle"
#define CROSSHAIR_RIFLE_TEX_PATH    L"../Bin/Resources/UI/Sprite/CrossHair_Rifle.png"
#define CROSSHAIR_SHOTGUN_TEX       L"Proto_CrossHeader_ShotGun"
#define CROSSHAIR_SHOTGUN_PATH       L"../Bin/Resources/UI/Sprite/CrossHair_ShotGun.png"
#define CROSSHAIR_SNIPER_TEX       L"Proto_CrossHeader_Sniper"
#define CROSSHAIR_SNIPER_TEX_PATH    L"../Bin/Resources/UI/Sprite/CrossHair_Sniper.png"

// PNGtoCUBE
#define BULLET_ITEM_BUFFER         L"BULLET_ITEM"
#define BULLET_ITEM_TEX            L"BULLET_ITEM_TEX"
#define BULLET_ITEM_TEX_PATH      L"../Bin/Resources/PngToCube/ammo_box_pistol.png"

#define ADDMAXHP_ITEM_BUFFER      L"ADDMAXHP_ITEM"
#define ADDMAXHP_ITEM_TEX         L"ADDMAXHP_ITEM_TEX"
#define ADDMAXHP_ITEM_TEX_PATH      L"../Bin/Resources/PngToCube/AddMaxHp.png"

#define ADDDEFENCE_ITEM_BUFFER      L"ADDDEFENCE_ITEM"
#define ADDDEFENCE_ITEM_TEX         L"ADDDEFENCE_ITEM_TEX"
#define ADDDEFENCE_ITEM_TEX_PATH   L"../Bin/Resources/PngToCube/Defence.png"

#define ADDHP_ITEM_BUFFER         L"ADDHP_ITEM"
#define ADDHP_ITEM_TEX            L"ADDHP_ITEM_TEX"
#define ADDHP_ITEM_TEX_PATH         L"../Bin/Resources/PngToCube/HP.png"

#define SNIPER_ITEM_BUFFER         L"SNIPER_ITEM"
#define SNIPER_ITEM_TEX            L"SNIPER_ITEM_TEX"
#define SNIPER_ITEM_TEX_PATH      L"../Bin/Resources/PngToCube/cannonui.png"

#define SHOTGUN_ITEM_BUFFER         L"SHOTGUN_ITEM"
#define SHOTGUN_ITEM_TEX         L"SHOTGUN_ITEM_TEX"
#define SHOTGUN_ITEM_TEX_PATH      L"../Bin/Resources/PngToCube/ssg_hud.png"

#define RIFLE_ITEM_BUFFER         L"RIFLE_ITEM"
#define RIFLE_ITEM_TEX            L"RIFLE_ITEM_TEX"
#define RIFLE_ITEM_TEX_PATH         L"../Bin/Resources/PngToCube/rifle_hud.png"

#define BLUEKEY_ITEM_BUFFER         L"BLUEKEY_ITEM"
#define BLUEKEY_ITEM_TEX         L"BLUEKEY_ITEM_TEX"
#define BLUEKEY_ITEM_TEX_PATH      L"../Bin/Resources/PngToCube/TEXTURE_KEY_BLUE.png"

#define REDKEY_ITEM_BUFFER         L"REDKEY_ITEM"
#define REDKEY_ITEM_TEX            L"REDKEY_ITEM_TEX"
#define REDKEY_ITEM_TEX_PATH      L"../Bin/Resources/PngToCube/TEXTURE_KEY_RED.png"

#define YELLOWKEY_ITEM_BUFFER      L"YELLOWKEY_ITEM"
#define YELLOWKEY_ITEM_TEX         L"YELLOWKEY_ITEM_TEX"
#define YELLOWKEY_ITEM_TEX_PATH      L"../Bin/Resources/PngToCube/TEXTURE_KEY_YELLOW.png"



// 이미지 경로
#define HP_100_FACE_PATH      L"../Bin/Resources/UI/Player/Face/HP100_%d.png"
#define HP_75_FACE_PATH         L"../Bin/Resources/UI/Player/Face/HP75_%d.png" 
#define HP_50_FACE_PATH         L"../Bin/Resources/UI/Player/Face/HP50_%d.png" 
#define HP_25_FACE_PATH         L"../Bin/Resources/UI/Player/Face/HP25_%d.png" 
#define HP_0_FACE_PATH         L"../Bin/Resources/UI/Player/Face/HP0.png"

// 몬스터 UI
#define MONSTER_HP_TEX            L"Monster_Hp"
#define MONSTER_HP_TEX_PATH         L"../Bin/Resources/UI/MonsterUI/Bar_monster_HP.png"
#define   MONSTER_HP_HUD_TEX         L"Monster_Hud"
#define MONSTER_HP_HUD_TEX_PATH      L"../Bin/Resources/UI/MonsterUI/HUD_bar_monster_HP.png"

#define HITBAR_TEX            L"HITBAR_TEX"
#define   HITBAR_TEX_PATH         L"../Bin/Resources/UI/Player/hitbar.png"

#define COMBOBAR_TEX            L"ComboBar"
#define COMBOBAR_TEX_PATH         L"../Bin/Resources/UI/Player/Gage/comboBar.png"
#define   COMBOBAR_HUD_TEX         L"ComboBar_Hud"
#define COMBOBAR_HUD_TEX_PATH      L"../Bin/Resources/UI/Player/Gage/comboHUD.png"

#define   WARNING_TEX         L"WarningUI"
#define   WARNING_TEX_PATH         L"../Bin/Resources/UI/Player/Warning%d.png"
