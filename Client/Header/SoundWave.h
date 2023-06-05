#pragma once
#include "PSystem.h"
class CSoundWave : public CPSystem
{
protected:
	explicit CSoundWave(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSoundWave();

public:
	virtual		HRESULT		Ready_Object(void);
	virtual		_int		Update_Object(const _float& fTimeDelta);
	virtual		void		LateUpdate_Object(void);
	virtual		void		Render_Object(void);
	void					Set_PclePos(_vec3 _vPos)
	{
		m_vSoundWavePatriclePos = _vPos;
	}

public:
	HRESULT						Add_Component(void);
	static CSoundWave*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual		void			Free(void);

private:
	_vec3	m_vSoundWavePatriclePos = { 0.f,0.f,0.f };

public:
	virtual		void		resetParticle(ATTRIBUTE* attribute);
};

