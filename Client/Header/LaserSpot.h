#pragma once
#include "GameObject.h"
class CLaserSpot : public CGameObject
{
private:
	explicit CLaserSpot(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLaserSpot();

public:
	virtual	HRESULT Ready_Object(const _vec3& vPos);
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual void	Render_Object(void) override;

private:
	HRESULT			Add_Component(void);
	CTransform*		m_pTransCom = nullptr;
	CCubeCol*		m_pCubeCol = nullptr;
	CCollision*		m_pCollision = nullptr;
	CHitBox*		m_pHitBox = nullptr;

	CTransform*		m_pPlayerTrans = nullptr;

public:
	void			Attack_Permit(_bool is) { m_bAttack = is; }
private:
	_bool			m_bAttack = false;

public:
	static CLaserSpot*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos);
	virtual void		Free(void);
};