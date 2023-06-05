#pragma once
#include "Scene.h"
#include "SkyBox.h"
#include "Terrain.h"
#include "StaticCamera.h"
#include "DynamicCamera.h"

class CFinalStage :
	public CScene
{
private:
	explicit CFinalStage(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFinalStage();

public:
	virtual HRESULT Ready_Scene(void) override;
	virtual _int Update_Scene(const _float& fTimeDelta) override;
	virtual void LateUpdate_Scene(void) override;
	virtual void Render_Scene(void) override;

private:
	HRESULT			Ready_Layer_Environment(const _tchar* pLayerTag);
	HRESULT			Ready_Layer_GameLogic(const _tchar* pLayerTag);
	HRESULT			Ready_Layer_UI(const _tchar* pLayerTag);

	HRESULT			Ready_Layer_Wall(const _tchar* pLayerTag);
	HRESULT			Ready_Layer_Character(const _tchar* pLayerTag);
	HRESULT			Ready_Layer_Monster(const _tchar* pLayerTag);
	HRESULT			Ready_Layer_Bullet(const _tchar* pLayerTag);
	HRESULT			Ready_Layer_PlayerBullet(const _tchar* pLayerTag);
	HRESULT			Ready_Layer_ExBullet(const _tchar* pLayerTag);
	HRESULT			Ready_Layer_Laser(const _tchar* pLayerTag);

	HRESULT			Ready_Layer_Tentacle(const _tchar* pLayerTag);

	HRESULT			Ready_Layer_Mapping(const _tchar* pLayerTag);
	HRESULT			Ready_Layer_Gun(const _tchar* pLayerTag);

	HRESULT			Ready_Layer_Item(const _tchar* pLayerTag);

	HRESULT			Ready_Layer_GunItem(const _tchar* pLayerTag);

	HRESULT			Ready_Layer_DestroyWall(const _tchar* pLayerTag);
	HRESULT			Ready_Layer_Skill(const _tchar* pLayerTag);
	HRESULT			Ready_Layer_SkillCruiser(const _tchar* pLayerTag);

	HRESULT			Ready_Layer_Creature(const _tchar* pLayerTag);

	//	ÆÄÃ÷ ·¹ÀÌ¾î
	HRESULT			Ready_Layer_Player(const _tchar* pLayerTag);

	HRESULT			Ready_Layer_Slime(const _tchar* pLayerTag);

	HRESULT			Ready_Layer_Trap(const _tchar* pLayerTag);
	HRESULT			Ready_Layer_Supporter(const _tchar* pLayerTag);

	HRESULT			Ready_Layer_PlayerFlight(const _tchar* pLayerTag);

	HRESULT			Ready_Light(void);

	HRESULT			Load_Position(OBJECT_ID eID, wstring strDirectory);
	HRESULT         Ready_Layer_KraKenBullet(const _tchar* pLayerTag);
private:
	////////////¸Ê ÆÄ½Ì
	_int			m_iIndex = 0;
	list<_tchar* >	NameList;

	_int	m_iGunIndex = 0;
	_int	m_iShopIndex = 0;
	_int	m_iLavaIndex = 0;
	_int	m_iItemIndex = 0;
	_int	m_iThroneIndex = 0;
	_int	m_iSlimeIndex = 0;
	_int	m_iFireManIndex = 0;
	_int	m_iZombieIndex = 0;
	_int	m_iIllusionerIndex = 0;

	vector<_vec3> vecGun;
	vector<_vec3> vecShop;
	vector<_vec3> vecLava;
	vector<_vec3> vecItem;
	vector<_vec3> vecThrone;
	vector<_vec3> vecSlime;
	vector<_vec3> vecFireMan;
	vector<_vec3> vecZombie;
	vector<_vec3> vecIllusioner;

	SCENE_ID m_eID = FINAL_SCENE;

public:
	static CFinalStage*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void			Free(void);
};

