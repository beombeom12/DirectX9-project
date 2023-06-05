#pragma once
#include "Trap.h"
class CItemBox : public CTrap
{
private:
	explicit CItemBox(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CItemBox();

public:
	virtual HRESULT		Ready_Object(const _vec3& vPos, _tchar* Name);
	virtual _int		Update_Object(const _float& fTimeDelta);
	virtual void		LateUpdate_Object(void);
	virtual void		Render_Object(void);
	_bool				Get_QuestOpen() { return m_bQuestOpen; }

private:
	HRESULT				Add_Component(void);

	CTransform*			m_pPlayerTransCom = nullptr;

private:	//	애니메이션 관련
	HRESULT				Build(void);
	void				Load_Animation(wstring FileName, _uint AnimationID);
	void				Run_Animation(const _float& AnimationSpeed);

	void				Up_Animation_Run(void);
	void				Down_Animation_Run(void);

	_float				m_fTimeDelta = 0.f;
	_bool				m_bFirst = true;
	_float				m_AnimationTime = 0.f;
	_bool				m_bStateChange = false;

	ITEMBOXSTATEID		m_STATE;

	_bool				m_bQuestOpen = false;
	CLayer*				pMyLayer;
	_tchar*				m_TrapName;

public:
	static CItemBox*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos, _tchar* Name);
	virtual void		Free(void)override;
};

