#include "stdafx.h"
#include "..\Header\LogoBilboard.h"


CLogoBilboard::CLogoBilboard(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}


CLogoBilboard::~CLogoBilboard()
{
}

HRESULT CLogoBilboard::Ready_Object()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransCom->Set_Scale(2.3f, 5.6f, 0.f);
	m_pTransCom->Set_Pos(45.f, 0.6f, 45.f);


	//비석
	m_pMeMorialTransCom->Set_Scale(2.5, 5.6f, 0.f);
	m_pMeMorialTransCom->Set_Pos(55.f, 2.f, 55.f);

	//야누비스
	m_pAnubisTransCom->Set_Scale(3.f, 5.6f, 0.f);
	m_pAnubisTransCom->Set_Pos(45.f, 2.f, 55.f);


	m_pMageTransCom->Set_Scale(2.3f, 5.f, 0.f);
	m_pMageTransCom->Set_Pos(38.f, 2.f, 52.f);

	m_pSocerTransCom->Set_Scale(3.f, 5.6f, 0.f);
	m_pSocerTransCom->Set_Pos(41.f, 2.f, 55.f);


	m_pTreeTransCom->Set_Scale(30.f, 13.f, 1.f);
	m_pTreeTransCom->Set_Pos(48.f, 2.f, 63.f);

	m_pNewTransCom->Set_Scale(8.3f, 5.6f, 0.f);
	m_pNewTransCom->Set_Pos(51.f, 2.f, 55.f);

	m_pGlcierTransCom->Set_Scale(2.5f, 7.5f, 0.f);
	m_pGlcierTransCom->Set_Pos(54.f, 2.f, 55.f);


	m_pProTransCom->Set_Scale(2.5f, 5.5f, 0.f);
	m_pProTransCom->Set_Pos(45.f, 2.f, 55.f);

	m_pHandTransCom->Set_Scale(2.5f, 5.5f, 0.f);
	m_pHandTransCom->Set_Pos(45.f, 2.f, 55.f);


	//이집트 나무 
	m_pEgyptTransCom->Set_Scale(2.5f, 10.f, 0.f);
	m_pEgyptTransCom->Set_Pos(50.f, 6.f, 47.f);
	return S_OK;
	
	return S_OK;
}

_int CLogoBilboard::Update_Object(const _float & fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);
	m_pTransCom->Billboard_Transform(fTimeDelta);

	m_pMeMorialTransCom->Billboard_Transform(fTimeDelta);

	m_pAnubisTransCom->Billboard_Transform(fTimeDelta);

	m_pMageTransCom->Billboard_Transform(fTimeDelta);

	m_pTreeTransCom->Billboard_Transform(fTimeDelta);

	m_pSocerTransCom->Billboard_Transform(fTimeDelta);

	m_pNewTransCom->Billboard_Transform(fTimeDelta);

	m_pGlcierTransCom->Billboard_Transform(fTimeDelta);

	m_pProTransCom->Billboard_Transform(fTimeDelta);

	m_pHandTransCom->Billboard_Transform(fTimeDelta);

	m_pEgyptTransCom->Billboard_Transform(fTimeDelta);

	
	Add_RenderGroup(RENDER_NONALPHA, this);
	return 0;
}

void CLogoBilboard::LateUpdate_Object(void)
{
	CGameObject::LateUpdate_Object();
}

void CLogoBilboard::Render_Object(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0xcc);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	m_pTextureCom->Set_Texture(0);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pMeMorialTransCom->Get_WorldMatrixPointer());

	m_pMeMorialTexture->Set_Texture(0);
	m_pMeMorialBufferCom->Render_Buffer();


	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pAnubisTransCom->Get_WorldMatrixPointer());
	m_pAnubisTexture->Set_Texture(0);
	m_pAnubisBufferCom->Render_Buffer();


	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pMageTransCom->Get_WorldMatrixPointer());
	m_pMageTexture->Set_Texture(0);
	m_pMageBufferCom->Render_Buffer();


	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTreeTransCom->Get_WorldMatrixPointer());

	m_pTreeTexture->Set_Texture(0);
	m_pTreeBufferCom->Render_Buffer();


	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pSocerTransCom->Get_WorldMatrixPointer());

	m_pSocerTexture->Set_Texture(0);
	m_pSocerBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pNewTransCom->Get_WorldMatrixPointer());

	m_pNewTexture->Set_Texture(0);
	m_pNewBufferCom->Render_Buffer();


	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pGlcierTransCom->Get_WorldMatrixPointer());

	m_pGlcierTexture->Set_Texture(0);
	m_pGlcierBufferCom->Render_Buffer();


	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pProTransCom->Get_WorldMatrixPointer());

	m_pProTexture->Set_Texture(0);
	m_pProBufferCom->Render_Buffer();


	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pHandTransCom->Get_WorldMatrixPointer());

	m_pHandTexture->Set_Texture(0);
	m_pHandBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pEgyptTransCom->Get_WorldMatrixPointer());

	m_pEgyptTexture->Set_Texture(0);
	m_pEgyptBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

HRESULT CLogoBilboard::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(RCTEX_COMP));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ RCTEX_COMP, pComponent });

	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Clone_Proto(TRANSFORM_COMP));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ TRANSFORM_COMP, pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_LogoBil"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_LogoBil", pComponent });



	pComponent = m_pMeMorialBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_MemorialRC"));
	NULL_CHECK_RETURN(m_pMeMorialBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_MemorialRC", pComponent });

	pComponent = m_pMeMorialTransCom = dynamic_cast<CTransform*>(Clone_Proto(TRANSFORM_COMP));
	NULL_CHECK_RETURN(m_pMeMorialTransCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Comp2", pComponent });

	pComponent = m_pMeMorialTexture = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Memorial"));
	NULL_CHECK_RETURN(m_pMeMorialTexture, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Memorial", pComponent });




	pComponent = m_pAnubisBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_AnubisRC"));
	NULL_CHECK_RETURN(m_pAnubisBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_AnubisRC", pComponent });

	pComponent = m_pAnubisTransCom = dynamic_cast<CTransform*>(Clone_Proto(TRANSFORM_COMP));
	NULL_CHECK_RETURN(m_pAnubisTransCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_AnubisTrans", pComponent });

	pComponent = m_pAnubisTexture = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Anubis"));
	NULL_CHECK_RETURN(m_pAnubisTexture, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Anubis", pComponent });


	pComponent = m_pMageBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_MageRC"));
	NULL_CHECK_RETURN(m_pMageBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_MageRC", pComponent });

	pComponent = m_pMageTransCom = dynamic_cast<CTransform*>(Clone_Proto(TRANSFORM_COMP));
	NULL_CHECK_RETURN(m_pMageTransCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_MageTrans", pComponent });

	pComponent = m_pMageTexture = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Mage"));
	NULL_CHECK_RETURN(m_pMageTexture, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Mage", pComponent });



	pComponent = m_pSocerBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_SocerRC"));
	NULL_CHECK_RETURN(m_pSocerBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_SocerRC", pComponent });

	pComponent = m_pSocerTransCom = dynamic_cast<CTransform*>(Clone_Proto(TRANSFORM_COMP));
	NULL_CHECK_RETURN(m_pSocerTransCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_SocerTrans", pComponent });

	pComponent = m_pSocerTexture = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Socer"));
	NULL_CHECK_RETURN(m_pSocerTexture, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Socer", pComponent });



	pComponent = m_pTreeBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_TreeRC"));
	NULL_CHECK_RETURN(m_pTreeBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_TreeRC", pComponent });

	pComponent = m_pTreeTransCom = dynamic_cast<CTransform*>(Clone_Proto(TRANSFORM_COMP));
	NULL_CHECK_RETURN(m_pTreeTransCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_TreeTrans", pComponent });

	pComponent = m_pTreeTexture = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Tree"));
	NULL_CHECK_RETURN(m_pMageTexture, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Tree", pComponent });


	pComponent = m_pNewBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_NewRC"));
	NULL_CHECK_RETURN(m_pNewBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_NewRC", pComponent });

	pComponent = m_pNewTransCom = dynamic_cast<CTransform*>(Clone_Proto(TRANSFORM_COMP));
	NULL_CHECK_RETURN(m_pNewTransCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_NewTransCom", pComponent });

	pComponent = m_pNewTexture = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_New"));
	NULL_CHECK_RETURN(m_pNewTexture, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_New", pComponent });


	//////////
	pComponent = m_pGlcierBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_GlcierRC"));
	NULL_CHECK_RETURN(m_pGlcierBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_GlcierRC", pComponent });

	pComponent = m_pGlcierTransCom = dynamic_cast<CTransform*>(Clone_Proto(TRANSFORM_COMP));
	NULL_CHECK_RETURN(m_pGlcierTransCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_GlcierTransCom", pComponent });

	pComponent = m_pGlcierTexture = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Glcier"));
	NULL_CHECK_RETURN(m_pGlcierTexture, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Glcier", pComponent });


	pComponent = m_pProBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_ProRC"));
	NULL_CHECK_RETURN(m_pProBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_ProRC", pComponent });

	pComponent = m_pProTransCom = dynamic_cast<CTransform*>(Clone_Proto(TRANSFORM_COMP));
	NULL_CHECK_RETURN(m_pProTransCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_ProTransCom", pComponent });

	pComponent = m_pProTexture = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Pro"));
	NULL_CHECK_RETURN(m_pProTexture, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Pro", pComponent });


	pComponent = m_pHandBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_HandRC"));
	NULL_CHECK_RETURN(m_pHandBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_HandRC", pComponent });

	pComponent = m_pHandTransCom = dynamic_cast<CTransform*>(Clone_Proto(TRANSFORM_COMP));
	NULL_CHECK_RETURN(m_pHandTransCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_HandTransCom", pComponent });

	pComponent = m_pHandTexture = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Hand"));
	NULL_CHECK_RETURN(m_pHandTexture, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Hand", pComponent });
	


	pComponent = m_pEgyptBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_EgyptRC"));
	NULL_CHECK_RETURN(m_pEgyptBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_EgyptRC", pComponent });

	pComponent = m_pEgyptTransCom = dynamic_cast<CTransform*>(Clone_Proto(TRANSFORM_COMP));
	NULL_CHECK_RETURN(m_pEgyptTransCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_EgyptTranCom", pComponent });

	pComponent = m_pEgyptTexture = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Egypt"));
	NULL_CHECK_RETURN(m_pEgyptTexture, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Egypt", pComponent });

	return S_OK;
}

CLogoBilboard * CLogoBilboard::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLogoBilboard* pInstance = new CLogoBilboard(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CLogoBilboard::Free(void)
{
	CGameObject::Free();
}
