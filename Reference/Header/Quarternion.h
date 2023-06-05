#pragma once
#include "Component.h"
#include "Engine_Include.h"

BEGIN(Engine)

class ENGINE_DLL CQuarternion : public CComponent
{
private:
	explicit CQuarternion(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CQuarternion(const CQuarternion& rhs);
	virtual ~CQuarternion();

public:
	HRESULT	Ready_Quarternion(void);
	HRESULT Add_World(D3DXMATRIX* _World);

public:
	vector<D3DXMATRIX>*		Get_WorldVector(void) { return &m_vecWorld; }
	void					Delete_WorldVector(void) { m_vecWorld.clear(); }
	void					Delete_Front(void)
	{
		m_vecWorld.erase(m_vecWorld.begin());
		m_vecWorld.shrink_to_fit();
	}

private:
	vector<D3DXMATRIX>		m_vecWorld;


// 저장 애니메이션 테스트
private:
	vector<D3DXMATRIX>		AnimationSlot[50];

public:
	void					Change_Animation(_uint Slot) { m_vecWorld = AnimationSlot[Slot]; }
	void					Fill_Animation(_matrix* matWorld, _uint Slot) { AnimationSlot[Slot].push_back(*matWorld); }

public:
	static CQuarternion*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent*		Clone(void);
	virtual void			Free(void);
};

END