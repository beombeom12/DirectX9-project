#pragma once
#include "GameObject.h"

class CRcEffect;
class CSparkEffect;
class CFlameEffect;

class CMeteor :	public CGameObject
{
private:
	explicit CMeteor(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMeteor();

public:
	virtual		HRESULT		Ready_Object(const _vec3& Position);
	virtual		_int		Update_Object(const _float& fTimeDelta);
	virtual		void		LateUpdate_Object(void);
	virtual		void		Render_Object(void);

	_float		Get_Attack() { return m_fAttack; }

private:
	HRESULT					Add_Component(void);
	void					Meteor_Effect(void);

private:
	CTransform*				m_pTransCom = nullptr;
	CSphereTex*				m_pSphereTex = nullptr;
	CTexture*				m_pTexture = nullptr;
	CGameObject*			m_pEffect = nullptr;
	CSparkEffect*			m_SparkEffectParticle = nullptr;
	CFlameEffect*			m_pFlameEffectParticle = nullptr;

	_vec3					m_vTarget;
	_vec3					m_vPos;
	_float					m_fTimer = 0.f;
	_float					m_fParticleTimer = 0.f;

	_float					m_fAttack = 0.f;

public:
	static CMeteor*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& Position);
	virtual void	Free(void);
};

