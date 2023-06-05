#pragma once
#include "GameObject.h"
class CFlightSpot : public CGameObject
{
private:
	explicit CFlightSpot(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFlightSpot();

public:
	virtual HRESULT Ready_Object(void);
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual void	Render_Object(void) override;

private:
	void			Positioning(void);

private:
	HRESULT			Add_Component(void);
	CTransform*		m_pTransform = nullptr;
	CCalculator*	m_pCalculator = nullptr;
	CCubeCol*		m_pCube = nullptr;

public:
	static CFlightSpot*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void			Free(void);
};

