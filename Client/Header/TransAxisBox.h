#pragma once
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CCubeTex;
	class CTexture;
	class CCalculator;
	class CQuarternion;
	class CFlexibleAxisCube;
}

class CTransAxisBox : public CGameObject
{
private:
	explicit CTransAxisBox(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTransAxisBox();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual void	Render_Object(void) override;
	virtual HRESULT	Set_Material()override;

public:
	bool			Set_SelectGizmo(void);
	CTransform*		Get_Transform(void) { return m_pTransCom; }
	CTransform*		Get_Parent(void) { return m_pParentTransform; }
	void			Set_Parent(CTransform* pTransform) { m_pParentTransform = pTransform; }
	void			ParentSettingStart(void) { bParentSetting = true; }
	void			Get_Final(_matrix* mat) { *mat = m_FinalWorldMatrix; }

public:
	void			Set_Axis(_float& fX, _float fY, _float fZ) { m_fX = fX, m_fY = fY, m_fZ = fZ; }
	_float			Get_X(void) { return m_fX; }
	_float			Get_Y(void) { return m_fY; }
	_float			Get_Z(void) { return m_fZ; }

	_float				GetBoundingSphereRadius(const _matrix& matWorld)
	{
		_vec3 vScaleX, vScaleY, vScaleZ;
		memcpy(&vScaleX, &matWorld.m[0][0], sizeof(_vec3));
		memcpy(&vScaleY, &matWorld.m[1][0], sizeof(_vec3));
		memcpy(&vScaleZ, &matWorld.m[2][0], sizeof(_vec3));

		_float fX = D3DXVec3Length(&vScaleX);
		_float fY = D3DXVec3Length(&vScaleY);
		_float fZ = D3DXVec3Length(&vScaleZ);
		return max(max(fX, fY), fZ);
	}

public:
	list<CTransAxisBox*>	m_ParentList;
	list<_tchar*>			m_ParentKey;
	list<_tchar*>			m_ChildKey;
	_bool					m_bIsRoot;

private:
	HRESULT				Add_Component(void);
	CFlexibleAxisCube*	m_pBufferCom = nullptr;
	CTransform*			m_pTransCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CCalculator*		m_pCalculatorCom = nullptr;

	CTransform*			m_pParentTransform = nullptr;

	CQuarternion*		m_pQuaternion = nullptr;

private:
	_matrix				m_FinalWorldMatrix;

	_matrix				m_AxisMoveMatrix;		//	축 이전용, 월드 적용 전에 해당 이동부터 해주자
	void				AxisMatrixCreate(void);

	_float				m_fX, m_fY, m_fZ;

	_bool				bParentSetting;

public:
	static CTransAxisBox* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void Free(void);
};

