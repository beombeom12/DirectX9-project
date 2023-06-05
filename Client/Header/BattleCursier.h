#pragma once
#include "GameObject.h"

class CBattleCursier :
	public CGameObject
{
private:
	explicit CBattleCursier(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBattleCursier();

public:
	virtual HRESULT Ready_Object(const _vec3 & vPos, const _vec3 & vDir, _tchar * Name);
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual void	Render_Object(void) override;

private:
	HRESULT			Add_Component(void);
	CTransform*		m_pTransform = nullptr;

public:
	void			Replace(const _vec3& vPos, const _vec3& vAngle, const _vec3& vDir)
	{
		m_vDirection = vDir;
		m_vAngle = vAngle;
		m_pTransform->Set_Pos(vPos.x, vPos.y, vPos.z);
	}
	void			Random(void);
	void			Set_Speed(_float fSpeed) { m_fSpeed = fSpeed; }

private:
	HRESULT			Build(void);
	void			Look_Direction(void);

	_vec3			m_vDirection;
	_vec3			m_vAngle;

	vector<_vec3>	m_ShufflePos;
	vector<_uint>	m_ShuffleSpeed;

	_float			m_fTimeDelta = 0.f;
	_bool			m_bFirst = true;
	_float			m_AnimationTime = 0.f;

	CLayer*			pMyLayer;
	_tchar*			m_RocketName;

	_float			m_fSpeed;

	list<_tchar*>	m_TcharList;

public:
	static CBattleCursier* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos, const _vec3 & vDir, _tchar* Name);
	virtual void		Free(void);
};

