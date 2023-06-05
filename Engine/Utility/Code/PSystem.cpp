#include "..\..\Header\PSystem.h"

USING(Engine)

//const DWORD PARTICLE::FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;

CPSystem::CPSystem(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
	, m_origin({ 0.f,0.f,0.f })
	, m_fEmitRate(0.f)
	, m_fSize(0.f)
	, m_pTextureCom(nullptr)
	, m_vb(nullptr)
	, m_maxParticles(0)
	, m_vbSize(0)
	, m_vbOffset(0)
	, m_vbBatchSize(0)
{
}

CPSystem::~CPSystem()
{

}

HRESULT CPSystem::Ready_Object(void)
{
	// D3D장치 의존적 초기화 작업을 처리
	FAILED_CHECK_RETURN(m_pGraphicDev->CreateVertexBuffer(
		m_vbSize * sizeof(PARTICLE),	// 버텍스 버퍼 크기
		D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY,
		// _DYNAMIC : 동적 버텍스버퍼를 사용한다.
		// _POINTS	: 버텍스 버퍼가 포인트 스프라이트를 보관할 것임을 지정
		FVF_PARTICLE,
		D3DPOOL_DEFAULT,
		&m_vb,
		0), E_FAIL);

	return S_OK;
}

_int CPSystem::Update_Object(const _float & fTimeDelta)
{
	return CGameObject::Update_Object(fTimeDelta);
}

void CPSystem::LateUpdate_Object(void)
{
	CGameObject::LateUpdate_Object();
}

void CPSystem::Render_Object(void)
{
	render();
}

void CPSystem::Free(void)
{
	m_particles.clear();

	Safe_Release(m_vb);

	CGameObject::Free();
}

void CPSystem::reset()
{
	// 모든 파티클 속성을 리셋
	for (auto& iter = m_particles.begin(); iter != m_particles.end(); ++iter)
	{
		resetParticle(&(*iter));
	}
}

void CPSystem::addParticle()
{
	// 시스템에 파티클을 추가.
	ATTRIBUTE tAttribute;

	resetParticle(&tAttribute);

	m_particles.push_back(tAttribute);
}

void CPSystem::preRender()
{
	// 렌더링에 앞서 지정할 초기 렌더 상태
	// 현재 지정된 전체 텍스처 포인트 스프라이트의 텍스처 매핑에 이용할 것임
	m_pGraphicDev->SetRenderState(D3DRS_POINTSPRITEENABLE, true);

	// 포인트 크기를 뷰스페이스 단위로 해석할 것인가?
	// 뷰스페이스 단위 : 카메라 공간내 3D 포인트. 카메라와으 거리에 따라 크기가 조절됨.
	// false : 스크린스페이스 기준
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALEENABLE, true);

	m_pGraphicDev->SetRenderState(D3DRS_POINTSIZE, FtoDw(m_fSize));
	m_pGraphicDev->SetRenderState(D3DRS_POINTSIZE_MIN, FtoDw(0.0f));

	// 거리(포인트와 카메라 사이 거리)에 따른 파티클 크기 제어
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALE_A, FtoDw(0.0f));
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALE_B, FtoDw(0.0f));
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALE_C, FtoDw(1.0f));

	// 텍스처의 알파를 이용한다.
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, true);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x00);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
}

void CPSystem::render()
{
		// 렌더 상태 지정
		preRender();
		
		m_pGraphicDev->SetFVF(FVF_PARTICLE);
		m_pGraphicDev->SetStreamSource(0, m_vb, 0, sizeof(PARTICLE));

		// 버텍스 버퍼를 벗어날 경우 처음부터 시작한다.
		if (m_vbOffset >= m_vbSize)
			m_vbOffset = 0;

		PARTICLE* v = 0;

		m_vb->Lock(
			m_vbOffset * sizeof(PARTICLE),
			m_vbBatchSize * sizeof(PARTICLE),
			(void**)&v,
			m_vbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);

		DWORD numParticlesInBatch = 0;

		// 모든 파티클이 렌더링 될때까지
		for (auto iter = m_particles.begin(); iter != m_particles.end(); ++iter)
		{
			if (iter->bAlive)
			{
				// 한단계의 생존한 파티클을 다음 버텍스 버퍼 세그먼트로 복사
				v->vPos = iter->vPos;
				v->dwColor = (D3DCOLOR)iter->dwColor;
				v++; // next element;

				numParticlesInBatch++; // 단계 카운터를 증가시킨다.

									   // 현재 단계가 모두 채워져있는가?
				if (numParticlesInBatch == m_vbBatchSize)
				{
					// 버텍스 버퍼로 복사된 마지막 단계의 파티클들을 그린다.
					m_vb->Unlock();

					m_pGraphicDev->DrawPrimitive(
						D3DPT_POINTLIST,
						m_vbOffset,
						m_vbBatchSize);

					// 단계가 그려지는 동안 다음 단계를 파티클로 채운다.

					// 다음 단계의 처음 오프셋으로 이동한다.
					m_vbOffset += m_vbBatchSize;

					// 버텍스 버퍼의 경계를 넘는 메모리로 오프셋을 설정하지 ㅇ낳는다.
					// 경계를 넘을 경우 처음부터 시작
					if (m_vbOffset >= m_vbSize)
						m_vbOffset = 0;

					m_vb->Lock(
						m_vbOffset * sizeof(PARTICLE),
						m_vbBatchSize * sizeof(PARTICLE),
						(void**)&v,
						m_vbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);

					numParticlesInBatch = 0; // 다음단계를 위한 리셋
				}
			}
		}

		m_vb->Unlock();

		// numParticleInBatch == vbBatchSize 조건이 만족되지 않아
		// 마지막단계가 렌더링되지 않는 경우가 발생할 수 있음.
		// 일부만 채워진 단계는 이곳에서 렌더링함.
		if (numParticlesInBatch)
		{
			m_pGraphicDev->DrawPrimitive(
				D3DPT_POINTLIST,
				m_vbOffset,
				numParticlesInBatch);
		}

		// 다음블록
		m_vbOffset += m_vbBatchSize;

		postRender();
}

void CPSystem::postRender() // 렌더상태를 복구하기 위해 필요한 함수
{
	m_pGraphicDev->SetRenderState(D3DRS_POINTSPRITEENABLE, false);
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALEENABLE, false);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, false);
}

_bool CPSystem::isEmpty()
{
	return m_particles.empty();
}

_bool CPSystem::isDead()
{
	for (auto& pt : m_particles)
	{
		if (true == pt.bAlive)
		{
			return false;
		}
	}
	return true;
}

void CPSystem::removeDeadParticles()
{
	std::list<ATTRIBUTE>::iterator i;

	i = m_particles.begin();

	while (i != m_particles.end())
	{
		if (i->bAlive == false)
		{
			// erase returns the next iterator, so no need to
			// incrememnt to the next one ourselves.
			i = m_particles.erase(i);
		}
		else
		{
			i++; // next in list
		}
	}
}
