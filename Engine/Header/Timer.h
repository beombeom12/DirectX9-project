#pragma once

#include "Engine_Include.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL  CTimer :	public CBase
{
public:
	explicit CTimer();
	virtual ~CTimer();

public:
	_float			Get_TimeDelta(void) { return m_fTimeDelta; }

public:
	HRESULT		Ready_Timer(void);
	void		Update_Timer(void);

private:
	LARGE_INTEGER		m_CurrentTime;
	LARGE_INTEGER		m_OldTime;
	LARGE_INTEGER		m_OriginTime;
	LARGE_INTEGER		m_CpuTick;

	float				m_fTimeDelta;

public:
	static CTimer*	Create(void);
	virtual void Free(void);

};

END