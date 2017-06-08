#ifndef __DHSH_GAMEDLL_GAME_METHOD_CALL_EXCUTEACTION_H__
#define __DHSH_GAMEDLL_GAME_METHOD_CALL_EXCUTEACTION_H__

#include "GameBase.h"
#include <queue>
#include <MyTools/CLLock.h>

//#define MESSAGE_CUSTOME WM_USER + 0x1A5
class CExcuteAction : public MyTools::CRelfexBaseClass
{
public:
	struct ThreadMethodInfo
	{
		std::function<VOID(VOID)> ThreadExcutePtr;
		HANDLE                    hEvent;
	};

	using PeekMessageAPtr = BOOL(WINAPI*)(_Out_ LPMSG lpMsg, _In_opt_ HWND hWnd, _In_ UINT wMsgFilterMin, _In_ UINT wMsgFilterMax, _In_ UINT wRemoveMsg);
public:
	CExcuteAction();
	~CExcuteAction() = default;
	
	// �������������߳�ȥִ��!
	VOID PushPtrToMainThread(_In_ std::function<VOID(VOID)> MethodPtr);

	VOID ExcutePtr();

	VOID SetRun(_In_ BOOL bRun);

protected:
	VOID RunGame();

	VOID Stop();
private:
	static BOOL WINAPI PeekMessage_(_Out_ LPMSG lpMsg, _In_opt_ HWND hWnd, _In_ UINT wMsgFilterMin, _In_ UINT wMsgFilterMax, _In_ UINT wRemoveMsg);
private:
	// ���߳�ִ�к�������
	std::queue<ThreadMethodInfo> _QueMethodPtr;

	// ������
	MyTools::CLLock _LockQueMethodPtr;

	//
	static PeekMessageAPtr _OldPeekMessagePtr;

	BOOL _RunComplete;
public:
	static CExcuteAction* CreateInstance()
	{
		return new CExcuteAction;
	}
	
	virtual VOID ReleaseInstance(_In_ LPVOID lpObjectAddr) CONST
	{
		delete reinterpret_cast<CExcuteAction*>(lpObjectAddr);
	}
};

#endif // !__DHSH_GAMEDLL_GAME_METHOD_CALL_EXCUTEACTION_H__
