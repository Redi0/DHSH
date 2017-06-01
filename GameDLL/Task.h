#ifndef __DHSH_GAMEDLL_GAME_OBJECT_TASK_TASK_H__
#define __DHSH_GAMEDLL_GAME_OBJECT_TASK_TASK_H__

#include "BaseObject.h"

class CTaskObject : public CBaseObject
{
public:
	enum class em_Task_Gang_Type
	{
		em_Task_Gang_Type_None,
		// ��������
		em_Task_Gang_Type_MakeEqui,
		// ��ɱǿ��
		em_Task_Gang_Type_KillMonster,
	};

	struct BangTaskNpc
	{
		std::wstring wsNpcName;
		Point        NpcPoint;
	};
public:
	CTaskObject(_In_ DWORD dwNodeBase);
	~CTaskObject() = default;

	virtual VOID SetName() override;

	// �������ʾ����
	std::wstring GetTaskContent() CONST;

	// �����������
	em_Task_Gang_Type GetBangTaskType() CONST;

	// ��������Npc
	BangTaskNpc GetBangTaskNpc() CONST;

private:
	
};



#endif // !__DHSH_GAMEDLL_GAME_OBJECT_TASK_TASK_H__
