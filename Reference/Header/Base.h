#ifndef Base_h__
#define Base_h__

class _declspec(dllexport) CBase
{
protected:
	inline		explicit CBase(void);
	inline		virtual ~CBase(void);

public:
	inline		unsigned long		AddRef(void);		// 레퍼런스 카운트 증가
	inline		unsigned long		Release(void);		// 레퍼런스 카운트 감소 및 삭제

private:
	unsigned long				m_dwRefCnt;

public:
	inline virtual void	Free(void) = 0;
};

#include "Base.inl"

#endif // Base_h__
