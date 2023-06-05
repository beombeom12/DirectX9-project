#include "..\..\Header\Transform.h"

#include "Export_Function.h"

USING(Engine)

CTransform::CTransform(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)
	, m_vScale(1.f, 1.f, 1.f)
	, m_vAngle(0.f, 0.f, 0.f)
{
	ZeroMemory(m_vInfo, sizeof(m_vInfo));
	D3DXMatrixIdentity(&m_matWorld);
}


Engine::CTransform::CTransform(const CTransform& rhs)
	: CComponent(rhs),m_vScale(rhs.m_vScale), m_vAngle(rhs.m_vAngle)
{
	for (_uint i = 0; i < INFO_END; ++i)
		memcpy(m_vInfo[i], rhs.m_vInfo[i], sizeof(_vec3));

	memcpy(m_matWorld, rhs.m_matWorld, sizeof(_matrix));
}

CTransform::~CTransform()
{
}

void CTransform::Rotation_Axis_X(const _float & fMovement, const _float & fAngle)
{
	_matrix matScale, matRot, matTrans;

	_matrix matUp, matDown;

	_matrix matAxis;

	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixRotationX(&matRot, fAngle);
	D3DXMatrixTranslation(&matTrans, m_vInfo[INFO_POS].x, m_vInfo[INFO_POS].y, m_vInfo[INFO_POS].z);

	D3DXMatrixTranslation(&matUp, 0.f, fMovement, 0.f);
	D3DXMatrixTranslation(&matDown, 0.f, -fMovement, 0.f);

	m_matWorld = matScale * matDown * matRot * matUp  *matTrans;
}

void CTransform::Rotation_Axis_Y(const _float & fMovement, const _float & fAngle)
{
	_matrix matScale, matRot, matTrans;

	_matrix matMove, matOriginalPos;

	_matrix matAxis;

	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixRotationY(&matRot, fAngle);
	D3DXMatrixTranslation(&matTrans, m_vInfo[INFO_POS].x, m_vInfo[INFO_POS].y, m_vInfo[INFO_POS].z);

	D3DXMatrixTranslation(&matMove, fMovement, 0.f, 0.f);
	D3DXMatrixTranslation(&matOriginalPos, -(fMovement), 0.f, 0.f);

	m_matWorld = matScale * matMove * matRot * matOriginalPos * matTrans;
}

void	CTransform::Rotation_Revolution_Z(const _float& fAngle)
{
	_matrix matScale, matRot, matTrans;

	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixRotationZ(&matRot, fAngle);
	D3DXMatrixTranslation(&matTrans, m_vInfo[INFO_POS].x, m_vInfo[INFO_POS].y, m_vInfo[INFO_POS].z);

	m_matWorld = matScale * matTrans * matRot;
}

void CTransform::Rotation_Axis_Gun(	const _float & fXMove, const _float & fXAngle,
									const _float & fYMove, const _float & fYAngle,
									const _float & fZMove, const _float & fZAngle)
{
	_matrix matScale, matRotX, matRotY, matRotZ, matTrans;

	_matrix matSyncX, matReplaceX, matSyncY, matReplaceY, matSyncZ, matReplaceZ;

	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixTranslation(&matTrans, m_vInfo[INFO_POS].x, m_vInfo[INFO_POS].y, m_vInfo[INFO_POS].z);

	//	X
	D3DXMatrixTranslation(&matSyncX, 0.f, fXMove, 0.f);
	D3DXMatrixTranslation(&matReplaceX, 0.f, -fXMove, 0.f);
	D3DXMatrixRotationX(&matRotX, fXAngle);

	//	Y
	D3DXMatrixTranslation(&matSyncY, fYMove, 0.f, 0.f);
	D3DXMatrixTranslation(&matReplaceY, -fYMove, 0.f, 0.f);
	D3DXMatrixRotationY(&matRotY, fYAngle);

	//	Z
	D3DXMatrixTranslation(&matSyncZ, fZMove, 0.f, 0.f);
	D3DXMatrixTranslation(&matReplaceZ, -fZMove, 0.f, 0.f);
	D3DXMatrixRotationX(&matRotZ, fZAngle);

	m_matWorld = matScale *
		matSyncX * matRotX * matReplaceX *
		matSyncY * matRotY * matReplaceY *
		matSyncZ * matRotZ * matReplaceZ *
		matTrans;
}

void CTransform::Flexible_WorldSpace(_vec3 * vScale, _vec3 * vAngle, _vec3 * vTrans)
{
	_matrix matScale, matRotX, matRotY, matRotZ, matTrans;

	D3DXMatrixScaling(&matScale, vScale->x, vScale->y, vScale->z);
	D3DXMatrixRotationX(&matRotX, vAngle->x);
	D3DXMatrixRotationX(&matRotY, vAngle->y);
	D3DXMatrixRotationX(&matRotZ, vAngle->z);
	D3DXMatrixTranslation(&matTrans, vTrans->x, vTrans->y, vTrans->z);

	m_matWorld = matScale * matRotX * matRotY * matRotZ * matTrans;
}

void CTransform::Worldspace_By_Quarternion(void)
{
	_matrix matScale, matRotQuaternion, matTrans;

	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);

	D3DXQuaternionRotationYawPitchRoll(&Quaternion, m_vAngle.x, m_vAngle.y, m_vAngle.z);
	D3DXMatrixRotationQuaternion(&matRotQuaternion, &Quaternion);

	D3DXMatrixTranslation(&matTrans, m_vInfo[INFO_POS].x, m_vInfo[INFO_POS].y, m_vInfo[INFO_POS].z);

	m_matWorld = matScale * matRotQuaternion * matTrans;
}

void CTransform::Quaternion_Transform(void)
{
	_matrix matScale, matRotQuaternion, matTrans;

	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);

	D3DXQuaternionRotationYawPitchRoll(&Quaternion, m_vAngle.y, m_vAngle.x, m_vAngle.z);
	D3DXMatrixRotationQuaternion(&matRotQuaternion, &Quaternion);

	D3DXMatrixTranslation(&matTrans, m_vInfo[INFO_POS].x, m_vInfo[INFO_POS].y, m_vInfo[INFO_POS].z);

	m_matWorld = matScale * matRotQuaternion * matTrans;
}

void CTransform::Rotation_Axis_Animation(const _float & fXMove, const _float & fYMove, const _float & fXAngle, const _float & fYAngle, const _float& fExtraMove, const _float& fExtraAngle)
{
	_matrix matScale, matRotX, matRotY, matTrans;

	_matrix matSyncX, matReplaceX, matSyncY, matReplaceY;

	_matrix matSyncZ;

	_matrix matExtraSync, matExtraReplace, matRotExtra;

	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixTranslation(&matTrans, m_vInfo[INFO_POS].x, m_vInfo[INFO_POS].y, m_vInfo[INFO_POS].z);

	//	X
	D3DXMatrixTranslation(&matSyncX, 0.f, fXMove, 0.f);
	D3DXMatrixTranslation(&matReplaceX, 0.f, -fXMove, 0.f);
	D3DXMatrixRotationX(&matRotX, fXAngle);

	//	Y
	D3DXMatrixTranslation(&matSyncY, fYMove, 0.f, 0.f);
	D3DXMatrixTranslation(&matReplaceY, -fYMove, 0.f, 0.f);
	D3DXMatrixRotationY(&matRotY, fYAngle);

	//	Extra
	D3DXMatrixTranslation(&matExtraSync, 0.f, fExtraMove, 0.f);
	D3DXMatrixTranslation(&matExtraReplace, 0.f, -fExtraMove, 0.f);
	D3DXMatrixRotationX(&matRotExtra, fExtraAngle);

	if (fExtraMove == 0)
	{
		m_matWorld = matScale *
			matSyncX * matRotX * matReplaceX *
			matSyncY * matRotY * matReplaceY *
			matTrans;
	}
	else
	{
		m_matWorld = matScale *
			matExtraSync * matRotExtra * matExtraReplace *
			matSyncX * matRotX * matReplaceX *
			matSyncY * matRotY * matReplaceY *
			matTrans;
	}
}

void CTransform::Rotation_Axis_Special(const _float & fXMove, const _float & fYMove, const _float & fXAngle, const _float & fYAngle, const _float & fExtraMove, const _float & fExtraAngle)
{
	_matrix matScale, matRotX, matRotY, matTrans;

	_matrix matSyncX, matReplaceX, matSyncY, matReplaceY;

	_matrix matSyncZ;

	_matrix matExtraSync, matExtraReplace, matRotExtra;

	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixTranslation(&matTrans, m_vInfo[INFO_POS].x, m_vInfo[INFO_POS].y, m_vInfo[INFO_POS].z);

	//	X
	D3DXMatrixTranslation(&matSyncX, 0.f, fXMove, 0.f);
	D3DXMatrixTranslation(&matReplaceX, 0.f, -fXMove, 0.f);
	D3DXMatrixRotationX(&matRotX, fXAngle);

	//	Y
	D3DXMatrixTranslation(&matSyncY, fYMove, 0.f, 0.f);
	D3DXMatrixTranslation(&matReplaceY, -fYMove, 0.f, 0.f);
	D3DXMatrixRotationY(&matRotY, fYAngle);

	//	Extra
	D3DXMatrixTranslation(&matExtraSync, 0.f, fExtraMove, 0.f);
	D3DXMatrixTranslation(&matExtraReplace, 0.f, -fExtraMove, 0.f);
	D3DXMatrixRotationZ(&matRotExtra, fExtraAngle);

	if (fExtraMove == 0)
	{
		m_matWorld = matScale *
			matSyncX * matRotX * matReplaceX *
			matSyncY * matRotY * matReplaceY *
			matTrans;
	}
	else
	{
		m_matWorld = matScale *
			matExtraSync * matRotExtra * matExtraReplace *
			matSyncX * matRotX * matReplaceX *
			matSyncY * matRotY * matReplaceY *
			matTrans;
	}
}

void CTransform::Rotation_Axis_Optional(ROTATIONID eID_01, const _float & fMove_01, const _float & fAngle_01, ROTATIONID eID_02, const _float & fMove_02, const _float & fAngle_02, ROTATIONID eID_03, const _float & fMove_03, const _float & fAngle_03, ROTATIONID eID_04, const _float & fMove_04, const _float & fAngle_04)
{
	// 사용할거면 업데이트 후에 돌리기

	_matrix	matScale, matTrans;													//	Scale, Trans는 기존의 것 사용

	_matrix matRot_01, matRot_02, matRot_03, matRot_04;							//	회전 행렬

	_matrix matSyncAxis_01, matSyncAxis_02, matSyncAxis_03, matSyncAxis_04;		//	축 이동

	_matrix	matAsyncAxis_01, matAsyncAxis_02, matAsyncAxis_03, matAsyncAxis_04;	//	축 복귀

	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixTranslation(&matTrans, m_vInfo[INFO_POS].x, m_vInfo[INFO_POS].y, m_vInfo[INFO_POS].z);

	if (eID_02 = ROT_END)
	{
		switch (eID_01)
		{
		case ROT_X:
			D3DXMatrixTranslation(&matSyncAxis_01, 0.f, fMove_01, 0.f);
			D3DXMatrixRotationX(&matRot_01, fAngle_01);
			D3DXMatrixTranslation(&matAsyncAxis_01, 0.f, -fMove_01, 0.f);
			break;
		case ROT_Y:
			D3DXMatrixTranslation(&matSyncAxis_01, fMove_01, 0.f, 0.f);
			D3DXMatrixRotationY(&matRot_01, fAngle_01);
			D3DXMatrixTranslation(&matAsyncAxis_01, -fMove_01, 0.f, 0.f);
			break;
		case ROT_Z:
			D3DXMatrixTranslation(&matSyncAxis_01, 0.f, fMove_01, 0.f);
			D3DXMatrixRotationZ(&matRot_01, fAngle_01);
			D3DXMatrixTranslation(&matAsyncAxis_01, 0.f, -fMove_01, 0.f);
			break;
		}

		m_matWorld = matScale *
			matSyncAxis_01 * matRot_01 * matAsyncAxis_01 *
			matTrans;
	}
	else if (eID_03 == ROT_END)
	{
		switch (eID_01)
		{
		case ROT_X:
			D3DXMatrixTranslation(&matSyncAxis_01, 0.f, fMove_01, 0.f);
			D3DXMatrixRotationX(&matRot_01, fAngle_01);
			D3DXMatrixTranslation(&matAsyncAxis_01, 0.f, -fMove_01, 0.f);
			break;
		case ROT_Y:
			D3DXMatrixTranslation(&matSyncAxis_01, fMove_01, 0.f, 0.f);
			D3DXMatrixRotationY(&matRot_01, fAngle_01);
			D3DXMatrixTranslation(&matAsyncAxis_01, -fMove_01, 0.f, 0.f);
			break;
		case ROT_Z:
			D3DXMatrixTranslation(&matSyncAxis_01, 0.f, fMove_01, 0.f);
			D3DXMatrixRotationZ(&matRot_01, fAngle_01);
			D3DXMatrixTranslation(&matAsyncAxis_01, 0.f, -fMove_01, 0.f);
			break;
		}

		switch (eID_02)
		{
		case ROT_X:
			D3DXMatrixTranslation(&matSyncAxis_02, 0.f, fMove_02, 0.f);
			D3DXMatrixRotationX(&matRot_02, fAngle_02);
			D3DXMatrixTranslation(&matAsyncAxis_02, 0.f, -fMove_02, 0.f);
			break;
		case ROT_Y:
			D3DXMatrixTranslation(&matSyncAxis_02, fMove_02, 0.f, 0.f);
			D3DXMatrixRotationY(&matRot_02, fAngle_02);
			D3DXMatrixTranslation(&matAsyncAxis_02, -fMove_02, 0.f, 0.f);
			break;
		case ROT_Z:
			D3DXMatrixTranslation(&matSyncAxis_02, 0.f, fMove_02, 0.f);
			D3DXMatrixRotationZ(&matRot_02, fAngle_02);
			D3DXMatrixTranslation(&matAsyncAxis_02, 0.f, -fMove_02, 0.f);
			break;
		}

		m_matWorld = matScale *
			matSyncAxis_01 * matRot_01 * matAsyncAxis_01 *
			matSyncAxis_02 * matRot_02 * matAsyncAxis_02 *
			matTrans;
	}
	else if (eID_04 == ROT_END)
	{
		switch (eID_01)
		{
		case ROT_X:
			D3DXMatrixTranslation(&matSyncAxis_01, 0.f, fMove_01, 0.f);
			D3DXMatrixRotationX(&matRot_01, fAngle_01);
			D3DXMatrixTranslation(&matAsyncAxis_01, 0.f, -fMove_01, 0.f);
			break;
		case ROT_Y:
			D3DXMatrixTranslation(&matSyncAxis_01, fMove_01, 0.f, 0.f);
			D3DXMatrixRotationY(&matRot_01, fAngle_01);
			D3DXMatrixTranslation(&matAsyncAxis_01, -fMove_01, 0.f, 0.f);
			break;
		case ROT_Z:
			D3DXMatrixTranslation(&matSyncAxis_01, 0.f, fMove_01, 0.f);
			D3DXMatrixRotationZ(&matRot_01, fAngle_01);
			D3DXMatrixTranslation(&matAsyncAxis_01, 0.f, -fMove_01, 0.f);
			break;
		}

		switch (eID_02)
		{
		case ROT_X:
			D3DXMatrixTranslation(&matSyncAxis_02, 0.f, fMove_02, 0.f);
			D3DXMatrixRotationX(&matRot_02, fAngle_02);
			D3DXMatrixTranslation(&matAsyncAxis_02, 0.f, -fMove_02, 0.f);
			break;
		case ROT_Y:
			D3DXMatrixTranslation(&matSyncAxis_02, fMove_02, 0.f, 0.f);
			D3DXMatrixRotationY(&matRot_02, fAngle_02);
			D3DXMatrixTranslation(&matAsyncAxis_02, -fMove_02, 0.f, 0.f);
			break;
		case ROT_Z:
			D3DXMatrixTranslation(&matSyncAxis_02, 0.f, fMove_02, 0.f);
			D3DXMatrixRotationZ(&matRot_02, fAngle_02);
			D3DXMatrixTranslation(&matAsyncAxis_02, 0.f, -fMove_02, 0.f);
			break;
		}

		switch (eID_03)
		{
		case ROT_X:
			D3DXMatrixTranslation(&matSyncAxis_03, 0.f, fMove_03, 0.f);
			D3DXMatrixRotationX(&matRot_03, fAngle_03);
			D3DXMatrixTranslation(&matAsyncAxis_03, 0.f, -fMove_03, 0.f);
			break;
		case ROT_Y:
			D3DXMatrixTranslation(&matSyncAxis_03, fMove_03, 0.f, 0.f);
			D3DXMatrixRotationY(&matRot_03, fAngle_03);
			D3DXMatrixTranslation(&matAsyncAxis_03, -fMove_03, 0.f, 0.f);
			break;
		case ROT_Z:
			D3DXMatrixTranslation(&matSyncAxis_03, 0.f, fMove_03, 0.f);
			D3DXMatrixRotationZ(&matRot_03, fAngle_03);
			D3DXMatrixTranslation(&matAsyncAxis_03, 0.f, -fMove_03, 0.f);
			break;
		}

		m_matWorld = matScale *
			matSyncAxis_01 * matRot_01 * matAsyncAxis_01 *
			matSyncAxis_02 * matRot_02 * matAsyncAxis_02 *
			matSyncAxis_03 * matRot_03 * matAsyncAxis_03 *
			matTrans;
	}
	else
	{
		switch (eID_01)
		{
		case ROT_X:
			D3DXMatrixTranslation(&matSyncAxis_01, 0.f, fMove_01, 0.f);
			D3DXMatrixRotationX(&matRot_01, fAngle_01);
			D3DXMatrixTranslation(&matAsyncAxis_01, 0.f, -fMove_01, 0.f);
			break;
		case ROT_Y:
			D3DXMatrixTranslation(&matSyncAxis_01, fMove_01, 0.f, 0.f);
			D3DXMatrixRotationY(&matRot_01, fAngle_01);
			D3DXMatrixTranslation(&matAsyncAxis_01, -fMove_01, 0.f, 0.f);
			break;
		case ROT_Z:
			D3DXMatrixTranslation(&matSyncAxis_01, 0.f, fMove_01, 0.f);
			D3DXMatrixRotationZ(&matRot_01, fAngle_01);
			D3DXMatrixTranslation(&matAsyncAxis_01, 0.f, -fMove_01, 0.f);
			break;
		}

		switch (eID_02)
		{
		case ROT_X:
			D3DXMatrixTranslation(&matSyncAxis_02, 0.f, fMove_02, 0.f);
			D3DXMatrixRotationX(&matRot_02, fAngle_02);
			D3DXMatrixTranslation(&matAsyncAxis_02, 0.f, -fMove_02, 0.f);
			break;
		case ROT_Y:
			D3DXMatrixTranslation(&matSyncAxis_02, fMove_02, 0.f, 0.f);
			D3DXMatrixRotationY(&matRot_02, fAngle_02);
			D3DXMatrixTranslation(&matAsyncAxis_02, -fMove_02, 0.f, 0.f);
			break;
		case ROT_Z:
			D3DXMatrixTranslation(&matSyncAxis_02, 0.f, fMove_02, 0.f);
			D3DXMatrixRotationZ(&matRot_02, fAngle_02);
			D3DXMatrixTranslation(&matAsyncAxis_02, 0.f, -fMove_02, 0.f);
			break;
		}

		switch (eID_03)
		{
		case ROT_X:
			D3DXMatrixTranslation(&matSyncAxis_03, 0.f, fMove_03, 0.f);
			D3DXMatrixRotationX(&matRot_03, fAngle_03);
			D3DXMatrixTranslation(&matAsyncAxis_03, 0.f, -fMove_03, 0.f);
			break;
		case ROT_Y:
			D3DXMatrixTranslation(&matSyncAxis_03, fMove_03, 0.f, 0.f);
			D3DXMatrixRotationY(&matRot_03, fAngle_03);
			D3DXMatrixTranslation(&matAsyncAxis_03, -fMove_03, 0.f, 0.f);
			break;
		case ROT_Z:
			D3DXMatrixTranslation(&matSyncAxis_03, 0.f, fMove_03, 0.f);
			D3DXMatrixRotationZ(&matRot_03, fAngle_03);
			D3DXMatrixTranslation(&matAsyncAxis_03, 0.f, -fMove_03, 0.f);
			break;
		}

		switch (eID_04)
		{
		case ROT_X:
			D3DXMatrixTranslation(&matSyncAxis_04, 0.f, fMove_04, 0.f);
			D3DXMatrixRotationX(&matRot_04, fAngle_04);
			D3DXMatrixTranslation(&matAsyncAxis_04, 0.f, -fMove_04, 0.f);
			break;
		case ROT_Y:
			D3DXMatrixTranslation(&matSyncAxis_04, fMove_04, 0.f, 0.f);
			D3DXMatrixRotationY(&matRot_04, fAngle_04);
			D3DXMatrixTranslation(&matAsyncAxis_04, -fMove_04, 0.f, 0.f);
			break;
		case ROT_Z:
			D3DXMatrixTranslation(&matSyncAxis_04, 0.f, fMove_04, 0.f);
			D3DXMatrixRotationZ(&matRot_04, fAngle_04);
			D3DXMatrixTranslation(&matAsyncAxis_04, 0.f, -fMove_04, 0.f);
			break;
		}

		m_matWorld = matScale *
			matSyncAxis_01 * matRot_01 * matAsyncAxis_01 *
			matSyncAxis_02 * matRot_02 * matAsyncAxis_02 *
			matSyncAxis_03 * matRot_03 * matAsyncAxis_03 *
			matSyncAxis_04 * matRot_04 * matAsyncAxis_04 *
			matTrans;

	}
}

void CTransform::Rotation_Axis_Except_Scale(_vec3 * vScale, const _float & fXMove, const _float & fYMove, const _float & fXAngle, const _float & fYAngle, const _float & fExtraMove, const _float & fExtraAngle)
{
	_matrix matScale, matRotX, matRotY, matTrans;

	_matrix matSyncX, matReplaceX, matSyncY, matReplaceY;

	_matrix matSyncZ;

	_matrix matExtraSync, matExtraReplace, matRotExtra;

	D3DXMatrixScaling(&matScale, vScale->x, vScale->y, vScale->z);
	D3DXMatrixTranslation(&matTrans, m_vInfo[INFO_POS].x, m_vInfo[INFO_POS].y, m_vInfo[INFO_POS].z);

	//	X
	D3DXMatrixTranslation(&matSyncX, 0.f, fXMove, 0.f);
	D3DXMatrixTranslation(&matReplaceX, 0.f, -fXMove, 0.f);
	D3DXMatrixRotationX(&matRotX, fXAngle);

	//	Y
	D3DXMatrixTranslation(&matSyncY, fYMove, 0.f, 0.f);
	D3DXMatrixTranslation(&matReplaceY, -fYMove, 0.f, 0.f);
	D3DXMatrixRotationY(&matRotY, fYAngle);

	//	Extra
	D3DXMatrixTranslation(&matExtraSync, 0.f, fExtraMove, 0.f);
	D3DXMatrixTranslation(&matExtraReplace, 0.f, -fExtraMove, 0.f);
	D3DXMatrixRotationX(&matRotExtra, fExtraAngle);

	if (fExtraMove == 0)
	{
		m_matWorld = matScale *
			matSyncX * matRotX * matReplaceX *
			matSyncY * matRotY * matReplaceY *
			matTrans;
	}
	else
	{
		m_matWorld = matScale *
			matExtraSync * matRotExtra * matExtraReplace *
			matSyncX * matRotX * matReplaceX *
			matSyncY * matRotY * matReplaceY *
			matTrans;
	}
}

void CTransform::Static_Update(void)
{
	//	Static 속성인 Transform의 경우 임의적으로 업데이트 1회 진행하는 코드
	_matrix matScale, matRotX, matRotY, matRotZ, matTrans;

	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);

	D3DXMatrixRotationX(&matRotX, m_vAngle.x);
	D3DXMatrixRotationY(&matRotY, m_vAngle.y);
	D3DXMatrixRotationZ(&matRotZ, m_vAngle.z);

	D3DXMatrixTranslation(&matTrans, m_vInfo[INFO_POS].x, m_vInfo[INFO_POS].y, m_vInfo[INFO_POS].z);

	m_matWorld = matScale * matRotX * matRotY * matRotZ * matTrans;
}

void Engine::CTransform::Chase_Target(const _vec3* pTargetPos, const _float& fSpeed, const _float& fTimeDelta)
{
	_vec3		vDir = *pTargetPos - m_vInfo[INFO_POS];

	m_vInfo[INFO_POS] += *D3DXVec3Normalize(&vDir, &vDir) * fSpeed * fTimeDelta;

	_matrix		matRot = *Compute_LookAtTarget(pTargetPos);
	_matrix		matScale, matTrans;

	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixTranslation(&matTrans, m_vInfo[INFO_POS].x, m_vInfo[INFO_POS].y, m_vInfo[INFO_POS].z);

	m_matWorld = matScale * matRot * matTrans;
}

const _matrix* Engine::CTransform::Compute_LookAtTarget(const _vec3* pTargetPos)
{
	_vec3		vLook = *pTargetPos - m_vInfo[INFO_POS];

	_vec3		vAxis, vzLook;
	_matrix		matRot;
	
	// D3DXMatrixRotationAxis : 임의의 축회전 행렬을 만들어주는 함수
	return D3DXMatrixRotationAxis(&matRot, 
									D3DXVec3Cross(&vAxis, &m_vInfo[INFO_LOOK], &vLook),
									acosf(D3DXVec3Dot(D3DXVec3Normalize(&vLook, &vLook), 
												D3DXVec3Normalize(&vzLook, &m_vInfo[INFO_LOOK]))));
}

void CTransform::Chase_Target_By_Direction(_vec3 * vDirection, const _float & fSpeed, const _float & fTimeDelta)
{
	m_vInfo[INFO_POS] += *D3DXVec3Normalize(vDirection, vDirection) * fSpeed * fTimeDelta;

	_matrix		matRot = *LookAtTarget_By_Direction(vDirection);
	_matrix		matScale, matTrans;

	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixTranslation(&matTrans, m_vInfo[INFO_POS].x, m_vInfo[INFO_POS].y, m_vInfo[INFO_POS].z);

	m_matWorld = matScale * matRot * matTrans;
}

const _matrix * CTransform::LookAtTarget_By_Direction(_vec3 * vDirection)
{
	_vec3		vAxis, vzLook;
	_matrix		matRot;

	// D3DXMatrixRotationAxis : 임의의 축회전 행렬을 만들어주는 함수
	return D3DXMatrixRotationAxis(&matRot,
		D3DXVec3Cross(&vAxis, &m_vInfo[INFO_LOOK], vDirection),
		acosf(D3DXVec3Dot(D3DXVec3Normalize(vDirection, vDirection),
			D3DXVec3Normalize(&vzLook, &m_vInfo[INFO_LOOK]))));

}

void CTransform::Billboard_Transform(const _float & fTimeDelta)
{
	//	이거 쓸라면 Transform Component 속성 Static 이어야됨!
	_matrix	matScale, matRotX, matRotY, matRotZ, matBill, matTrans;

	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixRotationX(&matRotX, m_vAngle.x);
	D3DXMatrixRotationY(&matRotY, m_vAngle.y);
	D3DXMatrixRotationZ(&matRotZ, m_vAngle.z);

	D3DXMatrixIdentity(&matBill);

	_matrix matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	matBill.m[0][0] = matView.m[0][0];
	matBill.m[0][2] = matView.m[0][2];
	matBill.m[2][0] = matView.m[2][0];
	matBill.m[2][2] = matView.m[2][2];

	D3DXMatrixInverse(&matBill, nullptr, &matBill);

	D3DXMatrixTranslation(&matTrans, m_vInfo[INFO_POS].x, m_vInfo[INFO_POS].y, m_vInfo[INFO_POS].z);

	m_matWorld = matScale * matRotX * matRotY * matRotZ * matBill * matTrans;
}

HRESULT CTransform::Ready_Transform(void)
{
	D3DXMatrixIdentity(&m_matWorld);

	for (_uint i = 0; i < INFO_END; ++i)
		memcpy(&m_vInfo[i], &m_matWorld.m[i][0], sizeof(_vec3));
	
	return S_OK;
}

_int CTransform::Update_Component(const _float & fTimeDelta)
{
	D3DXMatrixIdentity(&m_matWorld);

	// 크기
	for (_uint i = 0; i < INFO_POS; ++i)
	{
		memcpy(&m_vInfo[i], &m_matWorld.m[i][0], sizeof(_vec3));
	}

	for(_uint i = 0; i < INFO_POS; ++i)
	{
		D3DXVec3Normalize(&m_vInfo[i], &m_vInfo[i]);
		m_vInfo[i] *= *(((_float*)&m_vScale) + i);
	}
	
	// 회전
	_matrix		matRot[ROT_END];
	D3DXMatrixRotationX(&matRot[ROT_X], m_vAngle.x);
	D3DXMatrixRotationY(&matRot[ROT_Y], m_vAngle.y);
	D3DXMatrixRotationZ(&matRot[ROT_Z], m_vAngle.z);

	for (_uint i = 0; i < INFO_POS; ++i)
	{
		for (_uint j = 0; j < ROT_END; ++j)
		{
			D3DXVec3TransformNormal(&m_vInfo[i], &m_vInfo[i], &matRot[j]);
		}
	}

	
	for (_uint i = 0; i < INFO_END; ++i)
	{
		memcpy(&m_matWorld.m[i][0], &m_vInfo[i], sizeof(_vec3));
	}
	
	return 0;
}

CTransform * CTransform::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTransform* pInstance = new CTransform(pGraphicDev);
	if (FAILED(pInstance->Ready_Transform()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CComponent* Engine::CTransform::Clone(void)
{
	return new CTransform(*this);
}

void CTransform::Free(void)
{
	CComponent::Free();
}
