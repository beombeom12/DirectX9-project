#include "..\Header\FrustumCullMgr.h"

USING(Engine)

IMPLEMENT_SINGLETON(CFrustumCullMgr)

CFrustumCullMgr::CFrustumCullMgr()
{
}

CFrustumCullMgr::~CFrustumCullMgr()
{
}

HRESULT CFrustumCullMgr::Update(const _matrix& matView, const _matrix& matProj)
{
	_matrix matViewInv;
	D3DXMatrixInverse(&matViewInv, nullptr, &matView);
	_matrix matProjInv;
	D3DXMatrixInverse(&matProjInv, nullptr, &matProj);
	_matrix matInv;
	matInv = matProjInv * matViewInv;

	vector<_vec3> vWorldPos =
	{
		_vec3(-1.f,1.f,0.f),
		_vec3(1.f,1.f,0.f),
		_vec3(1.f,-1.f,0.f),
		_vec3(-1.f,-1.f,0.f),
		_vec3(-1.f,1.f,1.f),
		_vec3(1.f,1.f,1.f),
		_vec3(1.f,-1.f,1.f),
		_vec3(-1.f,-1.f,1.f)
	};

	// w = 1·Î ¼¼ÆÃ
	for (size_t i = 0; i < vWorldPos.size(); i++)
	{
		D3DXVec3TransformCoord(&vWorldPos[i], &vWorldPos[i], &matInv);
	}

	 D3DXPlaneFromPoints(&m_Planes[PLANE_FRONT],&vWorldPos[0],&vWorldPos[1],&vWorldPos[2]);
	 D3DXPlaneFromPoints(&m_Planes[PLANE_BACK], &vWorldPos[4], &vWorldPos[7], &vWorldPos[5]);
	 D3DXPlaneFromPoints(&m_Planes[PLANE_UP], &vWorldPos[4], &vWorldPos[5], &vWorldPos[1]);
	 D3DXPlaneFromPoints(&m_Planes[PLANE_DOWN], &vWorldPos[7], &vWorldPos[3], &vWorldPos[6]);
	 D3DXPlaneFromPoints(&m_Planes[PLANE_LEFT], &vWorldPos[4], &vWorldPos[0], &vWorldPos[7]);
	 D3DXPlaneFromPoints(&m_Planes[PLANE_RIGHT], &vWorldPos[5], &vWorldPos[6], &vWorldPos[1]);

	return S_OK;
}

_bool CFrustumCullMgr::ContainsSphere(const _vec3 & vPos, _float radius)
{
	for (const D3DXPLANE& plane : m_Planes)
	{
		_vec3 vNormal = _vec3(plane.a, plane.b, plane.c);

		// ax + by + cz + d > radius
		if (D3DXVec3Dot(&vNormal, &vPos) + plane.d > radius)
			return false;
	}

	return true;
}
