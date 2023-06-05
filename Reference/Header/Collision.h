#pragma once
#include "Component.h"
#include "Engine_Include.h"

BEGIN(Engine);

class CHitBox;
class CCubeTex;
class CTransform;

class ENGINE_DLL CCollision : public CComponent
{
private:
	explicit CCollision(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCollision(const CCollision& rhs);
	virtual ~CCollision();

public:
	HRESULT		Ready_Collision(void);
	
	_bool		Sphere_Collision(CTransform* pTempTransform, CTransform* pSourTransform, _float fTemp, _float fSour);
	_bool		Animation_Collision(CTransform* pTempTransform, _matrix* AnimationFinalMatrix, _float fTemp, _float fAnimationScale);


	_int		Wall_Collision(_vec3* vNorm);
	_int		Wall_Collision_By_DotSliding(_vec3* vChangeDir);

	//	벽충돌 최종본
	void		Wall_Collision_Check(CTransform* pMonsterTrans, CHitBox* pMonsterHit, _vec3* vDir);

	_int		Wall_Collision_For_Monster(_vec3* vNorm, CTransform* pTransform, CHitBox* pHitBox);
	_int		Wall_Collision_By_DotSliding_For_Monster(_vec3* vChangeDir, CTransform* pTransform, CHitBox* pHitBox);

	_bool		Collision_Square(CTransform* pSrcTrans, CHitBox* pSrcHit, CTransform* pDstTrans, CHitBox* pDstHit);

	void		Get_Item(void);
	void		Get_GunItem();

	_bool		HitScan(HWND hWnd, _vec3 * SrcPos, const CCubeTex * pCubeTex, const CTransform * pTransform, _vec3* vReturn);

	_bool		Hit_In_ViewPort(HWND hWnd, const CCubeTex * pCubeTex, const CTransform * pTransform);

private:
	_vec3	m_vMin1 = { 0, 0, 0 };
	_vec3	m_vMin2 = { 0, 0, 0 };
	_vec3	m_vMax1 = { 0, 0, 0 };
	_vec3	m_vMax2 = { 0, 0, 0 };

	CHitBox*	m_pSrc = nullptr;
	CHitBox*	m_pDst = nullptr;

public:
	static CCollision* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent* Clone(void);
	virtual void Free(void);
};

END