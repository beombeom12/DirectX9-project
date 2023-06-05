#pragma once
#include "GameObject.h"

class CMonsterUI;
class CComboUI;

class CKrakenBoss :
	public CGameObject
{
public:
	explicit CKrakenBoss(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CKrakenBoss();


public:
	virtual	HRESULT Ready_Object(const _vec3& vPos, _tchar* Name);
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	Render_Object(void) override;
	virtual void	LateUpdate_Object(void) override;

public:
	void			Set_Damaged(_float iDamage)
	{
		if (this != nullptr)
			m_tAbility->fCurrentHp -= iDamage;
	}

public:
	void				Hit_SphereCheck(_float _DeltaTime);
	_bool				m_bCollisionDmg = false;
	void				Set_CollisionDmg() { m_bCollisionDmg = true; }

public:
	_float				Get_Hp(void) { return m_tAbility->fCurrentHp; }

private:
	void				Look_Direction(void);
	virtual _int		Update_Pattern(_float fTimeDelta);
	void				Hit_Check(_float _deltaTime);
	void				Dead_Event(void);

private:
	HRESULT				Build(void);
	void				Load_Animation(wstring FileName, _uint AnimationID);
	void				Run_Animation(const _float& AnimationSpeed);

private:
	void				APPEAR(void);
	void				IDLE_Animation_Run(void);

private:
	KRAKENSTATEID		m_STATE;
	KRAKENIDLEID		m_IDLE;
	KRAKENAPPEARID		m_APPEAR;
	KRAKENSMASHID		m_SMASH;
	KRAKENINKSHOTID		m_INKSHOT;
	KRAKENROLLINGID		m_ROLLING;
	KRAKENLURKERID		m_LURKER;
	KRAKENSKILLID		m_PATTERN;

private:
	void				Set_OnTerrain(void);

protected:
	HRESULT				Add_Component(void);
	CTransform*			m_pTransCom = nullptr;
	CTransform*			m_pTransUICom = nullptr;
	CTransform*			m_pHitBoxTransCom = nullptr;
	CTransform*			m_pSphereTransCom = nullptr;
	CTransform*			m_pMonsterMapping = nullptr;
	CTransform*			m_pPlayerTransCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CCubeTex*			m_pBufferCom = nullptr;
	CSphereTex*			m_pSphereBufferCom = nullptr;
	CMonsterUI*			m_pMonsterUI = nullptr;
	CRcTex*				m_pBufferUICom = nullptr;
	CCalculator*		m_pCalculatorCom = nullptr;
	CTexture*			m_pTextureUICom = nullptr;
	CHitBox*			m_pHitBox = nullptr;
	CCollision*			m_pCollision = nullptr;
	CComboUI*			m_pComboUI = nullptr;
	CLayer*				pMyLayer;
	CCubeCol*			m_pAnimationBox = nullptr;

private:
	KRAKENABILITY*		m_tAbility;
	_tchar*				m_MonsterName;
	_bool				m_bFirst = true;
	list<_tchar*>		m_TcharList;

	vector<KRAKENSKILLID>	m_vPattern;
	HRESULT			   Create_Monster();
private:
	_float				m_ReloadTimer = 0.f;
	_float				m_AnimationTime = 0.f;
	_float				m_fTimeDelta = 0.f;
	_float				m_fUISwitchTime = 0.f;
	_vec3				vUIPos;
	_bool				m_bAnihilate_First = true;
	_bool				m_bAnihilate_Second = true;
	_bool				m_bAnihilate_Third = true;
	_float				m_BeforeHp = 5000.f;
	_float				m_fInterval;
	_float				m_fFireAngle = 0.f;
	_bool				m_bDeadScene = true;
	_bool				m_bCameraShaking = false;

	_float				m_fMonCreFrame = 0.f;
	vector<_int>	    m_vMonstertype;
	_int				m_iMonsterCnt = 0;
public:
	static CKrakenBoss*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos, _tchar* Name);
	virtual void	Free(void);
};

