#include "stdafx.h"
#include "..\Header\Npc.h"
#include "StaticCamera.h"
#include "ItemBox.h"
#include "MiddleBoss.h"
#include "ProjectionEffect.h"
#include "TargetPointEffect.h"
#include "Flight.h"

CNpc::CNpc(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CNpc::~CNpc()
{
}

HRESULT CNpc::Ready_Object(const _vec3& vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	_vec3 vScale = { 0.f, 0.f, 0.f };
	m_pTransCom->Set_Scale(&vScale);
	m_pTransCom->Set_Pos(vPos.x,vPos.y,vPos.z);

	return S_OK;
}

_int CNpc::Update_Object(const _float & fTimeDelta)
{
	CStaticCamera* pCam = dynamic_cast<CStaticCamera*>(Get_GameObject(STAGE_ENVIRONMENT, L"StaticCamera"));
	CItemBox* pQuestBox1 = dynamic_cast<CItemBox*>(Get_GameObject(L"STAGE_QUESTBOX", L"ItemBox1"));
	CItemBox* pQuestBox2 = dynamic_cast<CItemBox*>(Get_GameObject(L"STAGE_QUESTBOX", L"ItemBox2"));

	if (Get_GameObject(L"STAGE_BOSS", L"MiddleBoss") != nullptr)
	{
		m_bBossDead = Get_GameObject(L"STAGE_BOSS", L"MiddleBoss")->Get_Dead();
	}

	if (pCam != nullptr && pCam->Get_bNpc() && m_bQuest1)
	{
		Quest1(fTimeDelta);
	}
	else if (pQuestBox1->Get_QuestOpen() && m_bQuest2)
	{
		Quest2(fTimeDelta);
	}
	else if (pQuestBox2->Get_QuestOpen() && m_bQuest3)
	{
		Quest3(fTimeDelta);
	}
	else if (Get_Layer(L"STAGE_BOSS")->Get_GameObjectMap().size() == 0 && m_bQuest3 == false && m_bQuest4)
	{
		Finish(fTimeDelta);
	}

	Projection_Effect();
	Projection_Sound();
	return 0;
}

void CNpc::Render_Object(void)
{
	_matrix      matWorld, matView, matProj;

	matWorld = *m_pTransCom->Get_WorldMatrixPointer();
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	D3DXMatrixTranspose(&matWorld, &matWorld);
	D3DXMatrixTranspose(&matView, &matView);
	D3DXMatrixTranspose(&matProj, &matProj);

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &matWorld, sizeof(_matrix))))
		return;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &matView, sizeof(_matrix))))
		return;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &matProj, sizeof(_matrix))))
		return;

	if (m_bQuest1)
	{
		_uint iTexindex = _uint(m_fTexFrame);
		m_pQuest1TexCom->Set_Texture(m_pShaderCom, "g_DefaultTexture", iTexindex);

	}
	else if (m_bQuest2)
	{
		_uint iTexindex = _uint(m_fTexFrame);
		m_pQuest2TexCom->Set_Texture(m_pShaderCom, "g_DefaultTexture", iTexindex);
	}
	else if (m_bQuest3)
	{
		_uint iTexindex = _uint(m_fTexFrame);
		m_pQuest3TexCom->Set_Texture(m_pShaderCom, "g_DefaultTexture", iTexindex);
	}
	else if (m_bQuest4)
	{
		_uint iTexindex = _uint(m_fTexFrame);
		m_pQuest4TexCom->Set_Texture(m_pShaderCom, "g_DefaultTexture", iTexindex);
	}

	m_pShaderCom->Begin_Shader(1);
	m_pBufferCom->Render_Buffer();
	m_pShaderCom->End_Shader();
}

HRESULT CNpc::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(RCTEX_COMP));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ RCTEX_COMP, pComponent });

	// Quest1 모음집
	pComponent = m_pQuest1TexCom = dynamic_cast<CTexture*>(Clone_Proto(L"LetterBox_Tex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"LetterBox_Tex", pComponent });

	// Quest2 모음집
	pComponent = m_pQuest2TexCom = dynamic_cast<CTexture*>(Clone_Proto(L"LetterBox_Tex_2"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"LetterBox_Tex_2", pComponent });

	// Quest3 모음집
	pComponent = m_pQuest3TexCom = dynamic_cast<CTexture*>(Clone_Proto(L"LetterBox_Tex_3"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"LetterBox_Tex_3", pComponent });
	
	// Quest4 모음집
	pComponent = m_pQuest4TexCom = dynamic_cast<CTexture*>(Clone_Proto(L"LetterBox_Tex_4"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"LetterBox_Tex_4", pComponent });

	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Clone_Proto(TRANSFORM_COMP));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ TRANSFORM_COMP, pComponent });

	pComponent = m_pShaderCom = dynamic_cast<CShader*>(Clone_Proto(RCTEX_SHADER));
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ RCTEX_SHADER, pComponent });

	return S_OK;
}

void CNpc::Quest1(const _float& fTimeDelta)
{
	CTransform* pPlayer = dynamic_cast<CTransform*>(Engine::Get_Component(STAGE_CHARACTER, L"HEAD", TRANSFORM_COMP, ID_DYNAMIC));
	_vec3 vPlayerLook;
	pPlayer->Get_Info(INFO_LOOK, &vPlayerLook);
	_vec3 vPlayerPos;
	pPlayer->Get_Info(INFO_POS, &vPlayerPos);
	if (!m_bInit)
	{
		
		m_vPlayerPos = vPlayerPos;
		D3DXVec3Normalize(&vPlayerLook, &vPlayerLook);
		m_vPlayerPos.x += vPlayerLook.x * 10.f;
		m_vPlayerPos.y += vPlayerLook.y * 10.f + 2.f;
		m_vPlayerPos.z += vPlayerLook.z * 10.f;

		m_bInit = true;
	}

	if (m_fTransFrame0 <= 1.f)
	{
		m_bEffect1 = true;
	
		m_bQuest1 = true;															//켜주는곳

		m_fTransFrame0 += fTimeDelta * 0.2f;
		_vec3 vTransLerp;
		D3DXVec3Lerp(&vTransLerp, &vPlayerPos, &m_vPlayerPos, m_fTransFrame0);
		m_pTransCom->Set_Pos(vTransLerp.x, vTransLerp.y, vTransLerp.z);
		CGameObject::Update_Object(fTimeDelta);
		m_pTransCom->Billboard_Transform(fTimeDelta);
		Add_RenderGroup(RENDER_EFFECT_UI, this);
	}
	else if (m_fTransFrame0 > 0.9f && m_fTexFrame < 52.f)
	{
		m_fScaleFrame0 += fTimeDelta;
		if (m_fScaleFrame0 <= 1.f)
		{
			_vec3 vScaleLerp;
			D3DXVec3Lerp(&vScaleLerp, &_vec3(0.f, 0.f, 0.f), &m_vScale, m_fScaleFrame0);
			m_pTransCom->Set_Scale(&vScaleLerp);
		}
		m_fTexFrame += fTimeDelta * 3.f;
		CGameObject::Update_Object(fTimeDelta);
		m_pTransCom->Billboard_Transform(fTimeDelta);
		Add_RenderGroup(RENDER_EFFECT_UI, this);
	}
	else if (m_fTexFrame >= 52.f)
	{
		m_fTexFrame = 52.f;
		m_fScaleFrame1 += fTimeDelta;
		if (m_fScaleFrame1 <= 1.f)
		{
			_vec3 vScaleLerp;
			D3DXVec3Lerp(&vScaleLerp, &m_vScale, &_vec3(0.f, 0.f, 0.f), m_fScaleFrame1);
			m_pTransCom->Set_Scale(&vScaleLerp);
			CGameObject::Update_Object(fTimeDelta);
			m_pTransCom->Billboard_Transform(fTimeDelta);
			Add_RenderGroup(RENDER_EFFECT_UI, this);
		}
		else
		{																				//꺼주는곳
			m_bSound1 = true;

			m_bQuest1 = false;
			m_bInit = false;
			m_bQuestText1 = true;
			m_bEffect1 = false;
			m_fTexFrame = 0.f;
			m_fTransFrame0 = 0.f;
			m_fScaleFrame0 = 0.f;
			m_fScaleFrame1 = 0.f;
		}
	}
}

void CNpc::Quest2(const _float & fTimeDelta)
{
	CTransform* pPlayer = dynamic_cast<CTransform*>(Engine::Get_Component(STAGE_CHARACTER, L"HEAD", TRANSFORM_COMP, ID_DYNAMIC));
	_vec3 vPlayerLook;
	pPlayer->Get_Info(INFO_LOOK, &vPlayerLook);
	_vec3 vPlayerPos;
	pPlayer->Get_Info(INFO_POS, &vPlayerPos);
	if (!m_bInit)
	{
		
		m_vPlayerPos = vPlayerPos;
		D3DXVec3Normalize(&vPlayerLook, &vPlayerLook);
		m_vPlayerPos.x += vPlayerLook.x * 1.f;
		m_vPlayerPos.y += vPlayerLook.y * 1.f + 2.5f;
		m_vPlayerPos.z += vPlayerLook.z * 1.f;

		m_bInit = true;
	}

	if (m_fTransFrame0 <= 1.f)
	{
		m_bEffect2 = true;

		m_bQuest2 = true;
		m_fTransFrame0 += fTimeDelta * 0.2f;
		_vec3 vTransLerp;
		D3DXVec3Lerp(&vTransLerp, &vPlayerPos, &m_vPlayerPos, m_fTransFrame0);
		m_pTransCom->Set_Pos(vTransLerp.x, vTransLerp.y, vTransLerp.z);
		CGameObject::Update_Object(fTimeDelta);
		m_pTransCom->Billboard_Transform(fTimeDelta);
		Add_RenderGroup(RENDER_EFFECT_UI, this);
	}
	else if (m_fTransFrame0 > 0.9f && m_fTexFrame < 60.f)
	{
		m_fScaleFrame0 += fTimeDelta;
		if (m_fScaleFrame0 <= 1.f)
		{
			_vec3 vScaleLerp;
			D3DXVec3Lerp(&vScaleLerp, &_vec3(0.f, 0.f, 0.f), &m_vScale, m_fScaleFrame0);
			m_pTransCom->Set_Scale(&vScaleLerp);
		}
		m_fTexFrame += fTimeDelta * 3.f;
		CGameObject::Update_Object(fTimeDelta);
		m_pTransCom->Billboard_Transform(fTimeDelta);
		Add_RenderGroup(RENDER_EFFECT_UI, this);
	}
	else if (m_fTexFrame >= 60.f)
	{
		m_fTexFrame = 60.f;
		m_fScaleFrame1 += fTimeDelta;
		if (m_fScaleFrame1 <= 1.f)
		{
			_vec3 vScaleLerp;
			D3DXVec3Lerp(&vScaleLerp, &m_vScale, &_vec3(0.f, 0.f, 0.f), m_fScaleFrame1);
			m_pTransCom->Set_Scale(&vScaleLerp);
			CGameObject::Update_Object(fTimeDelta);
			m_pTransCom->Billboard_Transform(fTimeDelta);
			Add_RenderGroup(RENDER_EFFECT_UI, this);
		}
		else
		{
			m_bSound1 = true;

			m_bEffect2 = false;
			m_bQuest2 = false;
			m_bInit = false;
			m_bQuestText2 = true;
			m_fTexFrame = 0.f;
			m_fTransFrame0 = 0.f;
			m_fScaleFrame0 = 0.f;
			m_fScaleFrame1 = 0.f;
		}
	}
}

void CNpc::Quest3(const _float & fTimeDelta)
{
	CTransform* pPlayer = dynamic_cast<CTransform*>(Engine::Get_Component(STAGE_CHARACTER, L"HEAD", TRANSFORM_COMP, ID_DYNAMIC));
	_vec3 vPlayerLook;
	pPlayer->Get_Info(INFO_LOOK, &vPlayerLook);
	_vec3 vPlayerPos;
	pPlayer->Get_Info(INFO_POS, &vPlayerPos);
	if (!m_bInit)
	{

		m_vPlayerPos = vPlayerPos;
		D3DXVec3Normalize(&vPlayerLook, &vPlayerLook);
		m_vPlayerPos.x += vPlayerLook.x * 1.f;
		m_vPlayerPos.y += vPlayerLook.y * 1.f + 2.5f;
		m_vPlayerPos.z += vPlayerLook.z * 1.f;

		m_bInit = true;
	}

	if (m_fTransFrame0 <= 1.f)
	{
		m_bEffect3 = true;
		m_bQuest3 = true;
		m_fTransFrame0 += fTimeDelta * 0.2f;
		_vec3 vTransLerp;
		D3DXVec3Lerp(&vTransLerp, &vPlayerPos, &m_vPlayerPos, m_fTransFrame0);
		m_pTransCom->Set_Pos(vTransLerp.x, vTransLerp.y, vTransLerp.z);
		CGameObject::Update_Object(fTimeDelta);
		m_pTransCom->Billboard_Transform(fTimeDelta);
		Add_RenderGroup(RENDER_EFFECT_UI, this);
	}
	else if (m_fTransFrame0 > 0.9f && m_fTexFrame < 35.f)
	{
		m_fScaleFrame0 += fTimeDelta;
		if (m_fScaleFrame0 <= 1.f)
		{
			_vec3 vScaleLerp;
			D3DXVec3Lerp(&vScaleLerp, &_vec3(0.f, 0.f, 0.f), &m_vScale, m_fScaleFrame0);
			m_pTransCom->Set_Scale(&vScaleLerp);
		}
		m_fTexFrame += fTimeDelta * 3.f;
		CGameObject::Update_Object(fTimeDelta);
		m_pTransCom->Billboard_Transform(fTimeDelta);
		Add_RenderGroup(RENDER_EFFECT_UI, this);
	}
	else if (m_fTexFrame >= 35.f)
	{
		m_fTexFrame = 35.f;
		m_fScaleFrame1 += fTimeDelta;
		if (m_fScaleFrame1 <= 1.f)
		{
			_vec3 vScaleLerp;
			D3DXVec3Lerp(&vScaleLerp, &m_vScale, &_vec3(0.f, 0.f, 0.f), m_fScaleFrame1);
			m_pTransCom->Set_Scale(&vScaleLerp);
			CGameObject::Update_Object(fTimeDelta);
			m_pTransCom->Billboard_Transform(fTimeDelta);
			Add_RenderGroup(RENDER_EFFECT_UI, this);
		}
		else
		{
			m_bSound1 = true;

			m_bEffect3 = false;
			m_bQuest3 = false;
			m_bInit = false;
			m_bQuestText3 = true;
			m_fTexFrame = 0.f;
			m_fTransFrame0 = 0.f;
			m_fScaleFrame0 = 0.f;
			m_fScaleFrame1 = 0.f;
		}
	}
}

void CNpc::Finish(const _float & fTimeDelta)
{
	CTransform* pPlayer = dynamic_cast<CTransform*>(Engine::Get_Component(STAGE_CHARACTER, L"HEAD", TRANSFORM_COMP, ID_DYNAMIC));
	_vec3 vPlayerLook;
	pPlayer->Get_Info(INFO_LOOK, &vPlayerLook);
	_vec3 vPlayerPos;
	pPlayer->Get_Info(INFO_POS, &vPlayerPos);
	if (!m_bInit)
	{
		m_vPlayerPos = vPlayerPos;
		D3DXVec3Normalize(&vPlayerLook, &vPlayerLook);
		m_vPlayerPos.x += vPlayerLook.x * 1.f;
		m_vPlayerPos.y += vPlayerLook.y * 1.f + 2.5f;
		m_vPlayerPos.z += vPlayerLook.z * 1.f;

		m_bInit = true;
	}

	if (m_fTransFrame0 <= 1.f)
	{
		m_bQuest4 = true;
		m_fTransFrame0 += fTimeDelta * 0.2f;
		_vec3 vTransLerp;
		D3DXVec3Lerp(&vTransLerp, &vPlayerPos, &m_vPlayerPos, m_fTransFrame0);
		m_pTransCom->Set_Pos(vTransLerp.x, vTransLerp.y, vTransLerp.z);
		CGameObject::Update_Object(fTimeDelta);
		m_pTransCom->Billboard_Transform(fTimeDelta);
		Add_RenderGroup(RENDER_EFFECT_UI, this);
	}
	else if (m_fTransFrame0 > 0.9f && m_fTexFrame < 61.f)
	{
		m_fScaleFrame0 += fTimeDelta;
		if (m_fScaleFrame0 <= 1.f)
		{
			_vec3 vScaleLerp;
			D3DXVec3Lerp(&vScaleLerp, &_vec3(0.f, 0.f, 0.f), &m_vScale, m_fScaleFrame0);
			m_pTransCom->Set_Scale(&vScaleLerp);
		}
		m_fTexFrame += fTimeDelta * 3.f;
		CGameObject::Update_Object(fTimeDelta);
		m_pTransCom->Billboard_Transform(fTimeDelta);
		Add_RenderGroup(RENDER_EFFECT_UI, this);
	}
	else if (m_fTexFrame >= 61.f)
	{
		m_fTexFrame = 61.f;
		m_fScaleFrame1 += fTimeDelta;
		if (m_fScaleFrame1 <= 1.f)
		{
			_vec3 vScaleLerp;
			D3DXVec3Lerp(&vScaleLerp, &m_vScale, &_vec3(0.f, 0.f, 0.f), m_fScaleFrame1);
			m_pTransCom->Set_Scale(&vScaleLerp);
			CGameObject::Update_Object(fTimeDelta);
			m_pTransCom->Billboard_Transform(fTimeDelta);
			Add_RenderGroup(RENDER_EFFECT_UI, this);
		}
		else
		{
			m_bSound1 = true;
			m_bEffect4 = false;
			m_bQuest4 = false;
			m_bInit = false;
			m_bQuestText4 = true;
			m_fTexFrame = 0.f;
			m_fTransFrame0 = 0.f;
			m_fScaleFrame0 = 0.f;
			m_fScaleFrame1 = 0.f;

			CGameObject* pGameObject = dynamic_cast<CFlight*>(Get_GameObject(STAGE_FLIGHTPLAYER, L"FLIGHTSHUTTLE"));
			if (dynamic_cast<CFlight*>(pGameObject)->Get_Ending() == false)
				dynamic_cast<CFlight*>(pGameObject)->Set_Ending(true);
		}
	}
}

void CNpc::Projection_Effect(void)
{
	if (m_bQuest1)
	{		
		CProjectionEffect* m_pProjectionEffect = nullptr;
		if (!m_pProjectionEffect)
		m_pProjectionEffect = dynamic_cast<CProjectionEffect*>(Engine::Get_GameObject(STAGE_ENVIRONMENT, L"ProjectionEffect"));
		
		_vec3 vPos;													//대쉬이펙트하려던것		
		_vec3 vRootPos;
		_vec3 vDir;
		_vec3 min = { -1.0f ,-1.0f ,-1.0f };
		_vec3 max = { 1.0f ,1.0f ,.0f };		
		m_pTransCom->Get_Info(INFO_POS, &vPos);
		vPos.z += 12.5f;
		vRootPos = vPos;
		vRootPos.y = 10.f;
		vRootPos.z = 55.f;
		for (_int i = -7; i < 7; i++)
		{
			for (_int j = -7; j < 7; j++)
			{
				for (_int k = -7; k < 0; k++)
				{
					//D3DXVec3Normalize(&min, &_vec3(i, j, k));
		
					dynamic_cast<CProjectionEffect*>(m_pProjectionEffect)->Set_PclePos(vRootPos+ _vec3(i, j, k)*0.005f);
					dynamic_cast<CProjectionEffect*>(m_pProjectionEffect)->Set_PcleDir(_vec3(i, j, k)*0.15f);
					dynamic_cast<CProjectionEffect*>(m_pProjectionEffect)->Set_PcleMoveDir((vPos- vRootPos)*0.1f);
					m_pProjectionEffect->addParticle();		
				}
			}
		}
	}
	if (m_bEffect2)
	{
		CTargetPointEffect* m_pTargetPointEffect = nullptr;
		if (!m_pTargetPointEffect)
		m_pTargetPointEffect = dynamic_cast<CTargetPointEffect*>(Engine::Get_GameObject(STAGE_ENVIRONMENT, L"TargetPointEffect"));
		
		_vec3 vPos;														//대쉬이펙트하려던것		
		_vec3 vDir;
		_vec3 min = { -1.0f ,-1.0f ,-1.0f };
		_vec3 max = { 1.0f ,1.0f ,1.0f };
		
		m_pTransCom->Get_Info(INFO_POS, &vPos);
		vPos.z -= 2.f;
		for (_int i = -5; i < 5; i++)
		{
			for (_int j = -5; j < 5; j++)
			{
				for (_int k = -5; k < 5; k++)
				{
					//D3DXVec3Normalize(&min, &_vec3(i, j, k));								
					dynamic_cast<CTargetPointEffect*>(m_pTargetPointEffect)->Set_PclePos(vPos + _vec3(i, j, k)*0.1f);		
					dynamic_cast<CTargetPointEffect*>(m_pTargetPointEffect)->Set_PcleDir(_vec3(i, j, k)*1.f);
					dynamic_cast<CTargetPointEffect*>(m_pTargetPointEffect)->Set_PcleMoveDir(max);		
					m_pTargetPointEffect->addParticle();		
				}
			}
		}
	}
	if (m_bEffect3)
	{
		CTargetPointEffect* m_pTargetPointEffect = nullptr;
		if (!m_pTargetPointEffect)
			m_pTargetPointEffect = dynamic_cast<CTargetPointEffect*>(Engine::Get_GameObject(STAGE_ENVIRONMENT, L"TargetPointEffect"));

		_vec3 vPos;														//대쉬이펙트하려던것		
		_vec3 vDir;
		_vec3 min = { -1.0f ,-1.0f ,-1.0f };
		_vec3 max = { 1.0f ,1.0f ,1.0f };

		m_pTransCom->Get_Info(INFO_POS, &vPos);
		vPos.z -= 2.f;
		for (_int i = -5; i < 5; i++)
		{
			for (_int j = -5; j < 5; j++)
			{
				for (_int k = -5; k < 5; k++)
				{
					//D3DXVec3Normalize(&min, &_vec3(i, j, k));								
					dynamic_cast<CTargetPointEffect*>(m_pTargetPointEffect)->Set_PclePos(vPos + _vec3(i, j, k)*0.1f);
					dynamic_cast<CTargetPointEffect*>(m_pTargetPointEffect)->Set_PcleDir(_vec3(i, j, k)*1.f);
					dynamic_cast<CTargetPointEffect*>(m_pTargetPointEffect)->Set_PcleMoveDir(max);
					m_pTargetPointEffect->addParticle();
				}
			}
		}
	}
	if (m_bEffect4)
	{
		if (!m_bSound3)
		{
			m_bSound3 = true;
			_float fSound = 1.f;
			Engine::PlaySoundGun(L"NPC.wav", SOUND_REFLECT, fSound);
		}
		CTargetPointEffect* m_pTargetPointEffect = nullptr;
		if (!m_pTargetPointEffect)
			m_pTargetPointEffect = dynamic_cast<CTargetPointEffect*>(Engine::Get_GameObject(STAGE_ENVIRONMENT, L"TargetPointEffect"));

		_vec3 vPos;														//대쉬이펙트하려던것		
		_vec3 vDir;
		_vec3 min = { -1.0f ,-1.0f ,-1.0f };
		_vec3 max = { 1.0f ,1.0f ,1.0f };

		m_pTransCom->Get_Info(INFO_POS, &vPos);
		vPos.z -= 2.f;
		for (_int i = -5; i < 5; i++)
		{
			for (_int j = -5; j < 5; j++)
			{
				for (_int k = -5; k < 5; k++)
				{
					//D3DXVec3Normalize(&min, &_vec3(i, j, k));								
					dynamic_cast<CTargetPointEffect*>(m_pTargetPointEffect)->Set_PclePos(vPos + _vec3(i, j, k)*0.1f);
					dynamic_cast<CTargetPointEffect*>(m_pTargetPointEffect)->Set_PcleDir(_vec3(i, j, k)*1.f);
					dynamic_cast<CTargetPointEffect*>(m_pTargetPointEffect)->Set_PcleMoveDir(max);
					m_pTargetPointEffect->addParticle();
				}
			}
		}
	}
}

void CNpc::Projection_Sound(void)
{
	if(m_bSound1)
	{
		_float fSound = 1.f;
		Engine::PlaySoundGun(L"Mission.wav", SOUND_EFFECT, fSound);
		m_bSound1 = false;
	}
	


}

CNpc * CNpc::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos)
{
	CNpc*	pInstance = new CNpc(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CNpc::Free()
{
	CGameObject::Free();
}
