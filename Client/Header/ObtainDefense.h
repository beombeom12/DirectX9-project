#pragma once
#include "Item.h"
#include "ItemParticle.h"

class CObtainDefense :	public CItem
{
private:
	explicit CObtainDefense(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CObtainDefense();

public:
	HRESULT Ready_Object(const _vec3& vPos) override;
	_int	Update_Object(const _float& fTimeDelta) override;
	void	LateUpdate_Object() override;
	void	Render_Object() override;
private:
	HRESULT			Add_Component(void);
	CItemParticle* m_pItemParicle = nullptr;
	void	Dead_Effect(void);
	
public:
	static CObtainDefense* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos);

protected:
	void Free() override;
};

