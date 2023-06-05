#pragma once
#include "GameObject.h"

class CObjectCube :	public CGameObject
{

private:
	explicit CObjectCube(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CObjectCube(const CGameObject& rhs);
	virtual ~CObjectCube();
public:
	HRESULT				Ready_Object(int PosX, int PosY, OBJECT_ID eID);

	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		Render_Object(void) override;

	_bool*				Get_WireFrame() { return &m_bWireFrame; }
	void				Set_WireFrame(_bool bWireFrame) { m_bWireFrame = bWireFrame; }
	void				Set_TransformPositon();
	bool				Set_SelectGizmo();

private:
	HRESULT				Add_Component(void);

private:
	_bool				m_bWireFrame = false;

private:
	CCubeTex*			m_pBufferCom = nullptr;
	CTransform*			m_pTransCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CCalculator*		m_pCalculatorCom = nullptr;

public:
	static CObjectCube*		Create(LPDIRECT3DDEVICE9 pGraphicDev, OBJECT_ID eID = GUN, int Posx = 0, int Posy = 0);

protected:
	virtual void Free();
};

