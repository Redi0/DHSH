#ifndef __DHSH_GAMEDLL_GAME_METHOD_ACTION_LOGICBAGITEMACTION_H__
#define __DHSH_GAMEDLL_GAME_METHOD_ACTION_LOGICBAGITEMACTION_H__

#include "GameBase.h"

class CLogicBagItemAction : public MyTools::CRelfexBaseClass
{
public:
	CLogicBagItemAction() = default;
	~CLogicBagItemAction() = default;
	
	// ս�������Ʒ����
	BOOL AfterFight_Item() CONST;

	// ��ħ����
	BOOL CheckExorcism() CONST;

	// װ���־öȼ��?
	
public:
	static CLogicBagItemAction* CreateInstance()
	{
		return new CLogicBagItemAction;
	}
	
	virtual VOID ReleaseInstance(_In_ LPVOID lpObjectAddr) CONST
	{
		delete reinterpret_cast<CLogicBagItemAction*>(lpObjectAddr);
	}

	static CONST std::wstring& GetClassName_()
	{
		static CONST std::wstring wsClassName = L"CLogicBagItemAction";
		return wsClassName;
	}

	CLogicBagItemAction(CONST CLogicBagItemAction&) = delete;

	CLogicBagItemAction& operator=(CONST CLogicBagItemAction&) = delete;
};

#endif // !__DHSH_GAMEDLL_GAME_METHOD_ACTION_LOGICBAGITEMACTION_H__
