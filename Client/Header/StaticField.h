#pragma once
#include "GameObject.h"
class CStaticField :
	public CGameObject
{
private:
	explicit CStaticField(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStaticField();


public:
	virtual		HRESULT		Ready_Object(const _vec3& Position);
	virtual		_int		Update_Object(const _float& fTimeDelta);
	virtual		void		LateUpdate_Object(void);
	virtual		void		Render_Object(void);

private:
	HRESULT					Add_Component(void);

private:
	CTransform*				m_pTransCom = nullptr;
	CSphereTex*				m_pSphereTex = nullptr;
	CSphereTex*				m_pSphereTex2 = nullptr;

	CTexture*				m_pTexture = nullptr;
	CGameObject*			m_pEffect = nullptr;

	_float					m_fAge = 0.f;
	_float					m_fDuration = 0.f;

public:
	static CStaticField*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& Position);
	virtual void	Free(void);
};

