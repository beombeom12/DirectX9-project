#pragma once
#include "GameObject.h"

class CMonsterUI;
class CComboUI;

class CMiddleBoss :	public CGameObject
{
public:
	explicit CMiddleBoss(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMiddleBoss();

public:
	virtual HRESULT Ready_Object(const _vec3& vPos, _tchar* Name);
	virtual _int      Update_Object(const _float& fTimeDelta) override;
	virtual void      LateUpdate_Object(void) override;
	virtual void      Render_Object(void) override;

public:
	void         Set_CollisionDmg() { m_tAbility->fCurrentHp -= 50.f; }

private:
	_int         Update_Pattern(_float fTimeDelta);
	void         Hit_Check(_float _deltaTime);

private:
	HRESULT      Build(void);
	void         Load_Animation(wstring FileName, _uint AnimationID);
	void         Run_Animation(const _float& AnimationSpeed);
	void         Look_Direction(void);

private:
	void         Walk_Animation_Run(void);
	void         Idle_Animation_Run(void);
	void         NormalAttack_Animation_Run(void);
	void         Bombing_Animation_Run(void);
	void         Dead_Animation_Run(void);
	void		 Hit_Effect(void);
	void		 LaserReady_Effect(void);
	void		 BombState_Effect(void);
	void		 Dead_Effect(void);

private:
	void         Set_OnTerrain(void);
	_bool        Collision_Wall(const _float& fTimeDelta);

private:
	_uint			m_MissileCnt = 0;
	_float          m_fMissileItv = 0.f;
	_float          m_fLaserTime = 0.f;
	_float          m_fTimeDelta = 0.f;
	_bool           m_bFirst = true;
	_float          m_AnimationTime = 0.f;
	CLayer*         pMyLayer = nullptr;
	_tchar*         m_MonsterName;
	list<_tchar*>   m_TcharList;

private:
	MIDDLEBOSSSTATEID			m_STATE;
	MIDDLEBOSSWALKID			m_WALK;
	MIDDLEBOSSIDLEID			m_IDLE;
	MIDDLEBOSSNORMALATTACKID    m_NORMALATTACK;
	MIDDLEBOSSBOMBINGID         m_BOMBING;
	MIDDLEBOSSCRASHID           m_CRASH;
	MIDDLEBOSSSKILLID           m_PATTERN;
	MIDDLEBOSSDEADID            m_DEAD;

private:
	HRESULT            Monster_Mapping(void);
	HRESULT			   Create_Monster();

protected:
	CTransform*        m_pTransCom = nullptr;
	CTransform*        m_pTransUICom = nullptr;
	CTransform*        m_pHitBoxTransCom = nullptr;
	CTransform*        m_pSphereTransCom = nullptr;
	CTransform*        m_pMonsterMapping = nullptr;
	CTransform*        m_pPlayerTransCom = nullptr;
	CTransform*        m_pSearchRange_TransCom = nullptr;
	CTransform*        m_pAttackRange_TransCom = nullptr;
	CTransform*        m_pRunawayRange_TransCom = nullptr;

	CTexture*         m_pTextureCom = nullptr;
	CCubeTex*         m_pBufferCom = nullptr;
	CSphereTex*       m_pSphereBufferCom = nullptr;
	CMonsterUI*       m_pMonsterUI = nullptr;
	CRcTex*           m_pBufferUICom = nullptr;
	CTexture*         m_pTextureUICom = nullptr;
	CHitBox*          m_pHitBox = nullptr;
	CCollision*       m_pCollision = nullptr;
	CComboUI*         m_pComboUI = nullptr;
	CCubeCol*         m_pAnimationBox = nullptr;
	CCalculator*      m_pCalculatorCom = nullptr;
	
private:
	_float            m_ReloadTimer = 0.f;

	_bool			  m_bPatternStart = false;
	_float			  m_fBeforeHp =- 0.f;
	_float			  m_fFrame = 0.f;
	_float			  m_fTargetLength = 0.f;
	_float			  m_fDetectRange = 0.f;
	_float			  m_fAttackRange = 0.f;
	_float			  m_fUISwitchTime = 0.f;
	_float			  m_fFireAngle = 0.f;
	_float			  m_fSpeed = 1.f;
	_vec3			  vUIPos = _vec3(0.f,0.f,0.f);
	_bool			  m_MappingInit = false;

	_int              m_iRandom = 0;
	TCHAR*            m_szCntName = new TCHAR[64];

	vector<MIDDLEBOSSSKILLID>   m_vPattern;

	vector<_vec3>	  m_vMonterPos;
	vector<_int>	  m_vMonstertype;
	_int			  m_iMonsterCnt = 0;

	list<TCHAR*>      m_listMonsterCnt;
	list<_tchar* >    NameList;

	_float			 m_fCreateMonFrame = 0.f;
	_vec3			 m_vCorePos;


private:
	HRESULT            Add_Component(void);
	MIDDLEBOSSABILITY*      m_tAbility;

public:
	static CMiddleBoss*   Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos, _tchar* Name);
	virtual void   Free(void);
};