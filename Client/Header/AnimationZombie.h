#pragma once
#include "GameObject.h"

#include "TransAxisBox.h"

BEGIN(Engine)

class CHitBox;
class CTransform;
class CCalculator;
class CCollision;
class CCubeCol;

END
class CAnimationZombie :
	public CGameObject
{
public:
	CAnimationZombie(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CAnimationZombie();


public:
	virtual		HRESULT		Ready_Object(void);
	virtual		_int		Update_Object(const _float& fTimeDelta);
	virtual		void		LateUpdate_Object(void);
	virtual		void		Render_Object(void);

private:
	void					Key_Input(void);					//	키보드 입력
	HRESULT					Build(void);						//	처음 시작시 불러올 모델
	void					Look_Direction(void);

private:
	HRESULT					Add_Component(void);

	CHitBox*				m_pHitbox = nullptr;
	CTransform*				m_pTransform = nullptr;
	CCalculator*			m_pCalculator = nullptr;
	CCollision*				m_pCollision = nullptr;

	CCubeCol*				m_pCubeCol = nullptr;
	CTransform*				m_pRotationTrans = nullptr;

private:
	_vec3					vDir = { 0.f ,0.f ,0.f };
	_float					m_fTimeDelta = 0.f;					//	업데이트 이외의 영역에서도 시간값을 사용하기 위함
	_bool					m_bFirst = true;					//	모델 불러오기에 대해 1회만 진행하도록 하기 위함
	_float					m_AnimationTime = 0.f;				//	fTimeDelta의 누적값으로 애니메이션의 보간을 위해 존재

	PLAYERSTATEID			m_STATE;
	PLAYERWALKID			m_WALK;
	PLAYERSTOPID			m_STOP;

private:
	void					Sync_Animation(PLAYERSTATEID eID);				//	이거 지금 안씁니다
	void					Load_Animation(wstring FileName);				//	실제 애니메이션 월드 변환 스페이스를 받아오는 함수
	void					Run_Animation(const _float& AnimationSpeed);	//	애니메이션 동작

private:
	void					Walk_Animation_Run(void);						//	역할에 따라 추가 함수 생성
	void					Stop_Animation_Run(void);

private:
	list<char*>							m_CharList;				// char쓰레기통
	list<_tchar*>						m_TcharList;			// tchar쓰레기통

public:
	static CAnimationPlayer* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void	Free(void);
};

};

