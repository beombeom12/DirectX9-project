#pragma once
#include "GameObject.h"

class CExBullet : public CGameObject
{
private:
	explicit CExBullet(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CExBullet();

public:
	virtual HRESULT Ready_Object(const _vec3* pPos, const _vec3* pDir, _float _fSpeed);
	virtual _int   Update_Object(const _float& fTimeDelta);
	virtual void   LateUpdate_Object(void);
	virtual void   Render_Object(void);

public:
	void         Set_Pos(const _vec3& vPos);
	void         Set_Dir(const _vec3& vDir) { m_vDirection = vDir; }
	void         Restore(void) { m_bDead = false, m_fTimeDelta = 0.f;	_float fSound = 1.f;
	Engine::PlaySoundGun(L"exbullet.wav", SOUND_EFFECT, fSound);
	}
private:
	void         Collision_check(void);
	void         Effect(void);

private:
	HRESULT         Add_Component(void);
	CTransform*      m_pTransCom = nullptr;
	CCubeCol*      m_pCubeCol = nullptr;
	CCubeTex*            m_pCube = nullptr;

	CTexture*            m_pTexture = nullptr;

	CTransform*      m_pHitboxTransCom = nullptr;
	CHitBox*      m_pHitbox = nullptr;

	CCollision*      m_pCollision = nullptr;

private:
	_vec3         m_vDirection = _vec3(0.f, 0.f, 0.f);
	_float         m_fSpeed = 10.f;
	_float         m_fTimeDelta = 0.f;

	list<_tchar*>   m_TcharList;

public:
	static CExBullet*   Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pPos, const _vec3* pDir, _float _fSpeed = 10.f);
	virtual void      Free(void);
};
