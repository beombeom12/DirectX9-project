#pragma once
#include "ItemParticle.h"
#include "Item.h"
class CObtainBullet : public CItem
{
private:
	explicit CObtainBullet(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CObtainBullet();

public:
	HRESULT Ready_Object(const _vec3& vPos) override;
	_int	Update_Object(const _float& fTimeDelta) override;
	void	LateUpdate_Object() override;
	void	Render_Object() override;

private:
	HRESULT			Add_Component(void);
	void			Dead_Effect(void);
public:
	static CObtainBullet* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos);

private:
	CItemParticle* m_pItemParicle = nullptr;

protected:
	void Free() override;
};

