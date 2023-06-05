#include "..\Header\Collision.h"
#include "Export_Function.h"

#include "../../Client/Header/Uzi.h"
#include "../../Client/Header/Item.h"
#include "../../Client/Default/Define.h"

#include "../../Client/Header/Flight.h"

USING(Engine)

CCollision::CCollision(LPDIRECT3DDEVICE9 pGraphicDev)
	:CComponent(pGraphicDev)
{
}

CCollision::CCollision(const CCollision & rhs)
	: CComponent(rhs)
{
}

CCollision::~CCollision()
{
}

HRESULT CCollision::Ready_Collision(void)
{
	return S_OK;
}

_bool CCollision::Sphere_Collision(CTransform* pTempTransform, CTransform* pSourTransform, _float fTemp, _float fSour)
{
	_vec3 vTemp, vSour;

	pTempTransform->Get_Info(INFO_POS, &vTemp);
	pSourTransform->Get_Info(INFO_POS, &vSour);

	_float fTempX, fTempY, fTempZ;
	_float fSourX, fSourY, fSourZ;

	fTempX = vTemp.x;
	fTempY = vTemp.y;
	fTempZ = vTemp.z;

	fSourX = vSour.x;
	fSourY = vSour.y;
	fSourZ = vSour.z;

	_float fDistance = sqrtf((fTempX - fSourX) * (fTempX - fSourX) + (fTempY - fSourY) * (fTempY - fSourY) + (fTempZ - fSourZ) * (fTempZ - fSourZ));

	if (fDistance > fTemp + fSour)
		return false;
	else
		return true;
}

_bool CCollision::Animation_Collision(CTransform * pTempTransform, _matrix * AnimationFinalMatrix, _float fTemp, _float fAnimationScale)
{
	_vec3 vTemp, vSour;

	pTempTransform->Get_Info(INFO_POS, &vTemp);
	vSour = { AnimationFinalMatrix->m[3][0], AnimationFinalMatrix->m[3][1] ,AnimationFinalMatrix->m[3][2] };

	_float fTempX, fTempY, fTempZ;
	_float fSourX, fSourY, fSourZ;

	fTempX = vTemp.x;
	fTempY = vTemp.y;
	fTempZ = vTemp.z;

	fSourX = vSour.x;
	fSourY = vSour.y;
	fSourZ = vSour.z;

	_float fDistance = sqrtf((fTempX - fSourX) * (fTempX - fSourX) + (fTempY - fSourY) * (fTempY - fSourY) + (fTempZ - fSourZ) * (fTempZ - fSourZ));

	if (fDistance > fTemp + fAnimationScale)
		return false;
	else
		return true;
}

_int CCollision::Wall_Collision(_vec3* vNorm)
{
	CLayer* pLayer = Engine::Get_Layer(STAGE_WALL);
	map<const _tchar*, CGameObject*> mapWall = pLayer->Get_GameObjectMap();

	pLayer = Engine::Get_Layer(STAGE_DESTORYWALL);
	list<CGameObject*> DestoryWallList = pLayer->Get_GameList();

	CTransform*	pPlayerTransform = dynamic_cast<CTransform*>(Get_Component(STAGE_CHARACTER, L"PLAYER", TRANSFORM_COMP, ID_DYNAMIC));
	CHitBox* pPlayerBox = dynamic_cast<CHitBox*>(Get_Component(STAGE_CHARACTER, L"PLAYER", HITBOX_COMP, ID_STATIC));

	CTransform* pPlayerBodyTransform = dynamic_cast<CTransform*>(Get_Component(STAGE_CHARACTER, L"BODY", TRANSFORM_COMP, ID_DYNAMIC));

	_vec3 vPlayerPos;
	pPlayerTransform->Get_Info(INFO_POS, &vPlayerPos);

	for (auto iter : mapWall)
	{
		CTransform* pWallTransform = dynamic_cast<CTransform*>(iter.second->Get_Component(TRANSFORM_COMP, ID_DYNAMIC));
		NULL_CHECK_RETURN(pWallTransform, false);
		CHitBox* pWallBox = dynamic_cast<CHitBox*>(iter.second->Get_Component(HITBOX_COMP, ID_STATIC));
		NULL_CHECK_RETURN(pWallBox, false);

		pPlayerBox->Get_MinMax(&m_vMin1, &m_vMax1);
		pWallBox->Get_MinMax(&m_vMin2, &m_vMax2);

		D3DXVec3TransformCoord(&m_vMin1, &m_vMin1, pPlayerTransform->Get_WorldMatrixPointer());
		D3DXVec3TransformCoord(&m_vMax1, &m_vMax1, pPlayerTransform->Get_WorldMatrixPointer());
		D3DXVec3TransformCoord(&m_vMin2, &m_vMin2, pWallTransform->Get_WorldMatrixPointer());
		D3DXVec3TransformCoord(&m_vMax2, &m_vMax2, pWallTransform->Get_WorldMatrixPointer());

		int iCollisionCnt = 0;

		if (m_vMin1.x <= m_vMax2.x && m_vMax1.x >= m_vMin2.x &&
			m_vMin1.y <= m_vMax2.y && m_vMax1.y >= m_vMin2.y &&
			m_vMin1.z <= m_vMax2.z && m_vMax1.z >= m_vMin2.z)
		{
			_vec3 vWallPos, vWallRight, vWallUp, vWallLook;
			pWallTransform->Get_Info(INFO_POS, &vWallPos);

			const _vec3*	pCubtVtx = pWallBox->Get_VtxPos();
			_vec3	vNormal(0,0,0);
			_ulong	dwVtxIdx[3]{};

			if ((vPlayerPos.x > vWallPos.x) && (m_vMin1.z > m_vMin2.z) && (m_vMax1.z < m_vMax2.z))
			{
				// 벽의 우측에 충돌
				dwVtxIdx[0] = 1;
				dwVtxIdx[1] = 5;
				dwVtxIdx[2] = 6;
				_vec3 vSrc = pCubtVtx[5] - pCubtVtx[6];
				_vec3 vDst = pCubtVtx[5] - pCubtVtx[1];
				D3DXVec3Cross(&vNormal, &vSrc, &vDst);
				D3DXVec3Normalize(&vNormal, &vNormal);

				memcpy(vNorm, vNormal, sizeof(_vec3));

				return WALL_RIGHT;
			}
			if ((vPlayerPos.x < vWallPos.x) && (m_vMin1.z > m_vMin2.z) && (m_vMax1.z < m_vMax2.z))
			{
				// 벽의 좌측에 충돌
				dwVtxIdx[0] = 4;
				dwVtxIdx[1] = 0;
				dwVtxIdx[2] = 3;
				_vec3 vSrc = pCubtVtx[3] - pCubtVtx[0];
				_vec3 vDst = pCubtVtx[4] - pCubtVtx[0];
				D3DXVec3Cross(&vNormal, &vSrc, &vDst);
				D3DXVec3Normalize(&vNormal, &vNormal);

				memcpy(vNorm, vNormal, sizeof(_vec3));

				return WALL_LEFT;
			}
			if ((vPlayerPos.z > vWallPos.z) && (m_vMin1.x > m_vMin2.x) && (m_vMax1.x < m_vMax2.x))
			{
				// 벽의 전측에 충돌
				dwVtxIdx[0] = 7;
				dwVtxIdx[1] = 6;
				dwVtxIdx[2] = 5;
				_vec3 vSrc = pCubtVtx[6] - pCubtVtx[7];
				_vec3 vDst = pCubtVtx[6] - pCubtVtx[5];
				D3DXVec3Cross(&vNormal, &vSrc, &vDst);
				D3DXVec3Normalize(&vNormal, &vNormal);

				memcpy(vNorm, vNormal, sizeof(_vec3));

				return WALL_FRONT;
			}
			if ((vPlayerPos.z < vWallPos.z) && (m_vMin1.x > m_vMin2.x) && (m_vMax1.x < m_vMax2.x))
			{
				// 벽의 후측에 충돌
				dwVtxIdx[0] = 0;
				dwVtxIdx[1] = 1;
				dwVtxIdx[2] = 2;
				_vec3 vSrc = pCubtVtx[1] - pCubtVtx[0];
				_vec3 vDst = pCubtVtx[2] - pCubtVtx[1];
				D3DXVec3Cross(&vNormal, &vSrc, &vDst);
				D3DXVec3Normalize(&vNormal, &vNormal);

				memcpy(vNorm, vNormal, sizeof(_vec3));

				return WALL_BACK;
			}
			return -1;
		}
	}

	if (!DestoryWallList.empty())
	{
		for (auto iter : DestoryWallList)
		{
			CTransform* pWallTransform = dynamic_cast<CTransform*>(iter->Get_Component(TRANSFORM_COMP, ID_DYNAMIC));
			NULL_CHECK_RETURN(pWallTransform, false);
			CHitBox* pWallBox = dynamic_cast<CHitBox*>(iter->Get_Component(HITBOX_COMP, ID_STATIC));
			NULL_CHECK_RETURN(pWallBox, false);

			pPlayerBox->Get_MinMax(&m_vMin1, &m_vMax1);
			pWallBox->Get_MinMax(&m_vMin2, &m_vMax2);

			D3DXVec3TransformCoord(&m_vMin1, &m_vMin1, pPlayerTransform->Get_WorldMatrixPointer());
			D3DXVec3TransformCoord(&m_vMax1, &m_vMax1, pPlayerTransform->Get_WorldMatrixPointer());
			D3DXVec3TransformCoord(&m_vMin2, &m_vMin2, pWallTransform->Get_WorldMatrixPointer());
			D3DXVec3TransformCoord(&m_vMax2, &m_vMax2, pWallTransform->Get_WorldMatrixPointer());

			int iCollisionCnt = 0;

			if (m_vMin1.x <= m_vMax2.x && m_vMax1.x >= m_vMin2.x &&
				m_vMin1.y <= m_vMax2.y && m_vMax1.y >= m_vMin2.y &&
				m_vMin1.z <= m_vMax2.z && m_vMax1.z >= m_vMin2.z)
			{
				_vec3 vWallPos, vWallRight, vWallUp, vWallLook;
				pWallTransform->Get_Info(INFO_POS, &vWallPos);

				const _vec3*	pCubtVtx = pWallBox->Get_VtxPos();
				_vec3	vNormal(0, 0, 0);
				_ulong	dwVtxIdx[3]{};

				if ((vPlayerPos.x > vWallPos.x) && (m_vMin1.z > m_vMin2.z) && (m_vMax1.z < m_vMax2.z))
				{
					// 벽의 우측에 충돌
					dwVtxIdx[0] = 1;
					dwVtxIdx[1] = 5;
					dwVtxIdx[2] = 6;
					_vec3 vSrc = pCubtVtx[5] - pCubtVtx[6];
					_vec3 vDst = pCubtVtx[5] - pCubtVtx[1];
					D3DXVec3Cross(&vNormal, &vSrc, &vDst);
					D3DXVec3Normalize(&vNormal, &vNormal);

					memcpy(vNorm, vNormal, sizeof(_vec3));

					return WALL_RIGHT;
				}
				if ((vPlayerPos.x < vWallPos.x) && (m_vMin1.z > m_vMin2.z) && (m_vMax1.z < m_vMax2.z))
				{
					// 벽의 좌측에 충돌
					dwVtxIdx[0] = 4;
					dwVtxIdx[1] = 0;
					dwVtxIdx[2] = 3;
					_vec3 vSrc = pCubtVtx[3] - pCubtVtx[0];
					_vec3 vDst = pCubtVtx[4] - pCubtVtx[0];
					D3DXVec3Cross(&vNormal, &vSrc, &vDst);
					D3DXVec3Normalize(&vNormal, &vNormal);

					memcpy(vNorm, vNormal, sizeof(_vec3));

					return WALL_LEFT;
				}
				if ((vPlayerPos.z > vWallPos.z) && (m_vMin1.x > m_vMin2.x) && (m_vMax1.x < m_vMax2.x))
				{
					// 벽의 전측에 충돌
					dwVtxIdx[0] = 7;
					dwVtxIdx[1] = 6;
					dwVtxIdx[2] = 5;
					_vec3 vSrc = pCubtVtx[6] - pCubtVtx[7];
					_vec3 vDst = pCubtVtx[6] - pCubtVtx[5];
					D3DXVec3Cross(&vNormal, &vSrc, &vDst);
					D3DXVec3Normalize(&vNormal, &vNormal);

					memcpy(vNorm, vNormal, sizeof(_vec3));

					return WALL_FRONT;
				}
				if ((vPlayerPos.z < vWallPos.z) && (m_vMin1.x > m_vMin2.x) && (m_vMax1.x < m_vMax2.x))
				{
					// 벽의 후측에 충돌
					dwVtxIdx[0] = 0;
					dwVtxIdx[1] = 1;
					dwVtxIdx[2] = 2;
					_vec3 vSrc = pCubtVtx[1] - pCubtVtx[0];
					_vec3 vDst = pCubtVtx[2] - pCubtVtx[1];
					D3DXVec3Cross(&vNormal, &vSrc, &vDst);
					D3DXVec3Normalize(&vNormal, &vNormal);

					memcpy(vNorm, vNormal, sizeof(_vec3));

					return WALL_BACK;
				}
				return -1;
			}
		}
	}

	return -1;
}

void CCollision::Wall_Collision_Check(CTransform * pMonsterTrans, CHitBox * pMonsterHit, _vec3* vDir)
{
	CLayer* pLayer = Engine::Get_Layer(STAGE_WALL);
	map<const _tchar*, CGameObject*> mapWall = pLayer->Get_GameObjectMap();

	_vec3 vMonsterPos;
	pMonsterTrans->Get_Info(INFO_POS, &vMonsterPos);

	for (auto iter : mapWall)
	{
		CTransform* pWallTransform = dynamic_cast<CTransform*>(iter.second->Get_Component(TRANSFORM_COMP, ID_DYNAMIC));
		NULL_CHECK_RETURN(pWallTransform, );
		CHitBox* pWallBox = dynamic_cast<CHitBox*>(iter.second->Get_Component(HITBOX_COMP, ID_STATIC));
		NULL_CHECK_RETURN(pWallBox, );

		pMonsterHit->Get_MinMax(&m_vMin1, &m_vMax1);
		pWallBox->Get_MinMax(&m_vMin2, &m_vMax2);

		D3DXVec3TransformCoord(&m_vMin1, &m_vMin1, pMonsterTrans->Get_WorldMatrixPointer());
		D3DXVec3TransformCoord(&m_vMax1, &m_vMax1, pMonsterTrans->Get_WorldMatrixPointer());
		D3DXVec3TransformCoord(&m_vMin2, &m_vMin2, pWallTransform->Get_WorldMatrixPointer());
		D3DXVec3TransformCoord(&m_vMax2, &m_vMax2, pWallTransform->Get_WorldMatrixPointer());

		int iCollisionCnt = 0;

		if (m_vMin1.x <= m_vMax2.x && m_vMax1.x >= m_vMin2.x &&
			m_vMin1.y <= m_vMax2.y && m_vMax1.y >= m_vMin2.y &&
			m_vMin1.z <= m_vMax2.z && m_vMax1.z >= m_vMin2.z)
		{
			if (m_vMin1.x > m_vMin2.x && m_vMax1.x < m_vMax2.x)
			{
				// 위아래로 충돌
				if (m_vMax1.z > m_vMax2.z && (*vDir).z < 0.f)
				{
					(*vDir).z = 0.f;
				}
				if (m_vMin1.z < m_vMin2.z && (*vDir).z > 0.f)
				{
					(*vDir).z = 0.f;
				}
			}
			else if (m_vMin1.z > m_vMin2.z && m_vMax1.z < m_vMax2.z)
			{
				if (m_vMax1.x > m_vMax2.x && (*vDir).x < 0.f)
				{
					(*vDir).x = 0.f;
				}
				if (m_vMin1.x < m_vMin2.x && (*vDir).x > 0.f)
				{
					(*vDir).x = 0.f;
				}
			}
			else
			{
				if ((m_vMax1.x > m_vMin2.x || m_vMin1.x < m_vMax2.x) && ( fabs(m_vMax1.x - m_vMin2.x) > 0.1 ) && (fabs(m_vMin1.x - m_vMax2.x) > 0.1))
				{
					if (m_vMax1.z > m_vMax2.z && (*vDir).z < 0.f)
					{
						(*vDir).z = 0.f;
					}
					if (m_vMin1.z < m_vMin2.z && (*vDir).z > 0.f)
					{
						(*vDir).z = 0.f;
					}
				}
				if ((m_vMax1.z > m_vMin2.z || m_vMin1.z < m_vMax2.z) && (fabs(m_vMax1.z - m_vMin2.z) > 0.1) && (fabs(m_vMin1.z - m_vMax2.z) > 0.1))
				{
					if (m_vMax1.x > m_vMax2.x && (*vDir).x < 0.f)
					{
						(*vDir).x = 0.f;
					}
					if (m_vMin1.x < m_vMin2.x && (*vDir).x > 0.f)
					{
						(*vDir).x = 0.f;
					}
				}
			}

		}
	}
}

_int CCollision::Wall_Collision_For_Monster(_vec3* vNorm, CTransform* pTransform, CHitBox* pHitBox)
{
	CLayer* pLayer = Engine::Get_Layer(STAGE_WALL);
	map<const _tchar*, CGameObject*> mapWall = pLayer->Get_GameObjectMap();

	pLayer = Engine::Get_Layer(STAGE_DESTORYWALL);
	list<CGameObject*> DestoryWallList = pLayer->Get_GameList();

	_vec3 vPos;
	pTransform->Get_Info(INFO_POS, &vPos);

	for (auto iter : mapWall)
	{
		CTransform* pWallTransform = dynamic_cast<CTransform*>(iter.second->Get_Component(TRANSFORM_COMP, ID_DYNAMIC));
		NULL_CHECK_RETURN(pWallTransform, false);
		CHitBox* pWallBox = dynamic_cast<CHitBox*>(iter.second->Get_Component(HITBOX_COMP, ID_STATIC));
		NULL_CHECK_RETURN(pWallBox, false);

		pHitBox->Get_MinMax(&m_vMin1, &m_vMax1);
		pWallBox->Get_MinMax(&m_vMin2, &m_vMax2);

		D3DXVec3TransformCoord(&m_vMin1, &m_vMin1, pTransform->Get_WorldMatrixPointer());
		D3DXVec3TransformCoord(&m_vMax1, &m_vMax1, pTransform->Get_WorldMatrixPointer());
		D3DXVec3TransformCoord(&m_vMin2, &m_vMin2, pWallTransform->Get_WorldMatrixPointer());
		D3DXVec3TransformCoord(&m_vMax2, &m_vMax2, pWallTransform->Get_WorldMatrixPointer());

		int iCollisionCnt = 0;

		if (m_vMin1.x <= m_vMax2.x && m_vMax1.x >= m_vMin2.x &&
			m_vMin1.y <= m_vMax2.y && m_vMax1.y >= m_vMin2.y &&
			m_vMin1.z <= m_vMax2.z && m_vMax1.z >= m_vMin2.z)
		{
			_vec3 vWallPos, vWallRight, vWallUp, vWallLook;
			pWallTransform->Get_Info(INFO_POS, &vWallPos);

			const _vec3*	pCubtVtx = pWallBox->Get_VtxPos();
			_vec3	vNormal(0, 0, 0);
			_ulong	dwVtxIdx[3]{};

			if ((vPos.x > vWallPos.x) && (m_vMin1.z > m_vMin2.z) && (m_vMax1.z < m_vMax2.z))
			{
				// 벽의 우측에 충돌
				dwVtxIdx[0] = 1;
				dwVtxIdx[1] = 5;
				dwVtxIdx[2] = 6;
				_vec3 vSrc = pCubtVtx[5] - pCubtVtx[6];
				_vec3 vDst = pCubtVtx[5] - pCubtVtx[1];
				D3DXVec3Cross(&vNormal, &vSrc, &vDst);
				D3DXVec3Normalize(&vNormal, &vNormal);

				memcpy(vNorm, vNormal, sizeof(_vec3));

				return WALL_RIGHT;
			}
			if ((vPos.x < vWallPos.x) && (m_vMin1.z > m_vMin2.z) && (m_vMax1.z < m_vMax2.z))
			{
				// 벽의 좌측에 충돌
				dwVtxIdx[0] = 4;
				dwVtxIdx[1] = 0;
				dwVtxIdx[2] = 3;
				_vec3 vSrc = pCubtVtx[3] - pCubtVtx[0];
				_vec3 vDst = pCubtVtx[4] - pCubtVtx[0];
				D3DXVec3Cross(&vNormal, &vSrc, &vDst);
				D3DXVec3Normalize(&vNormal, &vNormal);

				memcpy(vNorm, vNormal, sizeof(_vec3));

				return WALL_LEFT;
			}
			if ((vPos.z > vWallPos.z) && (m_vMin1.x > m_vMin2.x) && (m_vMax1.x < m_vMax2.x))
			{
				// 벽의 전측에 충돌
				dwVtxIdx[0] = 7;
				dwVtxIdx[1] = 6;
				dwVtxIdx[2] = 5;
				_vec3 vSrc = pCubtVtx[6] - pCubtVtx[7];
				_vec3 vDst = pCubtVtx[6] - pCubtVtx[5];
				D3DXVec3Cross(&vNormal, &vSrc, &vDst);
				D3DXVec3Normalize(&vNormal, &vNormal);

				memcpy(vNorm, vNormal, sizeof(_vec3));

				return WALL_FRONT;
			}
			if ((vPos.z < vWallPos.z) && (m_vMin1.x > m_vMin2.x) && (m_vMax1.x < m_vMax2.x))
			{
				// 벽의 후측에 충돌
				dwVtxIdx[0] = 0;
				dwVtxIdx[1] = 1;
				dwVtxIdx[2] = 2;
				_vec3 vSrc = pCubtVtx[1] - pCubtVtx[0];
				_vec3 vDst = pCubtVtx[2] - pCubtVtx[1];
				D3DXVec3Cross(&vNormal, &vSrc, &vDst);
				D3DXVec3Normalize(&vNormal, &vNormal);

				memcpy(vNorm, vNormal, sizeof(_vec3));

				return WALL_BACK;
			}
			return -1;
		}
	}

	if (!DestoryWallList.empty())
	{
		for (auto iter : DestoryWallList)
		{
			CTransform* pWallTransform = dynamic_cast<CTransform*>(iter->Get_Component(TRANSFORM_COMP, ID_DYNAMIC));
			NULL_CHECK_RETURN(pWallTransform, false);
			CHitBox* pWallBox = dynamic_cast<CHitBox*>(iter->Get_Component(HITBOX_COMP, ID_STATIC));
			NULL_CHECK_RETURN(pWallBox, false);

			pHitBox->Get_MinMax(&m_vMin1, &m_vMax1);
			pWallBox->Get_MinMax(&m_vMin2, &m_vMax2);

			D3DXVec3TransformCoord(&m_vMin1, &m_vMin1, pTransform->Get_WorldMatrixPointer());
			D3DXVec3TransformCoord(&m_vMax1, &m_vMax1, pTransform->Get_WorldMatrixPointer());
			D3DXVec3TransformCoord(&m_vMin2, &m_vMin2, pWallTransform->Get_WorldMatrixPointer());
			D3DXVec3TransformCoord(&m_vMax2, &m_vMax2, pWallTransform->Get_WorldMatrixPointer());

			int iCollisionCnt = 0;

			if (m_vMin1.x <= m_vMax2.x && m_vMax1.x >= m_vMin2.x &&
				m_vMin1.y <= m_vMax2.y && m_vMax1.y >= m_vMin2.y &&
				m_vMin1.z <= m_vMax2.z && m_vMax1.z >= m_vMin2.z)
			{
				_vec3 vWallPos, vWallRight, vWallUp, vWallLook;
				pWallTransform->Get_Info(INFO_POS, &vWallPos);

				const _vec3*	pCubtVtx = pWallBox->Get_VtxPos();
				_vec3	vNormal(0, 0, 0);
				_ulong	dwVtxIdx[3]{};

				if ((vPos.x > vWallPos.x) && (m_vMin1.z > m_vMin2.z) && (m_vMax1.z < m_vMax2.z))
				{
					// 벽의 우측에 충돌
					dwVtxIdx[0] = 1;
					dwVtxIdx[1] = 5;
					dwVtxIdx[2] = 6;
					_vec3 vSrc = pCubtVtx[5] - pCubtVtx[6];
					_vec3 vDst = pCubtVtx[5] - pCubtVtx[1];
					D3DXVec3Cross(&vNormal, &vSrc, &vDst);
					D3DXVec3Normalize(&vNormal, &vNormal);

					memcpy(vNorm, vNormal, sizeof(_vec3));

					return WALL_RIGHT;
				}
				if ((vPos.x < vWallPos.x) && (m_vMin1.z > m_vMin2.z) && (m_vMax1.z < m_vMax2.z))
				{
					// 벽의 좌측에 충돌
					dwVtxIdx[0] = 4;
					dwVtxIdx[1] = 0;
					dwVtxIdx[2] = 3;
					_vec3 vSrc = pCubtVtx[3] - pCubtVtx[0];
					_vec3 vDst = pCubtVtx[4] - pCubtVtx[0];
					D3DXVec3Cross(&vNormal, &vSrc, &vDst);
					D3DXVec3Normalize(&vNormal, &vNormal);

					memcpy(vNorm, vNormal, sizeof(_vec3));

					return WALL_LEFT;
				}
				if ((vPos.z > vWallPos.z) && (m_vMin1.x > m_vMin2.x) && (m_vMax1.x < m_vMax2.x))
				{
					// 벽의 전측에 충돌
					dwVtxIdx[0] = 7;
					dwVtxIdx[1] = 6;
					dwVtxIdx[2] = 5;
					_vec3 vSrc = pCubtVtx[6] - pCubtVtx[7];
					_vec3 vDst = pCubtVtx[6] - pCubtVtx[5];
					D3DXVec3Cross(&vNormal, &vSrc, &vDst);
					D3DXVec3Normalize(&vNormal, &vNormal);

					memcpy(vNorm, vNormal, sizeof(_vec3));

					return WALL_FRONT;
				}
				if ((vPos.z < vWallPos.z) && (m_vMin1.x > m_vMin2.x) && (m_vMax1.x < m_vMax2.x))
				{
					// 벽의 후측에 충돌
					dwVtxIdx[0] = 0;
					dwVtxIdx[1] = 1;
					dwVtxIdx[2] = 2;
					_vec3 vSrc = pCubtVtx[1] - pCubtVtx[0];
					_vec3 vDst = pCubtVtx[2] - pCubtVtx[1];
					D3DXVec3Cross(&vNormal, &vSrc, &vDst);
					D3DXVec3Normalize(&vNormal, &vNormal);

					memcpy(vNorm, vNormal, sizeof(_vec3));

					return WALL_BACK;
				}
				return -1;
			}
		}
	}

	return -1;
}

_int CCollision::Wall_Collision_By_DotSliding(_vec3 * vChangeDir)
{
	CLayer* pLayer = Engine::Get_Layer(STAGE_WALL);
	map<const _tchar*, CGameObject*> mapWall = pLayer->Get_GameObjectMap();

	pLayer = Engine::Get_Layer(STAGE_DESTORYWALL);
	list<CGameObject*> DestoryWallList = pLayer->Get_GameList();

	CTransform*	pPlayerTransform = dynamic_cast<CTransform*>(Get_Component(STAGE_CHARACTER, L"PLAYER", TRANSFORM_COMP, ID_DYNAMIC));
	CHitBox* pPlayerBox = dynamic_cast<CHitBox*>(Get_Component(STAGE_CHARACTER, L"PLAYER", HITBOX_COMP, ID_STATIC));

	CTransform* pPlayerBodyTransform = dynamic_cast<CTransform*>(Get_Component(STAGE_CHARACTER, L"BODY", TRANSFORM_COMP, ID_DYNAMIC));

	_vec3 vPlayerPos;
	pPlayerTransform->Get_Info(INFO_POS, &vPlayerPos);

	for (auto iter : mapWall)
	{
		CTransform* pWallTransform = dynamic_cast<CTransform*>(iter.second->Get_Component(TRANSFORM_COMP, ID_DYNAMIC));
		NULL_CHECK_RETURN(pWallTransform, false);
		CHitBox* pWallBox = dynamic_cast<CHitBox*>(iter.second->Get_Component(HITBOX_COMP, ID_STATIC));
		NULL_CHECK_RETURN(pWallTransform, false);

		pPlayerBox->Get_MinMax(&m_vMin1, &m_vMax1);
		pWallBox->Get_MinMax(&m_vMin2, &m_vMax2);

		D3DXVec3TransformCoord(&m_vMin1, &m_vMin1, pPlayerTransform->Get_WorldMatrixPointer());
		D3DXVec3TransformCoord(&m_vMax1, &m_vMax1, pPlayerTransform->Get_WorldMatrixPointer());
		D3DXVec3TransformCoord(&m_vMin2, &m_vMin2, pWallTransform->Get_WorldMatrixPointer());
		D3DXVec3TransformCoord(&m_vMax2, &m_vMax2, pWallTransform->Get_WorldMatrixPointer());

		if (m_vMin1.x <= m_vMax2.x && m_vMax1.x >= m_vMin2.x &&
			m_vMin1.y <= m_vMax2.y && m_vMax1.y >= m_vMin2.y &&
			m_vMin1.z <= m_vMax2.z && m_vMax1.z >= m_vMin2.z)
		{
			_vec3 vWallPos, vWallRight, vWallUp, vWallLook;
			pWallTransform->Get_Info(INFO_POS, &vWallPos);
			pWallTransform->Get_Info(INFO_RIGHT, &vWallRight);
			pWallTransform->Get_Info(INFO_UP, &vWallUp);
			pWallTransform->Get_Info(INFO_LOOK, &vWallLook);

			_vec3	vWallAxis, vPlayerDir, vSliding;

			if ((vPlayerPos.x > vWallPos.x) && (m_vMin1.z > m_vMin2.z) && (m_vMax1.z < m_vMax2.z))
			{
				// 벽의 우측에 충돌
				// Dir과 Look 내적하여 스칼라값 구하고
				// z이동값이 +이면 z+방향으로 스칼라값만큼 이동, z이동값이 -이면 z-방향으로 스칼라값만큼 이동

				vWallAxis = vWallLook;
				vPlayerDir = *vChangeDir;

				D3DXVec3Normalize(&vWallAxis, &vWallAxis);
				
				_float fScalar = D3DXVec3Dot(&vPlayerDir, &vWallAxis);

				vSliding = vWallAxis * fScalar;

				*vChangeDir = vSliding;

				return WALL_RIGHT;
			}
			if ((vPlayerPos.x < vWallPos.x) && (m_vMin1.z > m_vMin2.z) && (m_vMax1.z < m_vMax2.z))
			{
				// 벽의 좌측에 충돌
				// Dir과 Look 내적하여 스칼라값 구하고
				// z이동값이 +이면 z+방향으로 스칼라값만큼 이동, z이동값이 -이면 z-방향으로 스칼라값만큼 이동

				vWallAxis = vWallLook;
				vPlayerDir = *vChangeDir;

				D3DXVec3Normalize(&vWallAxis, &vWallAxis);

				_float fScalar = D3DXVec3Dot(&vPlayerDir, &vWallAxis);

				vSliding = vWallAxis * fScalar;

				*vChangeDir = vSliding;

				return WALL_LEFT;
			}
			if ((vPlayerPos.z > vWallPos.z) && (m_vMin1.x > m_vMin2.x) && (m_vMax1.x < m_vMax2.x))
			{
				// 벽의 전측에 충돌
				// Dir과 Right 내적하여 스칼라값 구하고
				// x이동값이 +이면 x+방향으로 스칼라값만큼 이동, x이동값이 -이면 x-방향으로 스칼라값만큼 이동

				vWallAxis = vWallRight;
				vPlayerDir = *vChangeDir;

				D3DXVec3Normalize(&vWallAxis, &vWallAxis);

				_float fScalar = D3DXVec3Dot(&vPlayerDir, &vWallAxis);

				vSliding = vWallAxis * fScalar;

				*vChangeDir = vSliding;

				return WALL_FRONT;
			}
			if ((vPlayerPos.z < vWallPos.z) && (m_vMin1.x > m_vMin2.x) && (m_vMax1.x < m_vMax2.x))
			{
				// 벽의 후측에 충돌
				// Dir과 Right 내적하여 스칼라값 구하고
				// x이동값이 +이면 x+방향으로 스칼라값만큼 이동, x이동값이 -이면 x-방향으로 스칼라값만큼 이동

				vWallAxis = vWallRight;
				vPlayerDir = *vChangeDir;

				D3DXVec3Normalize(&vWallAxis, &vWallAxis);

				_float fScalar = D3DXVec3Dot(&vPlayerDir, &vWallAxis);

				vSliding = vWallAxis * fScalar;

				*vChangeDir = vSliding;

				return WALL_BACK;
			}
		}
	}

	if (!DestoryWallList.empty())
	{
		for (auto iter : DestoryWallList)
		{
			CTransform* pWallTransform = dynamic_cast<CTransform*>(iter->Get_Component(TRANSFORM_COMP, ID_DYNAMIC));
			NULL_CHECK_RETURN(pWallTransform, false);
			CHitBox* pWallBox = dynamic_cast<CHitBox*>(iter->Get_Component(HITBOX_COMP, ID_STATIC));
			NULL_CHECK_RETURN(pWallTransform, false);

			pPlayerBox->Get_MinMax(&m_vMin1, &m_vMax1);
			pWallBox->Get_MinMax(&m_vMin2, &m_vMax2);

			D3DXVec3TransformCoord(&m_vMin1, &m_vMin1, pPlayerTransform->Get_WorldMatrixPointer());
			D3DXVec3TransformCoord(&m_vMax1, &m_vMax1, pPlayerTransform->Get_WorldMatrixPointer());
			D3DXVec3TransformCoord(&m_vMin2, &m_vMin2, pWallTransform->Get_WorldMatrixPointer());
			D3DXVec3TransformCoord(&m_vMax2, &m_vMax2, pWallTransform->Get_WorldMatrixPointer());

			if (m_vMin1.x <= m_vMax2.x && m_vMax1.x >= m_vMin2.x &&
				m_vMin1.y <= m_vMax2.y && m_vMax1.y >= m_vMin2.y &&
				m_vMin1.z <= m_vMax2.z && m_vMax1.z >= m_vMin2.z)
			{
				_vec3 vWallPos, vWallRight, vWallUp, vWallLook;
				pWallTransform->Get_Info(INFO_POS, &vWallPos);
				pWallTransform->Get_Info(INFO_RIGHT, &vWallRight);
				pWallTransform->Get_Info(INFO_UP, &vWallUp);
				pWallTransform->Get_Info(INFO_LOOK, &vWallLook);

				_vec3	vWallAxis, vPlayerDir, vSliding;

				if ((vPlayerPos.x > vWallPos.x) && (m_vMin1.z > m_vMin2.z) && (m_vMax1.z < m_vMax2.z))
				{
					// 벽의 우측에 충돌
					// Dir과 Look 내적하여 스칼라값 구하고
					// z이동값이 +이면 z+방향으로 스칼라값만큼 이동, z이동값이 -이면 z-방향으로 스칼라값만큼 이동

					vWallAxis = vWallLook;
					vPlayerDir = *vChangeDir;

					D3DXVec3Normalize(&vWallAxis, &vWallAxis);

					_float fScalar = D3DXVec3Dot(&vPlayerDir, &vWallAxis);

					vSliding = vWallAxis * fScalar;

					*vChangeDir = vSliding;

					return WALL_RIGHT;
				}
				if ((vPlayerPos.x < vWallPos.x) && (m_vMin1.z > m_vMin2.z) && (m_vMax1.z < m_vMax2.z))
				{
					// 벽의 좌측에 충돌
					// Dir과 Look 내적하여 스칼라값 구하고
					// z이동값이 +이면 z+방향으로 스칼라값만큼 이동, z이동값이 -이면 z-방향으로 스칼라값만큼 이동

					vWallAxis = vWallLook;
					vPlayerDir = *vChangeDir;

					D3DXVec3Normalize(&vWallAxis, &vWallAxis);

					_float fScalar = D3DXVec3Dot(&vPlayerDir, &vWallAxis);

					vSliding = vWallAxis * fScalar;

					*vChangeDir = vSliding;

					return WALL_LEFT;
				}
				if ((vPlayerPos.z > vWallPos.z) && (m_vMin1.x > m_vMin2.x) && (m_vMax1.x < m_vMax2.x))
				{
					// 벽의 전측에 충돌
					// Dir과 Right 내적하여 스칼라값 구하고
					// x이동값이 +이면 x+방향으로 스칼라값만큼 이동, x이동값이 -이면 x-방향으로 스칼라값만큼 이동

					vWallAxis = vWallRight;
					vPlayerDir = *vChangeDir;

					D3DXVec3Normalize(&vWallAxis, &vWallAxis);

					_float fScalar = D3DXVec3Dot(&vPlayerDir, &vWallAxis);

					vSliding = vWallAxis * fScalar;

					*vChangeDir = vSliding;

					return WALL_FRONT;
				}
				if ((vPlayerPos.z < vWallPos.z) && (m_vMin1.x > m_vMin2.x) && (m_vMax1.x < m_vMax2.x))
				{
					// 벽의 후측에 충돌
					// Dir과 Right 내적하여 스칼라값 구하고
					// x이동값이 +이면 x+방향으로 스칼라값만큼 이동, x이동값이 -이면 x-방향으로 스칼라값만큼 이동

					vWallAxis = vWallRight;
					vPlayerDir = *vChangeDir;

					D3DXVec3Normalize(&vWallAxis, &vWallAxis);

					_float fScalar = D3DXVec3Dot(&vPlayerDir, &vWallAxis);

					vSliding = vWallAxis * fScalar;

					*vChangeDir = vSliding;

					return WALL_BACK;
				}
			}
		}
	}

	return -1;
}

_int CCollision::Wall_Collision_By_DotSliding_For_Monster(_vec3* vChangeDir, CTransform* pTransform, CHitBox* pHitBox)
{
	CLayer* pLayer = Engine::Get_Layer(STAGE_WALL);
	map<const _tchar*, CGameObject*> mapWall = pLayer->Get_GameObjectMap();

	pLayer = Engine::Get_Layer(STAGE_DESTORYWALL);
	list<CGameObject*> DestoryWallList = pLayer->Get_GameList();

	_vec3 vPos;
	pTransform->Get_Info(INFO_POS, &vPos);

	for (auto iter : mapWall)
	{
		CTransform* pWallTransform = dynamic_cast<CTransform*>(iter.second->Get_Component(TRANSFORM_COMP, ID_DYNAMIC));
		NULL_CHECK_RETURN(pWallTransform, false);
		CHitBox* pWallBox = dynamic_cast<CHitBox*>(iter.second->Get_Component(HITBOX_COMP, ID_STATIC));
		NULL_CHECK_RETURN(pWallTransform, false);

		pHitBox->Get_MinMax(&m_vMin1, &m_vMax1);
		pWallBox->Get_MinMax(&m_vMin2, &m_vMax2);

		D3DXVec3TransformCoord(&m_vMin1, &m_vMin1, pTransform->Get_WorldMatrixPointer());
		D3DXVec3TransformCoord(&m_vMax1, &m_vMax1, pTransform->Get_WorldMatrixPointer());
		D3DXVec3TransformCoord(&m_vMin2, &m_vMin2, pWallTransform->Get_WorldMatrixPointer());
		D3DXVec3TransformCoord(&m_vMax2, &m_vMax2, pWallTransform->Get_WorldMatrixPointer());

		if (m_vMin1.x <= m_vMax2.x && m_vMax1.x >= m_vMin2.x &&
			m_vMin1.y <= m_vMax2.y && m_vMax1.y >= m_vMin2.y &&
			m_vMin1.z <= m_vMax2.z && m_vMax1.z >= m_vMin2.z)
		{
			_vec3 vWallPos, vWallRight, vWallUp, vWallLook;
			pWallTransform->Get_Info(INFO_POS, &vWallPos);
			pWallTransform->Get_Info(INFO_RIGHT, &vWallRight);
			pWallTransform->Get_Info(INFO_UP, &vWallUp);
			pWallTransform->Get_Info(INFO_LOOK, &vWallLook);

			_vec3	vWallAxis, vPlayerDir, vSliding;

			if ((vPos.x > vWallPos.x) && (m_vMin1.z > m_vMin2.z) && (m_vMax1.z < m_vMax2.z))
			{
				// 벽의 우측에 충돌
				// Dir과 Look 내적하여 스칼라값 구하고
				// z이동값이 +이면 z+방향으로 스칼라값만큼 이동, z이동값이 -이면 z-방향으로 스칼라값만큼 이동

				vWallAxis = vWallLook;
				vPlayerDir = *vChangeDir;

				D3DXVec3Normalize(&vWallAxis, &vWallAxis);

				_float fScalar = D3DXVec3Dot(&vPlayerDir, &vWallAxis);

				vSliding = vWallAxis * fScalar;

				*vChangeDir = vSliding;

				return WALL_RIGHT;
			}
			if ((vPos.x < vWallPos.x) && (m_vMin1.z > m_vMin2.z) && (m_vMax1.z < m_vMax2.z))
			{
				// 벽의 좌측에 충돌
				// Dir과 Look 내적하여 스칼라값 구하고
				// z이동값이 +이면 z+방향으로 스칼라값만큼 이동, z이동값이 -이면 z-방향으로 스칼라값만큼 이동

				vWallAxis = vWallLook;
				vPlayerDir = *vChangeDir;

				D3DXVec3Normalize(&vWallAxis, &vWallAxis);

				_float fScalar = D3DXVec3Dot(&vPlayerDir, &vWallAxis);

				vSliding = vWallAxis * fScalar;

				*vChangeDir = vSliding;

				return WALL_LEFT;
			}
			if ((vPos.z > vWallPos.z) && (m_vMin1.x > m_vMin2.x) && (m_vMax1.x < m_vMax2.x))
			{
				// 벽의 전측에 충돌
				// Dir과 Right 내적하여 스칼라값 구하고
				// x이동값이 +이면 x+방향으로 스칼라값만큼 이동, x이동값이 -이면 x-방향으로 스칼라값만큼 이동

				vWallAxis = vWallRight;
				vPlayerDir = *vChangeDir;

				D3DXVec3Normalize(&vWallAxis, &vWallAxis);

				_float fScalar = D3DXVec3Dot(&vPlayerDir, &vWallAxis);

				vSliding = vWallAxis * fScalar;

				*vChangeDir = vSliding;

				return WALL_FRONT;
			}
			if ((vPos.z < vWallPos.z) && (m_vMin1.x > m_vMin2.x) && (m_vMax1.x < m_vMax2.x))
			{
				// 벽의 후측에 충돌
				// Dir과 Right 내적하여 스칼라값 구하고
				// x이동값이 +이면 x+방향으로 스칼라값만큼 이동, x이동값이 -이면 x-방향으로 스칼라값만큼 이동

				vWallAxis = vWallRight;
				vPlayerDir = *vChangeDir;

				D3DXVec3Normalize(&vWallAxis, &vWallAxis);

				_float fScalar = D3DXVec3Dot(&vPlayerDir, &vWallAxis);

				vSliding = vWallAxis * fScalar;

				*vChangeDir = vSliding;

				return WALL_BACK;
			}
		}
	}

	if (!DestoryWallList.empty())
	{
		for (auto iter : DestoryWallList)
		{
			CTransform* pWallTransform = dynamic_cast<CTransform*>(iter->Get_Component(TRANSFORM_COMP, ID_DYNAMIC));
			NULL_CHECK_RETURN(pWallTransform, false);
			CHitBox* pWallBox = dynamic_cast<CHitBox*>(iter->Get_Component(HITBOX_COMP, ID_STATIC));
			NULL_CHECK_RETURN(pWallTransform, false);

			pHitBox->Get_MinMax(&m_vMin1, &m_vMax1);
			pWallBox->Get_MinMax(&m_vMin2, &m_vMax2);

			D3DXVec3TransformCoord(&m_vMin1, &m_vMin1, pTransform->Get_WorldMatrixPointer());
			D3DXVec3TransformCoord(&m_vMax1, &m_vMax1, pTransform->Get_WorldMatrixPointer());
			D3DXVec3TransformCoord(&m_vMin2, &m_vMin2, pWallTransform->Get_WorldMatrixPointer());
			D3DXVec3TransformCoord(&m_vMax2, &m_vMax2, pWallTransform->Get_WorldMatrixPointer());

			if (m_vMin1.x <= m_vMax2.x && m_vMax1.x >= m_vMin2.x &&
				m_vMin1.y <= m_vMax2.y && m_vMax1.y >= m_vMin2.y &&
				m_vMin1.z <= m_vMax2.z && m_vMax1.z >= m_vMin2.z)
			{
				_vec3 vWallPos, vWallRight, vWallUp, vWallLook;
				pWallTransform->Get_Info(INFO_POS, &vWallPos);
				pWallTransform->Get_Info(INFO_RIGHT, &vWallRight);
				pWallTransform->Get_Info(INFO_UP, &vWallUp);
				pWallTransform->Get_Info(INFO_LOOK, &vWallLook);

				_vec3	vWallAxis, vPlayerDir, vSliding;

				if ((vPos.x > vWallPos.x) && (m_vMin1.z > m_vMin2.z) && (m_vMax1.z < m_vMax2.z))
				{
					// 벽의 우측에 충돌
					// Dir과 Look 내적하여 스칼라값 구하고
					// z이동값이 +이면 z+방향으로 스칼라값만큼 이동, z이동값이 -이면 z-방향으로 스칼라값만큼 이동

					vWallAxis = vWallLook;
					vPlayerDir = *vChangeDir;

					D3DXVec3Normalize(&vWallAxis, &vWallAxis);

					_float fScalar = D3DXVec3Dot(&vPlayerDir, &vWallAxis);

					vSliding = vWallAxis * fScalar;

					*vChangeDir = vSliding;

					return WALL_RIGHT;
				}
				if ((vPos.x < vWallPos.x) && (m_vMin1.z > m_vMin2.z) && (m_vMax1.z < m_vMax2.z))
				{
					// 벽의 좌측에 충돌
					// Dir과 Look 내적하여 스칼라값 구하고
					// z이동값이 +이면 z+방향으로 스칼라값만큼 이동, z이동값이 -이면 z-방향으로 스칼라값만큼 이동

					vWallAxis = vWallLook;
					vPlayerDir = *vChangeDir;

					D3DXVec3Normalize(&vWallAxis, &vWallAxis);

					_float fScalar = D3DXVec3Dot(&vPlayerDir, &vWallAxis);

					vSliding = vWallAxis * fScalar;

					*vChangeDir = vSliding;

					return WALL_LEFT;
				}
				if ((vPos.z > vWallPos.z) && (m_vMin1.x > m_vMin2.x) && (m_vMax1.x < m_vMax2.x))
				{
					// 벽의 전측에 충돌
					// Dir과 Right 내적하여 스칼라값 구하고
					// x이동값이 +이면 x+방향으로 스칼라값만큼 이동, x이동값이 -이면 x-방향으로 스칼라값만큼 이동

					vWallAxis = vWallRight;
					vPlayerDir = *vChangeDir;

					D3DXVec3Normalize(&vWallAxis, &vWallAxis);

					_float fScalar = D3DXVec3Dot(&vPlayerDir, &vWallAxis);

					vSliding = vWallAxis * fScalar;

					*vChangeDir = vSliding;

					return WALL_FRONT;
				}
				if ((vPos.z < vWallPos.z) && (m_vMin1.x > m_vMin2.x) && (m_vMax1.x < m_vMax2.x))
				{
					// 벽의 후측에 충돌
					// Dir과 Right 내적하여 스칼라값 구하고
					// x이동값이 +이면 x+방향으로 스칼라값만큼 이동, x이동값이 -이면 x-방향으로 스칼라값만큼 이동

					vWallAxis = vWallRight;
					vPlayerDir = *vChangeDir;

					D3DXVec3Normalize(&vWallAxis, &vWallAxis);

					_float fScalar = D3DXVec3Dot(&vPlayerDir, &vWallAxis);

					vSliding = vWallAxis * fScalar;

					*vChangeDir = vSliding;

					return WALL_BACK;
				}
			}
		}
	}

	return -1;
}

_bool CCollision::Collision_Square(CTransform * pSrcTrans, CHitBox * pSrcHit, CTransform * pDstTrans, CHitBox * pDstHit)
{
	pSrcHit->Get_MinMax(&m_vMin1, &m_vMax1);
	pDstHit->Get_MinMax(&m_vMin2, &m_vMax2);

	D3DXVec3TransformCoord(&m_vMin1, &m_vMin1, pSrcTrans->Get_WorldMatrixPointer());
	D3DXVec3TransformCoord(&m_vMax1, &m_vMax1, pSrcTrans->Get_WorldMatrixPointer());
	D3DXVec3TransformCoord(&m_vMin2, &m_vMin2, pDstTrans->Get_WorldMatrixPointer());
	D3DXVec3TransformCoord(&m_vMax2, &m_vMax2, pDstTrans->Get_WorldMatrixPointer());

	if (m_vMin1.x <= m_vMax2.x && m_vMax1.x >= m_vMin2.x &&
		m_vMin1.y <= m_vMax2.y && m_vMax1.y >= m_vMin2.y &&
		m_vMin1.z <= m_vMax2.z && m_vMax1.z >= m_vMin2.z)
	{
		return true;
	}
	return false;
}

void CCollision::Get_Item(void)
{
	CTransform*	pPlayerTransform = dynamic_cast<CTransform*>(Get_Component(STAGE_CHARACTER, L"PLAYER", TRANSFORM_COMP, ID_DYNAMIC));
	CHitBox* pPlayerBox = dynamic_cast<CHitBox*>(Get_Component(STAGE_CHARACTER, L"PLAYER", HITBOX_COMP, ID_STATIC));
	CTransform* pPlayerBodyTransform = dynamic_cast<CTransform*>(Get_Component(STAGE_CHARACTER, L"BODY", TRANSFORM_COMP, ID_DYNAMIC));
	if (Get_Scene()->Get_SceneId() == STAGE_SCENE)
	{
		CLayer* pLayer = Engine::Get_Layer(L"STAGE_KEY");
		list<CGameObject*> KeyList = pLayer->Get_GameList();
		
		if (KeyList.empty())
			return;

		for (auto iter : KeyList)
		{
			CTransform* pItemTransform = dynamic_cast<CTransform*>(iter->Get_Component(ITEM_TRANSFORM_COMP, ID_DYNAMIC));
			NULL_CHECK_RETURN(pItemTransform, );
			CHitBox* pItemBox = dynamic_cast<CHitBox*>(iter->Get_Component(HITBOX_COMP, ID_STATIC));
			NULL_CHECK_RETURN(pItemBox, );

			pPlayerBox->Get_MinMax(&m_vMin1, &m_vMax1);
			pItemBox->Get_MinMax(&m_vMin2, &m_vMax2);

			D3DXVec3TransformCoord(&m_vMin1, &m_vMin1, pPlayerTransform->Get_WorldMatrixPointer());
			D3DXVec3TransformCoord(&m_vMax1, &m_vMax1, pPlayerTransform->Get_WorldMatrixPointer());
			D3DXVec3TransformCoord(&m_vMin2, &m_vMin2, pItemTransform->Get_WorldMatrixPointer());
			D3DXVec3TransformCoord(&m_vMax2, &m_vMax2, pItemTransform->Get_WorldMatrixPointer());

			if (m_vMin1.x <= m_vMax2.x && m_vMax1.x >= m_vMin2.x &&
				m_vMin1.y <= m_vMax2.y && m_vMax1.y >= m_vMin2.y &&
				m_vMin1.z <= m_vMax2.z && m_vMax1.z >= m_vMin2.z)
			{
				iter->Kill_Obj();
			}
		}
	}

	CLayer* pLayer = Engine::Get_Layer(STAGE_ITEM);
	list<CGameObject*> ItemList = pLayer->Get_GameList();

	if (ItemList.empty())
		return;

	_vec3 vPlayerPos;
	pPlayerTransform->Get_Info(INFO_POS, &vPlayerPos);

	for (auto iter : ItemList)
	{
		CTransform* pItemTransform = dynamic_cast<CTransform*>(iter->Get_Component(ITEM_TRANSFORM_COMP, ID_DYNAMIC));
		NULL_CHECK_RETURN(pItemTransform, );
		CHitBox* pItemBox = dynamic_cast<CHitBox*>(iter->Get_Component(HITBOX_COMP, ID_STATIC));
		NULL_CHECK_RETURN(pItemBox, );

		pPlayerBox->Get_MinMax(&m_vMin1, &m_vMax1);
		pItemBox->Get_MinMax(&m_vMin2, &m_vMax2);

		D3DXVec3TransformCoord(&m_vMin1, &m_vMin1, pPlayerTransform->Get_WorldMatrixPointer());
		D3DXVec3TransformCoord(&m_vMax1, &m_vMax1, pPlayerTransform->Get_WorldMatrixPointer());
		D3DXVec3TransformCoord(&m_vMin2, &m_vMin2, pItemTransform->Get_WorldMatrixPointer());
		D3DXVec3TransformCoord(&m_vMax2, &m_vMax2, pItemTransform->Get_WorldMatrixPointer());

		if (m_vMin1.x <= m_vMax2.x && m_vMax1.x >= m_vMin2.x &&
			m_vMin1.y <= m_vMax2.y && m_vMax1.y >= m_vMin2.y &&
			m_vMin1.z <= m_vMax2.z && m_vMax1.z >= m_vMin2.z)
		{
			iter->Kill_Obj();
		}
	}


	
}

void CCollision::Get_GunItem()
{
	CLayer* pLayer = Engine::Get_Layer(STAGE_GUNITEM);
	map<const _tchar*, CGameObject*> mapItem = pLayer->Get_GameObjectMap();

	CTransform*	pPlayerTransform = dynamic_cast<CTransform*>(Get_Component(STAGE_CHARACTER, L"PLAYER", TRANSFORM_COMP, ID_DYNAMIC));
	CHitBox* pPlayerBox = dynamic_cast<CHitBox*>(Get_Component(STAGE_CHARACTER, L"PLAYER", HITBOX_COMP, ID_STATIC));
	CTransform* pPlayerBodyTransform = dynamic_cast<CTransform*>(Get_Component(STAGE_CHARACTER, L"BODY", TRANSFORM_COMP, ID_DYNAMIC));

	if (mapItem.empty())
		return;

	_vec3 vPlayerPos;
	pPlayerTransform->Get_Info(INFO_POS, &vPlayerPos);

	for (auto iter : mapItem)
	{
		CTransform* pItemTransform = dynamic_cast<CTransform*>(iter.second->Get_Component(ITEM_TRANSFORM_COMP, ID_DYNAMIC));
		NULL_CHECK_RETURN(pItemTransform, );
		CHitBox* pItemBox = dynamic_cast<CHitBox*>(iter.second->Get_Component(HITBOX_COMP, ID_STATIC));
		NULL_CHECK_RETURN(pItemBox, );

		pPlayerBox->Get_MinMax(&m_vMin1, &m_vMax1);
		pItemBox->Get_MinMax(&m_vMin2, &m_vMax2);

		D3DXVec3TransformCoord(&m_vMin1, &m_vMin1, pPlayerTransform->Get_WorldMatrixPointer());
		D3DXVec3TransformCoord(&m_vMax1, &m_vMax1, pPlayerTransform->Get_WorldMatrixPointer());
		D3DXVec3TransformCoord(&m_vMin2, &m_vMin2, pItemTransform->Get_WorldMatrixPointer());
		D3DXVec3TransformCoord(&m_vMax2, &m_vMax2, pItemTransform->Get_WorldMatrixPointer());

		if (m_vMin1.x <= m_vMax2.x && m_vMax1.x >= m_vMin2.x &&
			m_vMin1.y <= m_vMax2.y && m_vMax1.y >= m_vMin2.y &&
			m_vMin1.z <= m_vMax2.z && m_vMax1.z >= m_vMin2.z)
		{
			iter.second->Kill_Obj();
			// 여기서 함수 실행 합시다.
			CGameObject* pGameObject = dynamic_cast<CFlight*>(Get_GameObject(STAGE_FLIGHTPLAYER, L"FLIGHTSHUTTLE"));

			if (iter.second->m_eSupporterID == SUPPORTER_UZI)
			{
				_vec3 vDestionaion;
				pItemTransform->Get_Info(INFO_POS, &vDestionaion);
				dynamic_cast<CFlight*>(Get_GameObject(STAGE_FLIGHTPLAYER, L"FLIGHTSHUTTLE"))->Set_Shuttle(true, vDestionaion, SUPPORTER_UZI);
			}
			else if (iter.second->m_eSupporterID == SUPPORTER_SHOTGUN)
			{
				_vec3 vDestionaion;
				pItemTransform->Get_Info(INFO_POS, &vDestionaion);
				dynamic_cast<CFlight*>(Get_GameObject(STAGE_FLIGHTPLAYER, L"FLIGHTSHUTTLE"))->Set_Shuttle(true, vDestionaion, SUPPORTER_SHOTGUN);
			}
			else if (iter.second->m_eSupporterID == SUPPORTER_SNIPER)
			{
				_vec3 vDestionaion;
				pItemTransform->Get_Info(INFO_POS, &vDestionaion);
				dynamic_cast<CFlight*>(Get_GameObject(STAGE_FLIGHTPLAYER, L"FLIGHTSHUTTLE"))->Set_Shuttle(true, vDestionaion, SUPPORTER_SNIPER);
			}
		}
	}
}

_bool CCollision::HitScan(HWND hWnd, _vec3 * SrcPos, const CCubeTex * pCubeTex, const CTransform * pTransform, _vec3* vReturn)
{
	if (!Get_DIMouseState(DIM_RB))
		return false;
	if (!Get_DIMouseState(DIM_LB))
		return false;

	POINT		ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(hWnd, &ptMouse);

	_vec3		vPoint;

	D3DVIEWPORT9		ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
	m_pGraphicDev->GetViewport(&ViewPort);

	// 뷰포트 -> 투영
	vPoint.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	vPoint.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;
	vPoint.z = 1.f;	//	이거 1해야됨!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////

	// 투영 -> 뷰 스페이스
	_matrix		matProj;

	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, nullptr, &matProj);
	D3DXVec3TransformCoord(&vPoint, &vPoint, &matProj);

	_vec3	vRayDir, vRayPos;		// 뷰 스페이스 영역에 있는 상태

	// 뷰 스페이스 -> 월드

	_matrix		matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	vRayPos = *SrcPos;

	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);
	vRayDir = vPoint - vRayPos;
	D3DXMatrixInverse(&matView, nullptr, &matView);
	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);

	D3DXVec3Normalize(&vRayDir, &vRayDir);

	*vReturn = vRayDir;

	// 월드 -> 로컬
	_matrix		matWorld;

	pTransform->Get_WorldMatrix(&matWorld);

	const _vec3*	pCubtVtx = pCubeTex->Get_VtxPos();

	_ulong	dwVtxIdx[3]{};
	_float	fU, fV, fDist;
	_vec3	vWorldVtx_01, vWorldVtx_02, vWorldVtx_03;

	// +X
	dwVtxIdx[0] = 1;
	dwVtxIdx[1] = 5;
	dwVtxIdx[2] = 6;
	D3DXVec3TransformCoord(&vWorldVtx_01, &pCubtVtx[dwVtxIdx[0]], &matWorld);
	D3DXVec3TransformCoord(&vWorldVtx_02, &pCubtVtx[dwVtxIdx[1]], &matWorld);
	D3DXVec3TransformCoord(&vWorldVtx_03, &pCubtVtx[dwVtxIdx[2]], &matWorld);
	if (D3DXIntersectTri(&vWorldVtx_01,
		&vWorldVtx_02,
		&vWorldVtx_03,
		&vRayPos, &vRayDir,
		&fU, &fV, &fDist))
	{
		return true;
	}
	dwVtxIdx[0] = 1;
	dwVtxIdx[1] = 6;
	dwVtxIdx[2] = 2;
	D3DXVec3TransformCoord(&vWorldVtx_01, &pCubtVtx[dwVtxIdx[0]], &matWorld);
	D3DXVec3TransformCoord(&vWorldVtx_02, &pCubtVtx[dwVtxIdx[1]], &matWorld);
	D3DXVec3TransformCoord(&vWorldVtx_03, &pCubtVtx[dwVtxIdx[2]], &matWorld);
	if (D3DXIntersectTri(&vWorldVtx_01,
		&vWorldVtx_02,
		&vWorldVtx_03,
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
	D3DXVec3TransformCoord(&vWorldVtx_01, &pCubtVtx[dwVtxIdx[0]], &matWorld);
	D3DXVec3TransformCoord(&vWorldVtx_02, &pCubtVtx[dwVtxIdx[1]], &matWorld);
	D3DXVec3TransformCoord(&vWorldVtx_03, &pCubtVtx[dwVtxIdx[2]], &matWorld);
	if (D3DXIntersectTri(&vWorldVtx_01,
		&vWorldVtx_02,
		&vWorldVtx_03,
		&vRayPos, &vRayDir,
		&fU, &fV, &fDist))
	{
		return true;
	}

	dwVtxIdx[0] = 4;
	dwVtxIdx[1] = 3;
	dwVtxIdx[2] = 7;
	D3DXVec3TransformCoord(&vWorldVtx_01, &pCubtVtx[dwVtxIdx[0]], &matWorld);
	D3DXVec3TransformCoord(&vWorldVtx_02, &pCubtVtx[dwVtxIdx[1]], &matWorld);
	D3DXVec3TransformCoord(&vWorldVtx_03, &pCubtVtx[dwVtxIdx[2]], &matWorld);
	if (D3DXIntersectTri(&vWorldVtx_01,
		&vWorldVtx_02,
		&vWorldVtx_03,
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
	D3DXVec3TransformCoord(&vWorldVtx_01, &pCubtVtx[dwVtxIdx[0]], &matWorld);
	D3DXVec3TransformCoord(&vWorldVtx_02, &pCubtVtx[dwVtxIdx[1]], &matWorld);
	D3DXVec3TransformCoord(&vWorldVtx_03, &pCubtVtx[dwVtxIdx[2]], &matWorld);
	if (D3DXIntersectTri(&vWorldVtx_01,
		&vWorldVtx_02,
		&vWorldVtx_03,
		&vRayPos, &vRayDir,
		&fU, &fV, &fDist))
	{
		return true;
	}

	dwVtxIdx[0] = 4;
	dwVtxIdx[1] = 1;
	dwVtxIdx[2] = 0;
	D3DXVec3TransformCoord(&vWorldVtx_01, &pCubtVtx[dwVtxIdx[0]], &matWorld);
	D3DXVec3TransformCoord(&vWorldVtx_02, &pCubtVtx[dwVtxIdx[1]], &matWorld);
	D3DXVec3TransformCoord(&vWorldVtx_03, &pCubtVtx[dwVtxIdx[2]], &matWorld);
	if (D3DXIntersectTri(&vWorldVtx_01,
		&vWorldVtx_02,
		&vWorldVtx_03,
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
	D3DXVec3TransformCoord(&vWorldVtx_01, &pCubtVtx[dwVtxIdx[0]], &matWorld);
	D3DXVec3TransformCoord(&vWorldVtx_02, &pCubtVtx[dwVtxIdx[1]], &matWorld);
	D3DXVec3TransformCoord(&vWorldVtx_03, &pCubtVtx[dwVtxIdx[2]], &matWorld);
	if (D3DXIntersectTri(&vWorldVtx_01,
		&vWorldVtx_02,
		&vWorldVtx_03,
		&vRayPos, &vRayDir,
		&fU, &fV, &fDist))
	{
		return true;
	}

	dwVtxIdx[0] = 3;
	dwVtxIdx[1] = 6;
	dwVtxIdx[2] = 7;
	D3DXVec3TransformCoord(&vWorldVtx_01, &pCubtVtx[dwVtxIdx[0]], &matWorld);
	D3DXVec3TransformCoord(&vWorldVtx_02, &pCubtVtx[dwVtxIdx[1]], &matWorld);
	D3DXVec3TransformCoord(&vWorldVtx_03, &pCubtVtx[dwVtxIdx[2]], &matWorld);
	if (D3DXIntersectTri(&vWorldVtx_01,
		&vWorldVtx_02,
		&vWorldVtx_03,
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
	D3DXVec3TransformCoord(&vWorldVtx_01, &pCubtVtx[dwVtxIdx[0]], &matWorld);
	D3DXVec3TransformCoord(&vWorldVtx_02, &pCubtVtx[dwVtxIdx[1]], &matWorld);
	D3DXVec3TransformCoord(&vWorldVtx_03, &pCubtVtx[dwVtxIdx[2]], &matWorld);
	if (D3DXIntersectTri(&vWorldVtx_01,
		&vWorldVtx_02,
		&vWorldVtx_03,
		&vRayPos, &vRayDir,
		&fU, &fV, &fDist))
	{
		return true;
	}

	dwVtxIdx[0] = 7;
	dwVtxIdx[1] = 5;
	dwVtxIdx[2] = 4;
	D3DXVec3TransformCoord(&vWorldVtx_01, &pCubtVtx[dwVtxIdx[0]], &matWorld);
	D3DXVec3TransformCoord(&vWorldVtx_02, &pCubtVtx[dwVtxIdx[1]], &matWorld);
	D3DXVec3TransformCoord(&vWorldVtx_03, &pCubtVtx[dwVtxIdx[2]], &matWorld);
	if (D3DXIntersectTri(&vWorldVtx_01,
		&vWorldVtx_02,
		&vWorldVtx_03,
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
	D3DXVec3TransformCoord(&vWorldVtx_01, &pCubtVtx[dwVtxIdx[0]], &matWorld);
	D3DXVec3TransformCoord(&vWorldVtx_02, &pCubtVtx[dwVtxIdx[1]], &matWorld);
	D3DXVec3TransformCoord(&vWorldVtx_03, &pCubtVtx[dwVtxIdx[2]], &matWorld);
	if (D3DXIntersectTri(&vWorldVtx_01,
		&vWorldVtx_02,
		&vWorldVtx_03,
		&vRayPos, &vRayDir,
		&fU, &fV, &fDist))
	{
		return true;
	}

	dwVtxIdx[0] = 0;
	dwVtxIdx[1] = 2;
	dwVtxIdx[2] = 3;
	D3DXVec3TransformCoord(&vWorldVtx_01, &pCubtVtx[dwVtxIdx[0]], &matWorld);
	D3DXVec3TransformCoord(&vWorldVtx_02, &pCubtVtx[dwVtxIdx[1]], &matWorld);
	D3DXVec3TransformCoord(&vWorldVtx_03, &pCubtVtx[dwVtxIdx[2]], &matWorld);
	if (D3DXIntersectTri(&vWorldVtx_01,
		&vWorldVtx_02,
		&vWorldVtx_03,
		&vRayPos, &vRayDir,
		&fU, &fV, &fDist))
	{
		return true;
	}

	// ~Z-
	return false;
}

_bool CCollision::Hit_In_ViewPort(HWND hWnd, const CCubeTex * pCubeTex, const CTransform * pTransform)
{
	POINT		ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(hWnd, &ptMouse);

	const _vec3*	pCubtVtx = pCubeTex->Get_VtxPos();
	_vec3	vChangeVtx[8];

	_matrix matWorld;
	pTransform->Get_WorldMatrix(&matWorld);
	for (int i = 0; i < 8; ++i)
	{
		D3DXVec3TransformCoord(&vChangeVtx[i], &pCubtVtx[i], &matWorld);
	}

	_matrix matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	for (int i = 0; i < 8; ++i)
	{
		D3DXVec3TransformCoord(&vChangeVtx[i], &vChangeVtx[i], &matView);
	}

	_matrix matProj;
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	for (int i = 0; i < 8; ++i)
	{
		D3DXVec3TransformCoord(&vChangeVtx[i], &vChangeVtx[i], &matProj);
	}

	D3DVIEWPORT9 Viewport;
	m_pGraphicDev->GetViewport(&Viewport);

	_matrix matVP;
	D3DXMatrixIdentity(&matVP);

	matVP.m[0][0] = Viewport.Width / 2.f;
	matVP.m[1][1] = -(Viewport.Height / 2.f);
	matVP.m[2][2] = Viewport.MaxZ - Viewport.MinZ;
	matVP.m[3][0] = Viewport.X + (Viewport.Width / 2.f);	
	matVP.m[3][1] = (Viewport.Height / 2.f) + Viewport.Y;
	matVP.m[3][2] = Viewport.MinZ;

	_vec3 pt[8];
	for (int i = 0; i < 8; ++i)
	{
		D3DXVec3TransformCoord(&pt[i], &vChangeVtx[i], &matVP);
	}

	return false;
}

CCollision * CCollision::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCollision* pInstance = new CCollision(pGraphicDev);

	if (FAILED(pInstance->Ready_Collision()))
	{
		MSG_BOX("Collision Create Fail");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CComponent * CCollision::Clone(void)
{
	return new CCollision(*this);
}

void CCollision::Free(void)
{
	CComponent::Free();
}
