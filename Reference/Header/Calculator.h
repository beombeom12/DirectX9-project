#pragma once
#include "Component.h"

BEGIN(Engine)

class CTerrainTex;
class CCubeTex;
class CTransform;

class ENGINE_DLL CCalculator : public CComponent
{
private:
	explicit CCalculator(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCalculator(const CCalculator& rhs);
	virtual ~CCalculator();

public:
	HRESULT		Ready_Calculator(void);

	_float		HeightOnTerrain(_vec3* pPos,
		const _vec3* pTerrainVtxPos,
		const _ulong& dwCntX,
		const _ulong& dwCntZ,
		const _ulong& dwVtxItv = 1);

	//	광선의 시작점을 정할 수 있도록 바꿈. 나중에 Ray충돌 구현할때 한번 더 업그레이드 할 예정
	_vec3		Peek_Target_Vector(HWND hWnd, _vec3* SrcPos, const CTerrainTex* pTerrainTex, const CTransform* pTransform);

	//	큐브 타겟 피킹
	_bool		Peek_Cube_Target(HWND hWnd, _vec3 * SrcPos, const CCubeTex * pCubeTex, const CTransform * pTransform);

	_vec3		Get_Mouse_World(HWND hWnd);

public:
	virtual CComponent* Clone(void) override;
	static CCalculator*	Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;
};

END