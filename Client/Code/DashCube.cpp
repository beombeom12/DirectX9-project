


#include "stdafx.h"
#include "DashCube.h"
#include "StaticCamera.h"
#include "CubePlayer.h"

CDashCube::CDashCube(LPDIRECT3DDEVICE9 pGraphicDev)
:CPSystem(pGraphicDev)
{
m_fSize = .01f;
m_vbSize = 2048;
m_vbOffset = 0;
m_vbBatchSize = 512;
}

CDashCube::~CDashCube()
{
}

HRESULT CDashCube::Ready_Object(void)
{
CPSystem::Ready_Object();


FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
return S_OK;
}

_int CDashCube::Update_Object(const _float & fTimeDelta)
{
for (list<ATTRIBUTE>::iterator iter = m_particles.begin(); iter != m_particles.end(); iter++)
{
iter->fAge += fTimeDelta;
iter->vPos += (iter->vVelocity) * fTimeDelta * .1f;
//iter->vPos.y -= 0.01f* (m_fGravity*iter->fAge)*(m_fGravity* iter->fAge) * fTimeDelta;

if (iter->fAge > iter->fLifeTime)
{
iter->bAlive = false;
}
}
removeDeadParticles();

Add_RenderGroup(RENDER_PRIORITY, this);

return 0;
}

void CDashCube::LateUpdate_Object(void)
{
CGameObject::LateUpdate_Object();
}

void CDashCube::Render_Object(void)
{
_matrix matWorld;
D3DXMatrixIdentity(&matWorld);

m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
if (!m_particles.empty())
{
m_pTextureCom->Set_Texture(0);
CPSystem::Render_Object();
}
}

HRESULT CDashCube::Add_Component(void)
{
CComponent* pComponent = nullptr;

pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Black_Tex"));
NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
m_mapComponent[ID_STATIC].insert({ L"Black_Tex", pComponent });

return S_OK;
}

CDashCube * CDashCube::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
CDashCube *	pInstance = new CDashCube(pGraphicDev);

if (FAILED(pInstance->Ready_Object()))
{
Safe_Release(pInstance);
return nullptr;
}

return pInstance;
}

void CDashCube::Free(void)
{
CPSystem::Free();
}

void CDashCube::resetParticle(ATTRIBUTE * attribute)
{
attribute->bAlive = true;
CTransform* pTransform = nullptr;
m_fGravity = 15.f;
_vec3 min = _vec3(-1.0f, -1.0f, -1.0f);
_vec3 max = _vec3(1.0f, 1.0f, 1.0f);
GetRandomVector(&attribute->vVelocity, &min, &max);
D3DXVec3Normalize(&attribute->vVelocity, &attribute->vVelocity);
D3DXVec3Normalize(&m_vDir, &m_vDir);
attribute->vPos = m_vCubePatriclePos + attribute->vVelocity / .5f;
attribute->vPos += m_vDir*1.5f;
attribute->vPos.y -= attribute->vVelocity.y / 5.f;
attribute->dwColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
attribute->fAge = 0.0f;
attribute->fLifeTime = .5f;
}


