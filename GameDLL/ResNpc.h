#ifndef __DHSH_GAMEDLL_GAME_OBJECT_RESNPC_NPC_H__
#define __DHSH_GAMEDLL_GAME_OBJECT_RESNPC_NPC_H__

#include "BaseObject.h"

class CResNpc : public CBaseObject
{
public:
	CResNpc();
	CResNpc(_In_ DWORD dwNodeBase);
	virtual ~CResNpc() = default;

	// Name
	virtual VOID SetName();

	// Npc���ڵ�ͼ
	CONST std::wstring GetNpcMapName() CONST;

	// ����
	Point GetPoint() CONST;

	// Npc���������ͼ
	CONST std::wstring GetNpcGlobleMapName() CONST;

	// ResId
	DWORD GetResId() CONST;

private:

};



#endif // !__DHSH_GAMEDLL_GAME_OBJECT_NPC_NPC_H__
