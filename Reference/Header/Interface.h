#pragma once

#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CInterface : public CGameObject
{
protected:
	explicit CInterface(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CInterface();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;


	// 뷰, 투영 행렬 반환 함수
	_matrix GetView() const { return m_matView; }
	_matrix GetProj() const { return m_matProj; }

protected:
	_vec3		m_vEye, m_vAt, m_vUp;
	_float		m_fFov, m_fAspect, m_fNear, m_fFar;
	_matrix		m_matView, m_matProj;

protected:
	virtual void		Free(void);
};

END