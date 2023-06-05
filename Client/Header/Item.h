#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CTexture;
class CSphereTex;
class CTransform;
class CCalculator;

END

class CItem : public CGameObject
{
protected:
	explicit CItem(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CItem(const CGameObject& rhs);
	virtual ~CItem();

public:
	virtual HRESULT Ready_Object(const _vec3& vPos);
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object() override;
	virtual void	Render_Object()override;

	virtual void    Move_Item(const _float& fTimeDelta);
	virtual _vec3	Get_ItemPos(void) { return m_vItemPos;}

protected:
	CDynamicBuffer* m_pDBufferCom = nullptr;
	CTexture*	    m_pTextureCom = nullptr;
	CTransform*	    m_pTransCom = nullptr;
	CSphereTex*		m_pBufferCom = nullptr;
	CCalculator*	m_pCalculatorCom = nullptr;
	
	CHitBox*	 m_pHitBox = nullptr;
	CTransform*  m_pHitBoxTransCom = nullptr;

	_float		 m_fRotSpeed = -180.f;
	_float		 m_fYSpeed = 0.f;
	_int		 m_iReverseDir = 1;
	_float		 m_fFrame = 0.f;
	_vec3		 m_vItemPos = { 0.f,0.f,0.f};


protected:
	virtual void Free()override;
};