#pragma once
#include "GameObject.h"
#include "Export_Function.h"
class CLetterBox : public CGameObject
{
private:
	explicit CLetterBox(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLetterBox();

public:
	virtual HRESULT Ready_Object(_tchar* tDialogue, _int iSize, _int iIndex);
	virtual _int   Update_Object(const _float& fTimeDelta) override;
	virtual void   LateUpdate_Object(void) override;
	virtual void   Render_Object(void) override;

	void   ChangeLetterContents(const wstring& str, _int iSize)
	{
		m_strLetterContents = str;
		m_iTextAmount = iSize - 3;
	}

	void	SetPosition(_float _PositionX, _float _PositionY) { m_fPosX = _PositionX, m_fPosY = _PositionY; }
	void   LetterDead() { m_bDead = true; }
	
public:
	void         On_Switch() { m_bPowerSwitch = true; }
	void         Off_Switch() { m_bPowerSwitch = false; }
	void      Maker(CLetterBox* pInstance)
	{
		Engine::Add_GameObject(STAGE_UI, pInstance, szCntName);
	}
	void		 SetAlpha(_float fAlpha) { m_fFontAlpha = fAlpha; }

private:
	HRESULT      Add_Component(void);
	void         Alpha_Effect();
	void         Begin_OrthoProj();
	void         End_OrthoProj();
	// 폰트 생성 창 인덱스
	// 0 : 화면 중앙 아래에서 깜빡거리는 문자열 (플레이어 알림)
	// 1 : 화면 오른쪽 위에서 점멸하는 문자열 (콤보 띄워주는 문자)
	// 2 : 화면 아래쪽에서 직교투영된 텍스쳐 위에 떠오르는 문자열 (대화문, 퀘스트)
	void         PlayerNotice();
	void         BoxText();
	void         Index1();
	void         HitCombo();
	void         DialogueBox();

private:
	CTransform*     m_TranformCom = nullptr;
	CTexture*       m_pFontTexture = nullptr;
	CTexture*       m_pTexture = nullptr;
	CCubeTex*       m_pCube = nullptr;
	CRcTex*         m_pRcCom = nullptr;
	CShader*	m_pShaderCom = nullptr;	
	wstring         m_strLetterContents;
	_float          m_fFontSizeX;
	_float          m_fFontSizeY;
	_float			m_fBoxSizeX;
	_float			m_fBoxSizeY;
	_float          m_fFontAlpha;
	_tchar          szCntName[64];

	_bool			m_bPowerSwitch = true;
	_bool			m_bAlphaSwitch = false;
	_bool			m_bInit = false;

	_int			m_iTextAmount;
	_int			m_iIndex;

	_float			m_fPosX = 0.f;
	_float			m_fPosY = 0.f;

public:
	static CLetterBox*   Create(LPDIRECT3DDEVICE9 pGraphicDev, _tchar* tDialogue, _int iSize, _int iIndex);
	virtual void Free(void) override;
};
