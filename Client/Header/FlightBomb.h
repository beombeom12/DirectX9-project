#pragma once
#include "GameObject.h"
class CFlightBomb :
	public CGameObject
{
public:
	explicit CFlightBomb(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFlightBomb();

public:
	virtual		HRESULT		Ready_Object(const _vec3& vPos, _tchar* Name);
	virtual		_int		Update_Object(const _float& fTimeDelta);
	virtual		void		LateUpdate_Object(void);
	virtual		void		Render_Object(void);

private:
	HRESULT					Add_Component(void);
	void					Bombing(const _float& fTimeDelta);
	void					Effect();
private:
	HRESULT					Build(void);

	_bool					m_bFirst = true;

	CLayer*					pMyLayer;
	_tchar*					m_MissileName;

	list<_tchar*>			m_TcharList;

private:
	CTransform*				m_pTransCom = nullptr;

private:	
	_vec3					m_vPosition;
	_vec3					m_vDirection;
	_float					m_fSpeed;

public:
	static CFlightBomb*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos, _tchar* Name);
	virtual void	Free(void);
};

