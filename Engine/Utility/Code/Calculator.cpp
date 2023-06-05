#include "..\..\Header\Calculator.h"
#include "Export_Function.h"

USING(Engine)

CCalculator::CCalculator(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)
{

}

Engine::CCalculator::CCalculator(const CCalculator& rhs)
	: CComponent(rhs)
{

}

CCalculator::~CCalculator()
{
}

HRESULT CCalculator::Ready_Calculator(void)
{
	return S_OK;
}

Engine::_float CCalculator::HeightOnTerrain(_vec3* pPos, const _vec3* pTerrainVtxPos,
	const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv)
{
	_ulong	dwIndex = _ulong(pPos->z / dwVtxItv) * dwCntX + _ulong(pPos->x / dwVtxItv);
	// dwindex= 플레이어 포지션x * 129 + 플레이어포지션z* 129
	_float	fWidth = (pPos->x - pTerrainVtxPos[dwIndex + dwCntX].x) / dwVtxItv;
	// 플레이어 포지션x - 터레인버텍스포지션[dwIndex+129]의 X
	_float	fHeight = (pTerrainVtxPos[dwIndex + dwCntX].z - pPos->z) / dwVtxItv;
	// 터레인버텍스포지션[dwIndex+129] - 플레이어 포지션z
	D3DXPLANE		Plane;

	// 오른쪽 위 삼각형
	if (fWidth > fHeight)
	{
		D3DXPlaneFromPoints(&Plane,
			&pTerrainVtxPos[dwIndex + dwCntX],
			&pTerrainVtxPos[dwIndex + dwCntX + 1],
			&pTerrainVtxPos[dwIndex + 1]);

	}
	// 왼쪽 아래 삼각형
	else
	{
		D3DXPlaneFromPoints(&Plane,
			&pTerrainVtxPos[dwIndex + dwCntX],
			&pTerrainVtxPos[dwIndex + 1],
			&pTerrainVtxPos[dwIndex]);
	}

	pPos->y = (-Plane.a * pPos->x - Plane.c * pPos->z - Plane.d) / Plane.b;
	
	return pPos->y;
}

_vec3 CCalculator::Peek_Target_Vector(HWND hWnd, _vec3 * SrcPos, const CTerrainTex * pTerrainTex, const CTransform * pTransform)
{
	POINT	WindowPt{};
	GetCursorPos(&WindowPt);
	ScreenToClient(hWnd, &WindowPt);

	_vec3	vPoint{};

	D3DVIEWPORT9	Viewport;
	ZeroMemory(&Viewport, sizeof(D3DVIEWPORT9));
	m_pGraphicDev->GetViewport(&Viewport);

	vPoint.x = WindowPt.x / (Viewport.Width * 0.5f) - 1.f;
	vPoint.y = WindowPt.y / -(Viewport.Height * 0.5f) + 1.f;
	vPoint.z = 0.f;

	_matrix			matProj;
	D3DXMatrixIdentity(&matProj);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, nullptr, &matProj);
	D3DXVec3TransformCoord(&vPoint, &vPoint, &matProj);

	_vec3 vRayPos = *SrcPos;
	_vec3 vRayDir = vPoint - vRayPos;

	_matrix			matView;
	D3DXMatrixIdentity(&matView);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, nullptr, &matView);
	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);

	_matrix			matWorld;
	D3DXMatrixIdentity(&matWorld);
	pTransform->Get_WorldMatrix(&matWorld);
	D3DXMatrixInverse(&matWorld, nullptr, &matWorld);
	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matWorld);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matWorld);

	const _vec3*	Vertex = pTerrainTex->Get_VtxPos();
	_ulong			dwVtxIndex[3]{};
	_float			fU, fV, fDist;

	for (_ulong dwIndex = 0; dwIndex < (VTXCNTX - 1) * (VTXCNTZ - 1) * 2; ++dwIndex)
	{
		//	우상단 삼각형
		dwVtxIndex[0] = dwIndex + VTXCNTX;
		dwVtxIndex[1] = dwIndex + VTXCNTX + 1;
		dwVtxIndex[2] = dwIndex + 1;

		if (D3DXIntersectTri
			(&Vertex[dwVtxIndex[1]],
				&Vertex[dwVtxIndex[0]],
				&Vertex[dwVtxIndex[2]],
				&vRayPos, &vRayDir,
				&fU, &fV, &fDist))
		{
			return _vec3(Vertex[dwVtxIndex[1]].x + (Vertex[dwVtxIndex[0]].x - Vertex[dwVtxIndex[1]].x) * fU,
				0.f,
				Vertex[dwVtxIndex[1]].z + (Vertex[dwVtxIndex[2]].z - Vertex[dwVtxIndex[1]].z) * fV);
		}

		// 좌하단 삼각형
		dwVtxIndex[0] = dwIndex + VTXCNTX;
		dwVtxIndex[1] = dwIndex + 1;
		dwVtxIndex[2] = dwIndex;

		if (D3DXIntersectTri
			(&Vertex[dwVtxIndex[2]],
				&Vertex[dwVtxIndex[1]],
				&Vertex[dwVtxIndex[0]],
				&vRayPos, &vRayDir,
				&fU, &fV, &fDist))
		{
			return _vec3(Vertex[dwVtxIndex[2]].x + (Vertex[dwVtxIndex[1]].x - Vertex[dwVtxIndex[2]].x) * fU,
				0.f,
				Vertex[dwVtxIndex[2]].z + (Vertex[dwVtxIndex[0]].z - Vertex[dwVtxIndex[2]].z) * fV);
		}
	}

	return _vec3(0, 0, 0);
}

// 큐브 피킹을 위한 함수
_bool CCalculator::Peek_Cube_Target(HWND hWnd, _vec3 * SrcPos, const CCubeTex * pCubeTex, const CTransform * pTransform)
{
	POINT		ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(hWnd, &ptMouse);

	_vec3		vPoint;

	m_pGraphicDev;

	D3DVIEWPORT9		ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
	m_pGraphicDev->GetViewport(&ViewPort);

	// 뷰포트 -> 투영
	vPoint.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	vPoint.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;
	vPoint.z = 0.f;

	// 투영 -> 뷰 스페이스
	_matrix		matProj;

	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, nullptr, &matProj);
	D3DXVec3TransformCoord(&vPoint, &vPoint, &matProj);

	_vec3	vRayDir, vRayPos;		// 뷰 스페이스 영역에 있는 상태

	vRayPos = *SrcPos;
	vRayDir = vPoint - vRayPos;

	// 뷰 스페이스 -> 월드

	_matrix		matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, nullptr, &matView);
	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);

	// 월드 -> 로컬
	_matrix		matWorld;

	pTransform->Get_WorldMatrix(&matWorld);
	D3DXMatrixInverse(&matWorld, nullptr, &matWorld);
	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matWorld);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matWorld);

	const _vec3*	pCubtVtx = pCubeTex->Get_VtxPos();

	_ulong	dwVtxIdx[3]{};
	_float	fU, fV, fDist;
	// +X
	dwVtxIdx[0] = 1;
	dwVtxIdx[1] = 5;
	dwVtxIdx[2] = 6;
	if (D3DXIntersectTri(&pCubtVtx[dwVtxIdx[1]],
		&pCubtVtx[dwVtxIdx[0]],
		&pCubtVtx[dwVtxIdx[2]],
		&vRayPos, &vRayDir,
		&fU, &fV, &fDist))
	{
		return true;
	}
	dwVtxIdx[0] = 1;
	dwVtxIdx[1] = 6;
	dwVtxIdx[2] = 2;
	if (D3DXIntersectTri(&pCubtVtx[dwVtxIdx[1]],
		&pCubtVtx[dwVtxIdx[0]],
		&pCubtVtx[dwVtxIdx[2]],
		&vRayPos, &vRayDir,
		&fU, &fV, &fDist))
	{
		return true;
	}
	// ~+X

	// -X
	dwVtxIdx[0] = 4;
	dwVtxIdx[1] = 0;
	dwVtxIdx[2] = 3;
	if (D3DXIntersectTri(&pCubtVtx[dwVtxIdx[1]],
		&pCubtVtx[dwVtxIdx[0]],
		&pCubtVtx[dwVtxIdx[2]],
		&vRayPos, &vRayDir,
		&fU, &fV, &fDist))
	{
		return true;
	}
	dwVtxIdx[0] = 4;
	dwVtxIdx[1] = 3;
	dwVtxIdx[2] = 7;
	if (D3DXIntersectTri(&pCubtVtx[dwVtxIdx[1]],
		&pCubtVtx[dwVtxIdx[0]],
		&pCubtVtx[dwVtxIdx[2]],
		&vRayPos, &vRayDir,
		&fU, &fV, &fDist))
	{
		return true;
	}
	// ~-X

	// Y
	dwVtxIdx[0] = 4;
	dwVtxIdx[1] = 5;
	dwVtxIdx[2] = 1;

	if (D3DXIntersectTri(&pCubtVtx[dwVtxIdx[1]],
		&pCubtVtx[dwVtxIdx[0]],
		&pCubtVtx[dwVtxIdx[2]],
		&vRayPos, &vRayDir,
		&fU, &fV, &fDist))
	{
		return true;
	}

	dwVtxIdx[0] = 4;
	dwVtxIdx[1] = 1;
	dwVtxIdx[2] = 0;

	if (D3DXIntersectTri(&pCubtVtx[dwVtxIdx[1]],
		&pCubtVtx[dwVtxIdx[0]],
		&pCubtVtx[dwVtxIdx[2]],
		&vRayPos, &vRayDir,
		&fU, &fV, &fDist))
	{
		return true;
	}
	// ~ +Y

	// ~ -Y
	dwVtxIdx[0] = 3;
	dwVtxIdx[1] = 2;
	dwVtxIdx[2] = 6;
	if (D3DXIntersectTri(&pCubtVtx[dwVtxIdx[1]],
		&pCubtVtx[dwVtxIdx[0]],
		&pCubtVtx[dwVtxIdx[2]],
		&vRayPos, &vRayDir,
		&fU, &fV, &fDist))
	{
		return true;
	}

	dwVtxIdx[0] = 3;
	dwVtxIdx[1] = 6;
	dwVtxIdx[2] = 7;
	if (D3DXIntersectTri(&pCubtVtx[dwVtxIdx[1]],
		&pCubtVtx[dwVtxIdx[0]],
		&pCubtVtx[dwVtxIdx[2]],
		&vRayPos, &vRayDir,
		&fU, &fV, &fDist))
	{
		return true;
	}
	// ~ -Y

	// Z+
	dwVtxIdx[0] = 7;
	dwVtxIdx[1] = 6;
	dwVtxIdx[2] = 5;
	if (D3DXIntersectTri(&pCubtVtx[dwVtxIdx[1]],
		&pCubtVtx[dwVtxIdx[0]],
		&pCubtVtx[dwVtxIdx[2]],
		&vRayPos, &vRayDir,
		&fU, &fV, &fDist))
	{
		return true;
	}

	dwVtxIdx[0] = 7;
	dwVtxIdx[1] = 5;
	dwVtxIdx[2] = 4;
	if (D3DXIntersectTri(&pCubtVtx[dwVtxIdx[1]],
		&pCubtVtx[dwVtxIdx[0]],
		&pCubtVtx[dwVtxIdx[2]],
		&vRayPos, &vRayDir,
		&fU, &fV, &fDist))
	{
		return true;
	}

	// ~Z+

	// Z-
	dwVtxIdx[0] = 0;
	dwVtxIdx[1] = 1;
	dwVtxIdx[2] = 2;
	if (D3DXIntersectTri(&pCubtVtx[dwVtxIdx[1]],
		&pCubtVtx[dwVtxIdx[0]],
		&pCubtVtx[dwVtxIdx[2]],
		&vRayPos, &vRayDir,
		&fU, &fV, &fDist))
	{
		return true;
	}

	dwVtxIdx[0] = 0;
	dwVtxIdx[1] = 2;
	dwVtxIdx[2] = 3;
	if (D3DXIntersectTri(&pCubtVtx[dwVtxIdx[1]],
		&pCubtVtx[dwVtxIdx[0]],
		&pCubtVtx[dwVtxIdx[2]],
		&vRayPos, &vRayDir,
		&fU, &fV, &fDist))
	{
		return true;
	}

	// ~Z-
	return false;
}

_vec3 CCalculator::Get_Mouse_World(HWND hWnd)
{
	POINT		ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(hWnd, &ptMouse);

	_vec3		vPoint;

	D3DVIEWPORT9		ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
	m_pGraphicDev->GetViewport(&ViewPort);

	vPoint.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	vPoint.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;
	vPoint.z = 1.f;

	_matrix		matProj;

	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, nullptr, &matProj);
	D3DXVec3TransformCoord(&vPoint, &vPoint, &matProj);

	_matrix		matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, nullptr, &matView);
	D3DXVec3TransformCoord(&vPoint, &vPoint, &matView);

	return vPoint;
}


CComponent* CCalculator::Clone(void)
{
	return new CCalculator(*this);
}

CCalculator* CCalculator::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCalculator*	pInstance = new CCalculator(pGraphicDev);

	if (FAILED(pInstance->Ready_Calculator()))
	{
		MSG_BOX("Calculator Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CCalculator::Free(void)
{
	CComponent::Free();
}
