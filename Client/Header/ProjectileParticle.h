#pragma once
#include "PSystem.h"

class CItemParticle;
class CFlameEffect;
class CIceEffect;
class CBubbleEffect;
class CCloudEffect;
class CSparkEffect;
class CSoundWave;
class CRainbowCloudEffect;
class CBrownCloudEffect;

class CProjectileParticle : public CPSystem
{
protected:
	explicit CProjectileParticle(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CProjectileParticle();

public:
	virtual		HRESULT		Ready_Object(void);
	virtual		_int		Update_Object(const _float& fTimeDelta);
	virtual		void		LateUpdate_Object(void);
	virtual		void		Render_Object(void);

public:
	HRESULT						Add_Component(void);
	static CProjectileParticle*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual		void			Free(void);
	_vec3		Get_DeadPos() { return m_vDeadPos; }

public:
	virtual		void		resetParticle(ATTRIBUTE* attribute);
private:
	void		Dead_Effect();
private:
	_float		m_fGravity = 0.f;
	_vec3		m_vDeadPos = { 0.f,0.f,0.f };
	_int	m_iTextureIndex = 0;
	_float  m_fTextureTime = 0;

	CFlameEffect* m_pFlameEffectParticle = nullptr;
	CIceEffect* m_pIceEffectParticle = nullptr;
	CBubbleEffect* m_pBubbleEffectParticle = nullptr;
	CCloudEffect* m_pCloudEffectParticle = nullptr;
	CRainbowCloudEffect* m_pRainbowCloudEffectParticle = nullptr;
	CBrownCloudEffect* m_pBrownCloudEffectParticle = nullptr;
	CSparkEffect* m_pSparkEffectParticle = nullptr;
	CSoundWave* m_pSoundWaveParticle = nullptr;
};

