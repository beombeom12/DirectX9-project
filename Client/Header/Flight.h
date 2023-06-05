#pragma once

#include "GameObject.h"
#include "FlightBomb.h"
#include "RcTex.h"
class CFlightBulletParticle;
class CCubeParticle;


class CFlight :
	public CGameObject
{
private:
	explicit CFlight(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFlight();

public:
	virtual HRESULT Ready_Object(const _vec3 & vPos, const _vec3 & vDir, _tchar * Name);
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual void	Render_Object(void) override;

private:
	HRESULT			Add_Component(void);
	HRESULT			Set_Material();

	void			Fly_Effect();

	CTransform*		m_pTransform = nullptr;
	CCalculator*	m_pCalculator = nullptr;
	CTransform*		m_pBomb = nullptr;

	CTexture*		m_pBombTexture = nullptr;
	CRcTex*			m_pBombBuffer = nullptr;
	CRcTex*			m_pBufferCom = nullptr;
	CCubeParticle* m_pCubeParticle = nullptr;


public:
	void			Replace(const _vec3& vPos, const _vec3& vAngle, const _vec3& vDir)
	{
		m_pTransform->Set_Pos(vPos.x, vPos.y, vPos.z);
		m_vAngle = vAngle;
		m_vDirection = vDir;
	}
	void			Random(void);
	void			Set_Speed(_float fSpeed) { m_fSpeed = fSpeed; }

	void			Set_Control() { m_bControl = !m_bControl; }
	_bool			Get_Control() { return m_bControl; }
	_bool			Get_Shoot() { return m_bShoot; }
	void			Set_Shoot(_bool bShoot) { m_bShoot = bShoot; }

	_bool			Get_ShuttleCam() { return m_bShuttleCam; }
	void			Set_ShuttleCam(_bool bCam) { m_bShuttleCam = bCam; }

	void			Set_Shuttle(_bool bShuttle , const _vec3& vDestination, SUPPORTER_ID eID) 
	{
		m_bShuttle = bShuttle; 
		m_vDestination = vDestination; 
		m_eSupporterID = eID;
	}

	_bool			Get_Shuttle() { return m_bShuttle; }
	_vec3			Get_Desitination() { return m_vDestination; }

	_bool			Get_Ending() { return m_bEnding; }
	void			Set_Ending(_bool bEnd) { m_bEnding = bEnd; }
	_bool			m_bEnding = false;/////////////////

	_bool			m_bLeaveMap = false;

	
	_bool			m_bDest = false;
	_float			m_fEndingFrame = 0.f;
	_bool			m_bRide = false;

private:
	HRESULT			Build(void);
	void			Key_Input(const _float& fTimeDelta);

	void			Look_Direction();
	void			Look_Direction_Only_Y();

	void			Fire_Bullet();
	void			Bombing();
	void			Move(const _float& fTimeDelta);
	
	_float			m_fRotX = 0.f;
	_float			m_fRotY = 0.f;
	_float			m_fTurn = 0.f;

	_vec3			m_vDestination = _vec3(0.f, 0.f, 0.f);

	_vec3           m_vEndingDest = _vec3(0.f, 0.f, 0.f);
	_vec3           m_vEndingDest2 = _vec3(0.f, 0.f, 0.f);

	_vec3			vAfterDir = _vec3(0, 0, 0);

	_vec3			m_vDirection;
	_vec3			m_vLeave;
	_vec3			m_vAngle;
	_float			m_Frame = 0.f;

	vector<_vec3>	m_ShufflePos;
	vector<_uint>	m_ShuffleSpeed;
	_float			m_fSpeed;

	_bool			m_bShuttle = false;
	_bool			m_bShuttleCam;

	_int			m_iPosSet = -1;

	_float			m_fTimeDelta = 0.f;
	_bool			m_bFirst = true;
	_float			m_AnimationTime = 0.f;

	CLayer*			pMyLayer;
	_tchar*			m_RocketName;

	_bool			m_bControl = false;
	_bool			m_bShoot = false;
	_float			m_fBulletTime = 0.f;
	CFlightBulletParticle * m_pBulletParicle = nullptr;
	
	SUPPORTER_ID	m_eSupporterID;

	list<_tchar*>	m_TcharList;

public:
	static CFlight*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos, const _vec3 & vDir, _tchar* Name);
	virtual void		Free(void);
};

