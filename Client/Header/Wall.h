#pragma once
#include "GameObject.h"
#include "WallMapping.h"
namespace Engine
{
	class CCubeTex;
	class CTexture;
	class CTransform;
}

class CWall : public CGameObject
{

	enum WALLID { WALL_TERRAIN, WALL_END, };

protected:
	explicit CWall(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CWall();

public:
	virtual		HRESULT		Ready_Object(_ulong Texture, _vec3 * Position);
	virtual		_int		Update_Object(const _float& fTimeDelta);
	virtual		void		LateUpdate_Object(void);
	virtual		void		Render_Object(void);

private:
	HRESULT					Add_Component(void);
	HRESULT					Wall_Mapping(void);
	CCubeTex*				m_pCubeTex = nullptr;
	CTexture*				m_pTexture = nullptr;
	CTransform*				m_pTransform = nullptr;

private:
	_ulong					m_dwWallTextureNum;

	_bool					m_MappingInit = false;
	_int					m_iCnt = 0;
	list<TCHAR*>			m_listWallCnt;
	CTransform*				m_pWallMapping = nullptr;


public:
	static CWall*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _ulong Texture, _vec3 * Position);
	virtual void	Free(void);

};

