#pragma once

#include "GameObject.h"
#include "FrustumCullMgr.h"

BEGIN(Engine)

class ENGINE_DLL CCamera : public CGameObject
{
protected:
	explicit CCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCamera();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;

	// 뷰, 투영 행렬 반환 함수
	_matrix GetView() const { return m_matView; }
	_matrix GetProj() const { return m_matProj; }

	_vec3	GetEye() const { return m_vEye; }
	_vec3	GetAt() const { return m_vAt; }
	_vec3	GetUp() const { return m_vUp; }

	void Get_Position(_vec3* vPos) { *vPos = m_vEye; }
	void Get_Look(_vec3* vlook) { *vlook = m_vAt - m_vEye; }
	
protected:
	_vec3		m_vEye, m_vAt, m_vUp;
	_float		m_fFov, m_fAspect, m_fNear, m_fFar;
	_matrix		m_matView, m_matProj;

protected:
	virtual void		Free(void);
};

END