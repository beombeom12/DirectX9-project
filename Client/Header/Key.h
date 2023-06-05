#pragma once
#include "Item.h"
class CKey : public CItem
{
public:
	explicit CKey(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CKey();

public:
	HRESULT Ready_Object(const _vec3& vPos, COLOR_ID eID);
	_int	Update_Object(const _float& fTimeDelta) override;
	void	LateUpdate_Object() override;
	void	Render_Object() override;

private:
	HRESULT			Add_Component(void);
	void			Effect();
public:
	static CKey* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos , COLOR_ID eID);
	COLOR_ID  m_eID = COLOR_END;
	
protected:
	void Free() override;
};

