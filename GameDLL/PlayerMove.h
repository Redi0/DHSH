#ifndef __DHSH_GAMEDLL_GAME_METHOD_ACTION_PLAYERMOVE_H__
#define __DHSH_GAMEDLL_GAME_METHOD_ACTION_PLAYERMOVE_H__

#include "GameBase.h"

class CGameUi;
class CPlayerMove : public MyTools::CRelfexBaseClass
{
public:
	CPlayerMove() = default;
	~CPlayerMove() = default;
	
	// Current Map
	BOOL MoveToPoint(_In_ CONST Point& TarPoint) CONST;

	// different Map
	BOOL MoveToMapPoint(_In_ CONST std::wstring& wsMapName, _In_ CONST Point& TarPoint) CONST;

	// ȥ�ӻ�̯
	BOOL MoveToGeneralStore(_Out_ std::wstring& wsNpcName) CONST;

	// Pure Move
	VOID Move(_In_ CONST std::wstring& wsMapName, _In_ CONST Point& TarPoint) CONST;

	// �󶨴����¼�
	BOOL FindDlg_By_BindMethod() CONST;

	// �ؼ�԰
	BOOL MoveToHome() CONST;

	// �ߵ�ĳ��Npc
	BOOL MoveToResNpc(_In_ CONST std::wstring& wsCityName, _In_ CONST std::wstring& wsNpcName) CONST;

	// ȥ�����ͼ����
	BOOL MoveToSpecialMap(_In_ CONST std::wstring& wsMapName, _In_ CONST Point& TarPoint, _In_ CONST std::wstring wsSpecialMapName) CONST;

	//
	BOOL MoveToPoint_By_Mouse(_In_ CONST Point& TarPoint) CONST;
private:
	// �ܵ��ӻ�̯Npc�����
	BOOL MoveToGeneralStoreNpc(_In_ CONST std::wstring& wsMapName, _In_ CONST std::wstring& wsNpcName) CONST;

	// �÷�����
	BOOL TransferToCity(_In_ CONST std::wstring& wsCityName) CONST;

	// ��ȡ3������ĸ��������Լ����
	std::wstring GetRecentlyCityName(_In_) CONST;

	//
	BOOL Action_When_UnMove(_In_ CONST std::wstring& wsMapName, _In_ CONST Point& TarPoint) CONST;

	//
	BOOL ShowNpcDlg_When_Move(CONST CGameUi& npcdlg) CONST;

	// 
	BOOL ShowQuestion_When_UnMove(CONST CGameUi& Question) CONST;

public:
	static CPlayerMove* CreateInstance()
	{
		return new CPlayerMove;
	}
	
	virtual VOID ReleaseInstance(_In_ LPVOID lpObjectAddr) CONST
	{
		delete reinterpret_cast<CPlayerMove*>(lpObjectAddr);
	}
};

#endif // !__DHSH_GAMEDLL_GAME_METHOD_ACTION_PLAYERMOVE_H__
