#pragma once
#include "GameObject.h"
#include "ComboUI.h"
#include "MonsterUI.h"
class CTrap : public CGameObject
{
protected:
	explicit CTrap(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTrap();

protected:
	virtual HRESULT		Ready_Object(const _vec3& vPos, _tchar* Name);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

protected:
	CCollision*			m_pCollision = nullptr;
	CHitBox*			m_pHitBox = nullptr;
	CTransform*			m_pTransCom = nullptr;
	CTransform*			m_pTransUICom = nullptr;

	CTransform*			m_pSearchRange_TransCom = nullptr;	
	CTransform*			m_pAttackRange_TransCom = nullptr;	
	CTransform*			m_pSphereTransCom = nullptr;
	CTransform*			m_pPlayerTransCom = nullptr;
	CTransform*			m_pHitBoxTransCom = nullptr;
	CSphereTex*			m_pSphereBufferCom = nullptr;
	CComboUI*			m_pComboUI = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CCubeTex*			m_pBufferCom = nullptr;
	CRcTex*				m_pBufferUICom = nullptr;
	CMonsterUI*			m_pMonsterUI = nullptr;
	CTexture*			m_pTextureUICom = nullptr;
	CCubeCol*			m_pAnimationBox = nullptr;
protected:
	list<_tchar*>		m_TcharList;

protected:
	virtual void		Free(void)override;
};

