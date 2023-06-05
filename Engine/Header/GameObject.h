#pragma once

#include "Component.h"
#include "Engine_Include.h"

BEGIN(Engine)

class ENGINE_DLL CGameObject : public CBase
{
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CGameObject(const CGameObject& rhs);
	virtual ~CGameObject();

public:
	CComponent*				Get_Component(const _tchar* pComponentTag, COMPONENTID eID);

public:
	void		Set_DrawTexIndex(_int iTexIndex) { m_iTexIndex = iTexIndex; }
	const _int	Get_DrawTexIndex()const { return m_iTexIndex; }
	_bool		Get_Dead() { return m_bDead; }

public:
	virtual		HRESULT		Ready_Object(void);
	virtual		_int		Update_Object(const _float& fTimeDelta);
	virtual		void		LateUpdate_Object(void);
	virtual		void		Render_Object(void);
	virtual		HRESULT		Set_Material();

	void	SetCheckFrustum(_bool Frustum) { m_bFrustumCheck = Frustum; }
	_bool	GetCheckFrustum() { return m_bFrustumCheck; }

	void	SetSphereSkill(_bool SphereSkill) { m_bSphereSkill = SphereSkill; }
	_bool	GetSphereSkill() { return m_bSphereSkill; }

	void	SetSphereSkillTag(_int SphereSkillTag) { m_iSphereSkillTag = SphereSkillTag; }
	_int	GetSphereSkillTag() { return m_iSphereSkillTag; }
	_float	GetSphereScale() { return m_fSphereScale; }

private:
	CComponent*		Find_Component(const _tchar* pComponentTag, COMPONENTID eID);

protected:
	LPDIRECT3DDEVICE9					m_pGraphicDev;
	map<const _tchar*, CComponent*>		m_mapComponent[ID_END];

public:
	void			Kill_Obj(void) { m_bDead = true; }
	SUPPORTER_ID		m_eSupporterID = SUPPORTER_END;

protected:
	_bool				m_bDead = false;
	_int				m_iTexIndex = 0;

	_bool				m_bFrustumCheck = true;

	_bool				m_bSphereSkill = false;
	_int 				m_iSphereSkillTag = 0;
	_float				m_fSphereScale = 0.f;

public:
	virtual void	Free(void);
};

END