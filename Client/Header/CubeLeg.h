#pragma once
#include "GameObject.h"
#include "Export_Function.h"

USING(Engine)

class CCubeLeg : public CGameObject
{
private:
	explicit CCubeLeg(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCubeLeg();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual void	Render_Object(void) override;

private:
	HRESULT			Add_Component(void);
	CTransform*		m_pTransform = nullptr;
	CTexture*		m_pTexture = nullptr;
	CCubeTex*		m_pCube = nullptr;
	CLayer*			m_pMyLayer = nullptr;

public:
	static CCubeLeg*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);
};

