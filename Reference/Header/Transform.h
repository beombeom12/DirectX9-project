#pragma once
#include "Component.h"
#include "Engine_Include.h"

BEGIN(Engine)

class ENGINE_DLL CTransform : public CComponent
{
public:
	explicit CTransform(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTransform(const CTransform& rhs);
	virtual ~CTransform();

public:
	///////////////////////	불러오기를 통해 Ready 단계에서 World값을 지정해주기 위함 ////////////////////
	void				Set_WorldMatrix(_matrix* pWorld) { memcpy(m_matWorld, pWorld, sizeof(_matrix)); }
	void				Set_Info(INFOID eID, _vec3* pInfo) { memcpy(m_vInfo[eID], pInfo, sizeof(_vec3)); }
	void				Set_Angle(_vec3* pInfo) { memcpy(m_vAngle, pInfo, sizeof(_vec3)); }
	void				Set_Scale(_vec3* pInfo) { memcpy(m_vScale, pInfo, sizeof(_vec3)); }
	////////////////////////////////캐릭터회전///////////////////////////////////////////////////////////
	void				Rotation_Axis_X(const _float& fMovement, const _float& fAngle);
	void				Rotation_Axis_Y(const _float& fMovement, const _float& fAngle);
	void				Rotation_Revolution_Z(const _float& fAngle);
	void				Rotation_Axis_Animation(const _float& fXMove, const _float& fYMove, 
												const _float& fXAngle, const _float& fYAngle,
												const _float& fExtraMove = 0.f, const _float& fExtraAngle = 0.f);

	void				Rotation_Axis_Special(	const _float& fXMove, const _float& fYMove,
												const _float& fXAngle, const _float& fYAngle,
												const _float& fExtraMove = 0.f, const _float& fExtraAngle = 0.f);
	void				Rotation_Axis_Optional(	ROTATIONID eID_01, const _float& fMove_01, const _float& fAngle_01,
												ROTATIONID eID_02 = ROT_END, const _float& fMove_02 = 0.f, const _float& fAngle_02 = 0.f,
												ROTATIONID eID_03 = ROT_END, const _float& fMove_03 = 0.f, const _float& fAngle_03 = 0.f,
												ROTATIONID eID_04 = ROT_END, const _float& fMove_04 = 0.f, const _float& fAngle_04 = 0.f);

	// 안씀
	void				Rotation_Axis_Except_Scale(	_vec3* vScale,
													const _float& fXMove, const _float& fYMove,
													const _float& fXAngle, const _float& fYAngle,
													const _float& fExtraMove = 0.f, const _float& fExtraAngle = 0.f);
	void				Static_Update(void);
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	void				Rotation_Axis_Gun(	const _float & fXMove, const _float & fXAngle, 
											const _float & fYMove, const _float & fYAngle, 
											const _float & fZMove, const _float & fZAngle	 );
	void				Flexible_WorldSpace(_vec3* vScale, _vec3 * vAngle, _vec3* vTrans);
	void				Get_Scale(_vec3* vScale) { memcpy(vScale, m_vScale, sizeof(_vec3)); }
	void				Get_Angle(_vec3* vAngle) { memcpy(vAngle, m_vAngle, sizeof(_vec3)); }
	////피킹/////////////////////////////////////////////////////////////////////////////////////////////
	void				Get_WorldMatrix(_matrix* pWorld) const { *pWorld = m_matWorld; }
	//void				Set_WorldMatrix(_matrix* pWorld) { m_matWorld = *pWorld; }
	//트랜스폼 전 변환값/////////////////////////////////////////////////////////////////////////////////
	void				Get_BeforeInfo(INFOID eID, _vec3* pInfo) { memcpy(pInfo, m_vInfo[eID], sizeof(_vec3)); }
	////////////////////////////////////////
	void				Worldspace_By_Quarternion(void);
	D3DXQUATERNION		Quaternion;
	////////////////////////////////////////
	void				Quaternion_Transform(void);


	const _matrix*		                                                                                                                                     Get_WorldMatrixPointer()					 {	return &m_matWorld; }
	void				Move_Pos(const _vec3* const pDir)			 {	m_vInfo[INFO_POS] += *pDir; }
	void				Rotation(ROTATIONID eID, const _float& fAngle){	*(((_float*)&m_vAngle) + eID) += fAngle;	}
	void				Rotation_Fix(ROTATIONID eID, const _float& fAngle) { *(((_float*)&m_vAngle) + eID) = fAngle; }

	void				Get_Info(INFOID eID, _vec3* pInfo) {	memcpy(pInfo, &m_matWorld.m[eID][0], sizeof(_vec3));}
	void				Set_Scale(_float fX, _float fY, _float fZ){	m_vScale = { fX, fY, fZ }; }
	void				Set_Pos(const _float& fX, const _float& fY, const _float& fZ)
	{
		m_vInfo[INFO_POS].x = fX;
		m_vInfo[INFO_POS].y = fY;
		m_vInfo[INFO_POS].z = fZ;
	}

	void				Set_PosY(const _float& fY)
	{
		m_vInfo[INFO_POS].y = fY;
	}

	_float				GetBoundingSphereRadius() 
	{ 
		_vec3 vScaleX, vScaleY, vScaleZ;
		memcpy(&vScaleX, &m_matWorld.m[0][0], sizeof(_vec3));
		memcpy(&vScaleY, &m_matWorld.m[1][0], sizeof(_vec3));
		memcpy(&vScaleZ, &m_matWorld.m[2][0], sizeof(_vec3));

		_float fX = D3DXVec3Length(&vScaleX);
		_float fY = D3DXVec3Length(&vScaleY);
		_float fZ = D3DXVec3Length(&vScaleZ);
		return max(max(fX, fY),fZ); 
	}



	void				Set_Rotation(ROTATIONID eID, const _float& fAngle) { *(((_float*)&m_vAngle) + eID) = fAngle; }

public:
	void				Chase_Target(const _vec3* pTargetPos, const _float& fSpeed, const _float& fTimeDelta);
	const _matrix*		Compute_LookAtTarget(const _vec3* pTargetPos);
	
	void				Chase_Target_By_Direction(_vec3* vDirection, const _float& fSpeed, const _float& fTimeDelta);
	const _matrix*		LookAtTarget_By_Direction(_vec3* vDirection);


public:
	void				Billboard_Transform(const _float& fTimeDelta);

public:
	HRESULT				Ready_Transform(void);
	virtual _int		Update_Component(const _float& fTimeDelta);

public:
	_vec3			m_vInfo[INFO_END];
	_vec3			m_vScale;
	_vec3			m_vAngle;
	_matrix			m_matWorld;
	D3DXQUATERNION	m_Quaternion;

public:
	static CTransform*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent*	Clone(void);
private:
	virtual void Free(void);

};

END