#pragma once
#include "GameObject.h"

class CMonsterUI;
class CMonsterParticle;
class CComboUI;

class CMonster : public CGameObject
{
protected:
	explicit CMonster(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMonster();

protected:
	virtual HRESULT		Ready_Object(const _vec3& vPos, wstring _strObjTag);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

	MONSTERABILITY*		Get_MAbility() { return m_tAbility; }
	HRESULT				Monster_Mapping(void);
	HRESULT				Monster_DeleteMapping(void);

	_bool				Collision_Wall(const _float& fTimeDelta);

	void				Hit_Effect();

protected:
	void				Hit_Check(_float _deltaTime);
	void				Hit_SphereCheck(_float _deltaTime);
	void				Skill_SphereCheck(_float _deltaTime);

protected:
	void            MiddleBoss_LimitBreak(void);
	void            Kraken_LimitBreak(void);
	_bool           MiddleBoss_Limit = true;
	_bool           Kraken_Limit = true;

protected:
	void				Set_On_Terrain(void);

public:
	void          Set_Damaged(_float iDamage)
	{
		if(this != nullptr)
			m_tAbility->fCurrentHp -= iDamage;
	}
	void		 Set_CollisionDmg() { m_bCollisionDmg = true; }
protected:
	CCubeTex*			m_pBufferCom = nullptr;
	CTransform*			m_pTransCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CCalculator*		m_pCalculatorCom = nullptr;
	CCollision*			m_pCollision = nullptr;

	CRcTex*				m_pBufferUICom = nullptr;
	CTexture*			m_pTextureUICom = nullptr;
	CTransform*			m_pTransUICom = nullptr;

	CHitBox*			m_pHitBox = nullptr;
	CTransform*			m_pHitBoxTransCom = nullptr;

	CMonsterUI*			m_pMonsterUI = nullptr;

	CTransform*			m_pPlayerTransCom = nullptr;

	CSphereTex*			m_pSphereBufferCom = nullptr;

	////////////////////////////////////////////////////////////////////////
	CTransform*			m_pSphereTransCom = nullptr;
	////////////////////////////////////////////////////////////////////////
	CTransform*			m_pSearchRange_TransCom = nullptr;		//	추적범위
	CTransform*			m_pAttackRange_TransCom = nullptr;		//	공격범위
	CTransform*			m_pRunawayRange_TransCom = nullptr;		//	근처에오면 도망(원거리몹)
	////////////////////////////////////////////////////////////////////////

	_bool				m_bisHit = false;
	_bool				m_bDetect = false;

	MONSTERABILITY*		m_tAbility;

	CMonsterParticle*	m_pHitParicle = nullptr;
	CComboUI*			m_pComboUI = nullptr;
	
	_float				m_fUISwitchTime = 0.f;

	_bool				m_MappingInit = false;
	list<TCHAR*>		m_listMonsterCnt;
	TCHAR*				m_szCntName = new TCHAR[64];

	CTransform*			m_pMonsterMapping = nullptr;

	_float				m_BeforeHp;
	_float				m_fSpeed = 1.f;
	_float				m_fSoundTimer = 0.f;
	_float				m_fIdleSound = 0.2f;
	_float				m_fHitSound = 0.3f;
	_float				m_fDeadSound = 1.f;

	_vec3				m_vDirection;
	_bool				m_bCollisionDmg = false;
protected:
	virtual void	Free(void)override;
};

