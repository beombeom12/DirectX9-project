#ifndef Engine_Struct_h__
#define Engine_Struct_h__

namespace Engine
{
	typedef	struct tagVertexColor
	{
		_vec3		vPos;
		_ulong		dwColor;

	}VTXCOL;

	const _ulong	FVF_COL = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX0;

	typedef struct tagVertexTex
	{
		_vec3		vPos;
		_vec3		vNormal;
		_vec2		vTexUV;
	}VTXTEX;

	const _ulong		FVF_TEX = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

	typedef struct tagVertexCubeTex
	{
		_vec3		vPos;
		_vec3		vTexUV;
		_vec3		vNormal;
	}VTXCUBE;

	const _ulong	FVF_CUBE = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0) | D3DFVF_NORMAL ;		// D3DFVF_TEXCOORDSIZE3 : 텍스처의 UV값이 FLOAT 형 3개의 크기만큼이며, 괄호산의 숫자 0의 의미는 본래 버텍스에 텍스처 UV 값이 여러개가 올 수 있는데 그중 0번째 것이 값을 지정하겠다는 의미이다.

	typedef struct tagVertexCubeTex2
	{
		_vec3		vPos;
		_vec2		vTexUV;

	}VTXCUBE2;

	const _ulong	FVF_CUBE2 = D3DFVF_XYZ | D3DFVF_TEX1;

	typedef struct tagParticle 
	{
		_vec3	vPos;
		_ulong	dwColor;
	}PARTICLE;

	const _ulong	FVF_PARTICLE = D3DFVF_XYZ | D3DFVF_DIFFUSE;

	typedef	struct tagAttribute
	{
		_vec3  vPos;
		_vec3  vVelocity;
		_vec3  vAcceleration;
		_float fLifeTime;
		_float fAge;
		_ulong dwColor;
		_ulong dwColorFade;
		_bool  bAlive;
	}ATTRIBUTE;

	typedef	struct tagIndex16
	{
		_ushort			_0;
		_ushort			_1;
		_ushort			_2;

	}INDEX16;

	typedef	struct tagIndex32
	{
		_ulong		_0;
		_ulong		_1;
		_ulong		_2;

	}INDEX32;

	typedef struct tagAbility
	{
		_float		fHp;				// 체력을 위한
		_float		fMaxHp;
		_float		fDefence;				// 방어력을 위한
		_float		fMaxDefence;
		_int		iGunTexture;			// 총 UI 변경을 위한
	}ABILITY;

	typedef struct tagGunAbility
	{
		_float		fBulletAttack;
		_float		fBulletRate;	  // 총알 속도
		_float		fRemainBulletCnt; // 남은 총알 개수 
		_float		fBulletCount;	  // 총알 개수 제한용
		_bool		bShoot;
	}GUNABILITY;

	typedef struct tagMonsterState
	{
		std::wstring strObjTag;	// UI 에서 몬스터 이름 찾아주려고
		_int   iLevel;
		_float fCurrentHp;
		_float fMaxHp;
		_float fDamage;
	}MONSTERABILITY;
	
	typedef struct tagMiddeMonster
	{
		std::wstring strObjTag;
		_float fCurrentHp;
		_float fMaxHp;
		_float fDamage;
	}MIDDLEBOSSABILITY;


	typedef struct tagKraken
	{
		std::wstring strObjTag;
		_float fCurrentHp;
		_float fMaxHp;
		_float fDamage;

	}KRAKENABILITY;


	typedef struct tagTerret
	{
		std::wstring strObjTag;
		_float fCurrentHp;
		_float fMaxHp;
		_float fDamage;
	}TERRETABILITY;
}


#endif // Engine_Struct_h__
