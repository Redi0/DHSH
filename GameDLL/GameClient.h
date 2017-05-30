#ifndef __DHSH_GAMEDLL_GAME_SOCKET_GAMECLIENT_H__
#define __DHSH_GAMEDLL_GAME_SOCKET_GAMECLIENT_H__

#include <MyTools/CLClient.h>
#include <MyTools/SocketBuffer.h>
#include <mutex>
#include "GameVariable.h"

class CGameClient : public MyTools::CRelfexBaseClass, public MyTools::CLClient
{
public:
	CGameClient();
	~CGameClient();
	
	// �첽����
	BOOL AsyncSend(_In_ MyTools::CLSocketBuffer& SocketBuffer);


	// ͬ������(�ȴ�������Ϣ)
	BOOL SyncSend(_In_ MyTools::CLSocketBuffer& SocketBuffer, std::function<VOID(MyTools::CLSocketBuffer& SocketBuffer)> ExtractPtr);


	VOID SetEchoErrorPtr(_In_ std::function<VOID(VOID)> EchoErrorPtr);

	// Keep ALive
	VOID RunKeepALive();

	//
	BOOL GameLogin();

protected:

	// �����հ�
	virtual VOID EchoPacket(_In_ MyTools::CLSocketBuffer& SocketBuffer) override;


	// ��������Ͽ����ӻص�
	virtual VOID DisConnectServer() CONST override;

private:
	// 
	VOID EchoError() CONST;

	// socket����
	static DWORD WINAPI _KeepALiveThread(LPVOID lpParam);

private:
	std::mutex                                  _SendMutex;
	DWORD                                       _dwMsgHead;
	HANDLE                                      _hEvent;
	std::function<VOID(MyTools::CLSocketBuffer& SocketBuffer)> _ExtractActionPtr;
	std::function<VOID(VOID)>                   _EchoErrorPtr;
	HANDLE                                      _hKeepALiveThread;
public:
	virtual VOID ReleaseInstance(_In_ LPVOID lpObjectAddr) CONST
	{
		delete reinterpret_cast<CGameClient*>(lpObjectAddr);
	}
	
	CGameClient(CONST CGameClient&) = delete;
	
	CGameClient& operator=(CONST CGameClient&) = delete;
};

#endif // !__DHSH_GAMEDLL_GAME_SOCKET_GAMECLIENT_H__
