#pragma once
#include "Supporter.h"
class CSupporter_Shotgun : public CSupporter
{
private:
	explicit CSupporter_Shotgun(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSupporter_Shotgun();

public:
	virtual HRESULT		Ready_Object(const _vec3& vPos, _tchar* Name);
	virtual _int		Update_Object(const _float& fTimeDelta);
	virtual void		LateUpdate_Object(void);
	virtual void		Render_Object(void);

	_bool				Get_setcam() { return m_bSetCam; }
	void				Set_setcam(_bool bcam) { m_bSetCam = bcam; }

private:
	HRESULT				Add_Component(void);
	void				Look_Direction(void);
	void				Find_Target(void);

private:	//	애니메이션 관련
	HRESULT				Build(void);
	void				Load_Animation(wstring FileName, _uint AnimationID);
	void				Run_Animation(const _float& AnimationSpeed);

	void				IDLE_Animation_Run(void);
	void				WALK_Animation_Run(void);
	void				ATTACK_Animation_Run(void);

	void				DROP_Animation_Run(void);

	_float				m_fTimeDelta = 0.f;
	_bool				m_bFirst = true;
	_float				m_AnimationTime = 0.f;

	CLayer*				pMyLayer;
	_tchar*				m_SupporterName;

	_bool				m_bOrdering = false;

	_vec3				m_vOrderPos = _vec3(0.f, 0.f, 0.f);

	_bool				m_bSetCam = true;

	SHOTGUNSUPPORTSTATEID	m_STATE;
	SHOTGUNSUPPORTIDLEID	m_IDLE;
	SHOTGUNSUPPORTWALKID	m_WALK;
	SHOTGUNSUPPORTATTACKID	m_ATTACK;

public:
	static CSupporter_Shotgun* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos, _tchar* Name);
	virtual void		Free(void);
};

