#pragma once
#include "GameObject.h"

class CNpc : public CGameObject
{
private:
	explicit CNpc(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CNpc();

public:
	HRESULT				Ready_Object(const _vec3& vPos);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		Render_Object(void) override;
	_bool				Get_QuestText1() { return m_bQuestText1; }
	_bool				Get_QuestText2() { return m_bQuestText2; }
	_bool				Get_QuestText3() { return m_bQuestText3; }

private:
	HRESULT				Add_Component(void);
	void				Quest1(const _float& fTimeDelta);
	void				Quest2(const _float& fTiemDelta);
	void				Quest3(const _float& fTiemDelta);
	void				Finish(const _float& fTimeDelta);
	void				Projection_Effect(void);
	void				Projection_Sound(void);

private:
	CRcTex*				m_pBufferCom = nullptr;
	CTransform*			m_pTransCom = nullptr;
	
	CTexture*			m_pQuest1TexCom = nullptr; // 52 장
	CTexture*			m_pQuest2TexCom = nullptr; // 60 장
	CTexture*			m_pQuest3TexCom = nullptr; // 36 장
	CTexture*			m_pQuest4TexCom = nullptr; // 62 장

	CShader*			m_pShaderCom = nullptr;

	_vec3				m_vScale = _vec3(10.f, 5.f, 1.f);
	_vec3				m_vDir = _vec3(0.f, 0.f, 0.f);
	_vec3				m_vPlayerPos = _vec3(0.f, 0.f, 0.f);
	_float				m_fSpeed = 10.f;
	_bool				m_bInit = false;

	_float				m_fTexFrame = 0.f;
	_float				m_fTransFrame0 = 0.f;

	_float				m_fScaleFrame0 = 0.f;
	_float				m_fScaleFrame1 = 0.f;

	_bool				m_bQuest1 = true;
	_bool				m_bQuest2 = true;
	_bool				m_bQuest3 = true;
	_bool				m_bQuest4 = true;

	_bool				m_bQuestText1 = false;
	_bool				m_bQuestText2 = false;
	_bool				m_bQuestText3 = false;
	_bool				m_bQuestText4 = false;

	_bool				m_bBossDead = false;

	_bool				m_bEffect1 = false;
	_bool				m_bEffect2 = false;
	_bool				m_bEffect3 = false;
	_bool				m_bEffect4 = false;

	_bool				m_bSound1 = false;
	_bool				m_bSound2 = false;
	_bool				m_bSound3 = false;


public:
	static	CNpc*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos);

protected:
	virtual void Free();
};

