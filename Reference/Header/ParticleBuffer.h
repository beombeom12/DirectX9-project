//#pragma once
//#include "Component.h"
//
//BEGIN(Engine)
//
//class ENGINE_DLL CParticleBuffer :	public CComponent
//{
//protected:
//	explicit CParticleBuffer(LPDIRECT3DDEVICE9 pGraphicDev);
//	explicit CParticleBuffer(const CParticleBuffer& rhs);
//	virtual ~CParticleBuffer();
//
//public:
//	virtual HRESULT		Ready_Buffer(void);
//
//	virtual void		PostRender_Buffer(void);
//	virtual void		Render_Buffer(void);
//	virtual void		PreRender_Buffer();
//
//	list<ATTRIBUTE>*    Get_ParticleList() { return &m_ParticleList; }
//
//	void				Set_Size(_float _size) { m_fSize = _size; }
//	void				Set_VTXSize(_ulong _dwVtxSize) { m_dwVtxSize = _dwVtxSize; }
//	void				Set_VTXOffset(_ulong _dwVtxOffset) { m_dwVtxOffset = _dwVtxOffset; }
//	void				Set_VTXBatchSize(_ulong _dwVtxBatchSize) { m_dwVtxBatchSize = _dwVtxBatchSize; }
//private:
//	DWORD FtoDw(float f) { return *((DWORD*)&f); }
//
//protected:
//
//	LPDIRECT3DVERTEXBUFFER9		m_pVB;				// 정점들을 보관하고 접근하기 위한 컴객체
//	
//	list<ATTRIBUTE>				m_ParticleList;
//
//	_float						m_fSize = 0.8f; 			// 파티클의 사이즈
//	_ulong						m_dwVtxSize = 2048;		// 버텍스 크기
//	_ulong						m_dwVtxOffset = 0;		// 버텍스 버퍼에서 복사를 시작할 파티클 내 다음 단계로의 오프셋
//	_ulong						m_dwVtxBatchSize = 512;   // 하나의 단계에 정의된 파티클 수
//
//public:
//	static CParticleBuffer*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
//	CComponent*				Clone(void);
//	virtual void			Free(void);
//};
//
//END