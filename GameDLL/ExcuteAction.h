#ifndef __DHSH_GAMEDLL_GAME_METHOD_CALL_EXCUTEACTION_H__
#define __DHSH_GAMEDLL_GAME_METHOD_CALL_EXCUTEACTION_H__

#include "GameBase.h"
#include <queue>
#include <MyTools/CLLock.h>

class CExcuteAction : public MyTools::CRelfexBaseClass
{
public:
	struct ThreadMethodInfo
	{
		std::function<VOID(VOID)> ThreadExcutePtr;
		HANDLE                    hEvent;
	};
public:
	CExcuteAction();
	~CExcuteAction() = default;
	
	// �������������߳�ȥִ��!
	VOID PushPtrToMainThread(_In_ std::function<VOID(VOID)> MethodPtr);

	VOID ExcutePtr();
private:
	
private:
	// ���߳�ִ�к�������
	std::queue<ThreadMethodInfo> _QueMethodPtr;

	// ������
	MyTools::CLLock _LockQueMethodPtr;
public:
	static CExcuteAction* CreateInstance()
	{
		return new CExcuteAction;
	}
	
	virtual VOID ReleaseInstance(_In_ LPVOID lpObjectAddr) CONST
	{
		delete reinterpret_cast<CExcuteAction*>(lpObjectAddr);
	}

	static CONST std::wstring& GetClassName_()
	{
		static CONST std::wstring wsClassName = L"CExcuteAction";
		return wsClassName;
	}
};

#endif // !__DHSH_GAMEDLL_GAME_METHOD_CALL_EXCUTEACTION_H__
