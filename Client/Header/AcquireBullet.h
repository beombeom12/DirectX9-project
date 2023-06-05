#pragma once
#include "Item.h"
class CAcquireBullet :
	public CItem
{
public:
	explicit	CAcquireBullet(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit	CAcquireBullet(const CItem &rhs);
	virtual ~CAcquireBullet();

public:
	HRESULT Ready_Object(const _vec3& vPos) override;
	_int	Update_Object(const _float& fTimeDelta) override;
	void	LateUpdate_Object() override;
	void	Render_Object() override;
	//void	Get_

private:
	HRESULT Add_Component(void);
	CHitBox* m_pHitBox = nullptr;

public:
	static CAcquireBullet* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 vPos);

public:
	void		Free() override;

};

