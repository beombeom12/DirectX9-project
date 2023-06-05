#ifndef Export_Utility_h__
#define Export_Utility_h__

#include "Engine_Include.h"

#include "TriCol.h"
#include "RcCol.h"
#include "RcTex.h"
#include "TerrainTex.h"
#include "CubeTex.h"
#include "SphereTex.h"
#include "Hitbox.h"
#include "Collision.h"

#include "Calculator.h"
#include "Transform.h"
#include "Texture.h"
#include "Light.h"

#include "FlexibleAxisCube.h"
#include "Quarternion.h"
#include "CubeCol.h"

#include "Camera.h"

#include "ProtoMgr.h"
#include "Management.h"
#include "Renderer.h"
#include "LightMgr.h"
#include "SoundMgr.h"

#include "PSystem.h"
#include "DynamicBuffer.h"
#include "FrustumCullMgr.h"

#include "Shader.h"

BEGIN(Engine)

// Management
inline HRESULT					Create_Management(LPDIRECT3DDEVICE9& pGraphicDev, CManagement** ppManagement);
inline HRESULT					Set_Scene(CScene* pScene);
inline _int						Update_Scene(const _float& fTimeDelta);
inline void						LateUpdate_Scene(void);
inline void						Render_Scene(LPDIRECT3DDEVICE9 pGraphicDev);
inline 	CComponent*				Get_Component(const _tchar* pLayerTag,
	const _tchar* pObjTag,
	const _tchar* pComponentTag,
	COMPONENTID eID);
inline 	CGameObject*			Get_GameObject(const _tchar* pLayerTag, const _tchar* pObjTag);
inline 	CLayer*					Get_Layer(const _tchar* pLayerTag);
inline	CScene*					Get_Scene(void);

inline HRESULT			Add_GameObject(const _tchar* pLayerTag, CGameObject* pGameObject, const _tchar* pObjTag);
inline HRESULT			Delete_GameObject(const _tchar* pLayerTag, const _tchar* pObjTag);

// ProtoMgr
inline HRESULT			Ready_Proto(const _tchar* pProtoTag, CComponent* pComponent);
inline CComponent*		Clone_Proto(const _tchar* pProtoTag);
inline HRESULT			Delete_Proto(const _tchar * pProtoTag);

// Renderer
inline void							Add_RenderGroup(RENDERID eID, CGameObject* pGameObject);
inline void							Render_GameObject(LPDIRECT3DDEVICE9& pGraphicDev);
inline void							Clear_RenderGroup(void);
inline list<CGameObject*>			Get_GameObjectGroup(RENDERID eId);

// LightMgr
inline HRESULT			Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev, const D3DLIGHT9* tLightInfo, const _uint& iIndex);
inline vector<CLight*>	Get_Light();
inline void				Set_Light(int iIndex, const _vec3& vPos, _float fRange);

// SoundMgr
inline void Initialize();
inline void Release();
inline void PlaySoundW(TCHAR* pSoundKey, CHANNELID eID, float fVolume);
inline void PlayBGM(TCHAR* pSoundKey, float fVolume);
inline void StopSound(CHANNELID eID);
inline void StopAll();
inline void PlaySlow(TCHAR * pSoundKey, CHANNELID eID, float fVolume, float fRate);
inline void SetChannelVolume(CHANNELID eID, float fVolume);
inline void PlaySoundGun(TCHAR* pSoundKey, CHANNELID eID, float fVolume);

inline void			Release_Utility(void);


#include "Export_Utility.inl"

END
#endif 
