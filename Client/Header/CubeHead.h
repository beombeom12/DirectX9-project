#pragma once
#include "GameObject.h"
#include "Export_Function.h"

USING(Engine)

class CCubeHead : public CGameObject
{
private:
	explicit CCubeHead(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCubeHead();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual void	Render_Object(void) override;

private:
	void			Key_Input(const _float& fTimeDelta);

private:
	HRESULT			Add_Component(void);
	CTransform*		m_pTransform = nullptr;
	CTexture*		m_pTexture = nullptr;
	CCubeTex*		m_pCube = nullptr;

public:
	static CCubeHead*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void Free(void);
};

