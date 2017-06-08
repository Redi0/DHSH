#ifndef __DHSH_GAMEDLL_GAME_METHOD_CALL_GAMECALL_H__
#define __DHSH_GAMEDLL_GAME_METHOD_CALL_GAMECALL_H__

#include "GameBase.h"

class CGameCALL : public MyTools::CRelfexBaseClass
{
public:
	CGameCALL() = default;
	~CGameCALL() = default;
	
	// ����
	BOOL Give(_In_ DWORD dwUiBase, _In_ DWORD dwIndex) CONST;
	
	// �����
	BOOL WhacAMole(_In_ DWORD dwIndex, _In_ DWORD dwCmd, _In_ DWORD dwNonw) CONST;

	// �򿪸���Ĵ���
	BOOL OpenGiveGameUi(_In_ DWORD dwUiBase) CONST;

	// Send
	BOOL Send(_In_ CONST CHAR* Buffer) CONST;

	// �ʼ�
	BOOL Mail(_In_ CONST std::wstring& wsBoxName, _In_ CONST std::wstring& wsPlayerName, _In_ DWORD dwId, _In_ DWORD dwPlayerId) CONST;

	// Close Game UI
	BOOL CloseGameUi(_In_ DWORD dwUiBase) CONST;

	// ����
	BOOL Give_Send(_In_ DWORD dwNpcId, _In_ DWORD dwId) CONST;

	// ���������Ƿ��ܽ���(�������)
	BOOL CheckPointIsWork(_In_ CONST Point& Point_) CONST;

	// ����
	BOOL RunAway() CONST;

	// ����
	BOOL Defence() CONST;

	// 
	BOOL ClickMonster(_In_ DWORD dwObjAddr) CONST;

	// ��
	BOOL SellToNpc(_In_ DWORD dwItemId, _In_ DWORD dwNpcId, _In_ DWORD dwCount) CONST;

	// ʰ��
	BOOL CollectScraps(_In_ DWORD dwId) CONST;

	// ��������
	BOOL AbandonTask(_In_ DWORD dwTaskId) CONST;

	// ����ʩ������
	BOOL SetUseSkill(_In_ DWORD dwSkillId) CONST;

	// �ر����
	BOOL CloseTeam(_In_ DWORD dwBase) CONST;

	// �����Զ�ʩ��
	BOOL SetAutoUseSkill() CONST;

	// ��ץ
	BOOL CatchMonster() CONST;

	// ��ݼ�
	BOOL SetHotKey(_In_ DWORD dwKeyValue) CONST;

	// ��Npc
	BOOL OpenNpc(_In_ DWORD dwNpcId) CONST;

	// ˢ����֤��
	BOOL FreshVerCode() CONST;

	// Save
	BOOL Save(_In_ DWORD dwItemId, _In_ DWORD dwCount) CONST;

	// Add Hp
	BOOL AddHp(_In_ DWORD dwActionId) CONST;

	// �ֽ�װ��
	BOOL DecompostionEqui(_In_ DWORD dwId) CONST;

	// ��ս
	BOOL EnterWar(_In_ DWORD dwPetId) CONST;

	// ����
	BOOL Destory(_In_ DWORD dwItemId, _In_ DWORD dwCount) CONST;

	// ����
	BOOL CallGod() CONST;

	// �ϳɱ�ʯ
	BOOL SynthesisGem(_In_ DWORD dwResId1, _In_ DWORD dwResId2) CONST;

	// ������ȡ
	BOOL ExtractItem(_In_ DWORD dwItemId) CONST;

	// ʹ������
	BOOL UseItem(_In_ DWORD dwItemId) CONST;

	// ʹ�÷���
	BOOL UseMagicWeapon(_In_ DWORD dwItemId) CONST;

	// ��ȭ
	BOOL Mora(_In_ DWORD dwIndex) CONST;

	// �Զ�
	BOOL SetAutoAction() CONST;

	// ���򷨱�
	BOOL Buy_By_Npc(_In_ DWORD dwNpcId, _In_ DWORD dwLocation, _In_ DWORD dwCount) CONST;

	// ����
	BOOL AnswerQuestion(_In_ DWORD dwAnswer, _In_ DWORD dwBase) CONST;

	// �ر�Npc�Ի�
	BOOL CloseNpcDlg() CONST;

	// Ѱ·
	BOOL MoveToNpc(_In_ LPCSTR pszMapName, _In_ LPCSTR pszNpcName) CONST;
	BOOL MoveToPoint(_In_ CONST Point& Point_) CONST; // Current Map
	BOOL MoveToPoint(_In_ LPCSTR pszMapName, _In_ CONST Point& Point_) CONST;
	BOOL MoveToPoint_Mouse(_In_ CONST Point& TarPoint) CONST;

	// �ɼ�
	BOOL CollectItem(_In_ DWORD dwObjAddr) CONST;
	BOOL CollectFurniture(_In_ DWORD dwId) CONST;


	// ���
	BOOL ClickNpcOption(_In_ DWORD dwNpcId, _In_ LPCSTR pszOptionText) CONST;

	// ���س�������
	CONST CHAR* GetPetName_By_SkillId(_In_ DWORD dwSkillId) CONST;

	// AutoMove
	BOOL SetAutoMove(_In_ BOOL bRun) CONST;

	//
public:
	static CGameCALL* CreateInstance()
	{
		return new CGameCALL;
	}
	
	virtual VOID ReleaseInstance(_In_ LPVOID lpObjectAddr) CONST
	{
		delete reinterpret_cast<CGameCALL*>(lpObjectAddr);
	}
};

#endif // !__DHSH_GAMEDLL_GAME_METHOD_CALL_GAMECALL_H__
