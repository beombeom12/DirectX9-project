#pragma once
#include "GameObject.h"

class CLaser : public CGameObject
{
private:
	explicit CLaser(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLaser();

public:
	virtual HRESULT Ready_Object(const _vec3* pPos, const _vec3* pDir, _float _fSpeed, _int _iIndex);
	virtual _int   Update_Object(const _float& fTimeDelta);
	virtual void   LateUpdate_Object(void);
	virtual void   Render_Object(void);

public:
	void         Set_Pos(const _vec3& vPos);
	void         Set_Dir(const _vec3& vDir) { m_vDirection = vDir; }
	void         Restore(void) { m_bDead = false; }

private:
	void         Collision_check(void);
	void         Bomb_effect(void);
	void         Bomb_Collision(void);
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
	_int   ix = 0;
	_int   iz = 0;
	list<_tchar*>   m_TcharList;
public:
	static CLaser*   Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pPos, const _vec3* pDir, _float _fSpeed = 10.f, _int _iIndex = 1);
	virtual void      Free(void);
};
