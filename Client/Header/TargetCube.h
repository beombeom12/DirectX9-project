#pragma once
#include "GameObject.h"

class CMonsterUI;
class CRainbowCloudEffect;
class CTargetCube :	public CGameObject
{
private:
	explicit CTargetCube(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTargetCube();

public:
	virtual HRESULT		Ready_Object(const _vec3& vPos, const _vec3& vDir, const _vec3& vScale, const _int& m_iIndex);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;
	void				Set_Damaged(_int iDamage) { m_tAbility->fCurrentHp -= iDamage; }
	MONSTERABILITY*		Get_MAbility() { return m_tAbility; }

private:
	HRESULT				Add_Component(void);
	void				Hit_Check(const _float& fTimeDelta);

private:
	CCubeTex*			m_pBufferCom = nullptr;
	CTransform*			m_pTransCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CCalculator*		m_pCalculatorCom = nullptr;
	CCollision*			m_pCollision = nullptr;

	CRcTex*				m_pBufferUICom = nullptr;
	CTexture*			m_pTextureUICom = nullptr;
	CTransform*			m_pTransUICom = nullptr;

	CMonsterUI*			m_pMonsterUI = nullptr;

	_float				m_fUISwitchTime = 0.f;

	CHitBox*			m_pHitBox = nullptr;
	_float				m_fFrame = 0.f;

	_vec3				m_vDir = _vec3(0.f, 0.f, 0.f);
	_vec3				m_vScale = _vec3(0.f, 0.f, 0.f);
	_float				m_BeforeHp;

private:
	MONSTERABILITY*		m_tAbility;
	CRainbowCloudEffect* m_pRainbowCloudEffectParticle = nullptr;

public:
	static CTargetCube*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos , const _vec3& vDir, const _vec3& vScale, const _int& iIndex);
	virtual void	Free(void);
};

