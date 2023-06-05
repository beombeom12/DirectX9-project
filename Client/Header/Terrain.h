#pragma once
#include "GameObject.h"

class CTerrain : public CGameObject
{
private:
	explicit CTerrain(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTerrain();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual void	Render_Object(void) override;

	void	Set_WireFrame(_bool bWireFrame) { m_bWireFrame = bWireFrame; }
	_bool*	Get_WireFrame() { return &m_bWireFrame; }

private:
	HRESULT				Add_Component(void);
	HRESULT				Set_Material(void);

public:
	_int				m_iTerrainIdx = 0;
private:
	CTerrainTex*		m_pBufferCom = nullptr;
	CTransform*			m_pTransCom = nullptr;
	CTexture*			m_pTextureCom_Tool = nullptr;
	CTexture*			m_pTextureCom = nullptr;

	_vec3				m_vDirection;
	_bool				m_bWireFrame = false;

public:
	static CTerrain*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void	Free(void);
};

