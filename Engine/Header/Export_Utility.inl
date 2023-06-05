
HRESULT	Create_Management(LPDIRECT3DDEVICE9& pGraphicDev, CManagement** ppManagement)
{
	CManagement*		pManagement = CManagement::GetInstance();
	NULL_CHECK_RETURN(pManagement, E_FAIL);

	*ppManagement = pManagement;

	return S_OK;
}

HRESULT		Set_Scene(CScene* pScene)
{
	return CManagement::GetInstance()->Set_Scene(pScene);
}

_int		Update_Scene(const _float& fTimeDelta)
{
	return CManagement::GetInstance()->Update_Scene(fTimeDelta);
}
void		LateUpdate_Scene(void)
{
	CManagement::GetInstance()->LateUpdate_Scene();
}
void		Render_Scene(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CManagement::GetInstance()->Render_Scene(pGraphicDev);
}

CComponent*				Get_Component(const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eID)
{
	return CManagement::GetInstance()->Get_Component(pLayerTag, pObjTag, pComponentTag, eID);
}

CGameObject*			Get_GameObject(const _tchar* pLayerTag, const _tchar* pObjTag)
{
	return CManagement::GetInstance()->Get_GameObject(pLayerTag, pObjTag);
}

CLayer*				Get_Layer(const _tchar* pLayerTag)
{
	return CManagement::GetInstance()->Get_Layer(pLayerTag);
}

CScene*				Get_Scene(void)
{
	return CManagement::GetInstance()->Get_Scene();
}

HRESULT					Add_GameObject(const _tchar* pLayerTag, CGameObject* pGameObject, const _tchar* pObjTag)
{
	return CManagement::GetInstance()->Add_GameObject(pLayerTag, pGameObject, pObjTag);
}

HRESULT					Delete_GameObject(const _tchar* pLayerTag, const _tchar* pObjTag)
{
	return CManagement::GetInstance()->Delete_GameObject(pLayerTag, pObjTag);
}

inline HRESULT			Ready_Proto(const _tchar* pProtoTag, CComponent* pComponent)
{
	return CProtoMgr::GetInstance()->Ready_Proto(pProtoTag, pComponent);
}

inline CComponent*		Clone_Proto(const _tchar* pProtoTag)
{
	return CProtoMgr::GetInstance()->Clone_Proto(pProtoTag);
}

inline HRESULT			Delete_Proto(const _tchar* pProtoTag)
{
	return CProtoMgr::GetInstance()->Delete_Proto(pProtoTag);
}

void		Add_RenderGroup(RENDERID eID, CGameObject* pGameObject)
{
	CRenderer::GetInstance()->Add_RenderGroup(eID, pGameObject);
}
void		Render_GameObject(LPDIRECT3DDEVICE9& pGraphicDev)
{
	CRenderer::GetInstance()->Render_GameObject(pGraphicDev);
}
void		Clear_RenderGroup(void)
{
	CRenderer::GetInstance()->Clear_RenderGroup();
}

list<CGameObject*>			Get_GameObjectGroup(RENDERID eId)
{
	CRenderer::GetInstance()->Get_GameObjectGroup(eId);
}

HRESULT			Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev, const D3DLIGHT9* tLightInfo, const _uint& iIndex)
{
	return CLightMgr::GetInstance()->Ready_Light(pGraphicDev, tLightInfo, iIndex);
}

void Initialize()
{
	CSoundMgr::GetInstance()->Initialize();
}
void Release()
{
	CSoundMgr::GetInstance()->Release();
}
void PlaySoundW(TCHAR* pSoundKey, CHANNELID eID, float fVolume)
{
	CSoundMgr::GetInstance()->PlaySoundW(pSoundKey, eID, fVolume);
}
void PlayBGM(TCHAR* pSoundKey, float fVolume)
{
	CSoundMgr::GetInstance()->PlayBGM(pSoundKey, fVolume);
}
void StopSound(CHANNELID eID)
{
	CSoundMgr::GetInstance()->StopSound(eID);
}
void StopAll()
{
	CSoundMgr::GetInstance()->StopAll();
}
void PlaySlow(TCHAR * pSoundKey, CHANNELID eID, float fVolume, float fRate)
{
	CSoundMgr::GetInstance()->PlaySlow(pSoundKey, eID, fVolume, fRate);
}
void SetChannelVolume(CHANNELID eID, float fVolume)
{
	CSoundMgr::GetInstance()->SetChannelVolume(eID, fVolume);
}
void PlaySoundGun(TCHAR* pSoundKey, CHANNELID eID, float fVolume)
{
	CSoundMgr::GetInstance()->PlaySoundGun(pSoundKey, eID, fVolume);
}

inline void			Release_Utility(void)
{
	CFrustumCullMgr::GetInstance()->DestroyInstance();
	CSoundMgr::GetInstance()->DestroyInstance();
	CLightMgr::GetInstance()->DestroyInstance();
	CRenderer::GetInstance()->DestroyInstance();
	CProtoMgr::GetInstance()->DestroyInstance();
	CManagement::GetInstance()->DestroyInstance();
	CFrustumCullMgr::GetInstance()->DestroyInstance();
}
