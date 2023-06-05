#pragma once
#include "Item.h"
class CAcquireDefense :
	public CItem
{
public:
	explicit	CAcquireDefense(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit	CAcquireDefense(const CItem &rhs);
	virtual ~CAcquireDefense();

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
	static CAcquireDefense* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 vPos);

public:
	void		Free() override;

};

