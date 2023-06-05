#pragma once
#include "GameObject.h"
class CMBullet :
	public CGameObject
{
public:
	CMBullet(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMBullet();



public:
	virtual HRESULT Ready_Object(const _vec3* pPos, const _vec3* pDir, const _float fSpeed, const _float fScale, const _float fDamage, const _float fInterval);
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual void	Render_Object(void) override;

public:
	HRESULT			Add_Component(void);
	void Set_Pos(const _vec3& vPos);
	void MoveToDir(const _vec3& vDir);

	void			Set_Dir(const _vec3& vDir) { m_vDirection = vDir; }

private:

	CTransform*			m_pTransCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CCubeTex*			m_pCubetexCom = nullptr;
	_vec3				m_vDirection = _vec3(0.f, 0.f, 0.f);
	_float				m_fSpeed = 10.f;
	_float				m_fTimeDelta = 0.f;


public:
	static CMBullet*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pPos, const _vec3* pDir, const _float fSpeed, const _float fScale, const _float fDamage, const _float fInterval);
	virtual void		Free(void);
};

