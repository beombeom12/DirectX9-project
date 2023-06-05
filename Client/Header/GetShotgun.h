#pragma once
#include "Item.h"
class CGetShotgun :	public CItem
{
public:
	explicit CGetShotgun(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CGetShotgun();

public:
	HRESULT Ready_Object(const _vec3& vPos) override;
	_int	Update_Object(const _float& fTimeDelta) override;
	void	LateUpdate_Object() override;
	void	Render_Object() override;


private:
	HRESULT			Add_Component(void);
	void			Effect();
public:
	static CGetShotgun* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos);

protected:
	void Free() override;
};
