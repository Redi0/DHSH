#ifndef __DHSH_GAMEDLL_GAME_METHOD_FARM_FARMMONSTER_H__
#define __DHSH_GAMEDLL_GAME_METHOD_FARM_FARMMONSTER_H__

#include "GameBase.h"

class CFarmMonster : public MyTools::CRelfexBaseClass
{
public:
	CFarmMonster() = default;
	~CFarmMonster() = default;
	
	BOOL Fight() CONST;
	
private:
	enum class em_FightSwitch
	{
		em_FightSwitch_None,
		em_FightSwitch_Person,
		em_FightSwitch_Pet
	};
	// ս��ʱ��
	em_FightSwitch GetFightSwitch() CONST;

	// �����ս���ж�
	VOID Fight_Person() CONST;

	// �����ս���ж�
	VOID Fight_Pet() CONST;

	//
	enum class em_MouseShape
	{
		em_MouseShape_None,
		em_MouseShape_Skill
	};
	// ս�������״
	em_MouseShape GetPersonMouseShape() CONST;
	em_MouseShape GetPetMouseShape() CONST;

	// 
	DWORD GetRoundOrder() CONST;

public:
	static CFarmMonster* CreateInstance()
	{
		return new CFarmMonster;
	}
	
	virtual VOID ReleaseInstance(_In_ LPVOID lpObjectAddr) CONST
	{
		delete reinterpret_cast<CFarmMonster*>(lpObjectAddr);
	}

	static CONST std::wstring& GetClassName_()
	{
		static CONST std::wstring wsClassName = L"CFarmMonster";
		return wsClassName;
	}
};

#endif // !__DHSH_GAMEDLL_GAME_METHOD_FARM_FARMMONSTER_H__
