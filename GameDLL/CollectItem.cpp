#include "stdafx.h"
#include "CollectItem.h"
#include <MyTools/Log.h>
#include <MyTools/CLPublic.h>
#include "GameVariable.h"
#include "BagItemExtend.h"
#include "LogicBagItemAction.h"
#include "PersonAttribute.h"
#include "PersonPetAction.h"
#include "ResText.h"
#include "PlayerMove.h"
#include "NpcExtend.h"
#include "Player.h"
#include "Npc.h"
#include "BagItemAction.h"
#include "FarmMonster.h"
#include "TextVariable.h"
#include "ExcuteAction.h"
#include "GameCALL.h"
#include "TextVariable.h"
#include "ItemFilter.h"
#include "ScriptServices.h"

#define _SELF L"CollectItemc.pp"

BOOL CCollectItem::Run() CONST
{
	if (!Check())
		return FALSE;

	CONST std::wstring wsMapName = MyTools::InvokeClassPtr<CTextVariable>()->GetRefValue_By_Id(em_TextVar::em_TextVar_Collect_MapName);
	CONST std::wstring wsCollectItemName = MyTools::InvokeClassPtr<CTextVariable>()->GetRefValue_By_Id(em_TextVar::em_TextVar_Collect_ItemName);

	if (wsMapName == L"��԰")
		return Collect_In_Home(wsCollectItemName);

	auto pVecPath = GetCollectPath_By_MapName(wsMapName);
	if (pVecPath == nullptr)
	{
		LOG_CF_E(L"�����ڵ�ͼ[%s]�Ĳɼ�·��������ϵ�Ϸ����!", wsMapName.c_str());
		return FALSE;
	}

	CONST auto pPersonAttribute = MyTools::InvokeClassPtr<CPersonAttribute>();
	if (pPersonAttribute->GetCurrentMapName() != wsMapName)
	{
		LOG_CF_D(L"�ܵ��ɼ���ȥ!");
		MyTools::InvokeClassPtr<CPlayerMove>()->MoveToMapPoint(wsMapName, pVecPath->at(0));
	}

	while (GameRun)
	{
		for (CONST auto& itm : *pVecPath)
		{
			while (GameRun && pPersonAttribute->GetDis(itm) > 2)
			{
				GameSleep(1000);

				if (MyTools::InvokeClassPtr<CBagItemExtend>()->IsBagFull())
				{
					LOG_CF_E(L"��������!");
					StopGame;
					break;
				}

				// Check��ħ��, �Ƿ�����������
				if ((!MyTools::InvokeClassPtr<CLogicBagItemAction>()->CheckExorcism() || GetPetDrinksCount() == 0) && !Check())
				{
					LOG_CF_E(L"�ɼ������в�������ʧ�ܡ���");
					return FALSE;
				}

				if (pPersonAttribute->IsWar())
				{
					LOG_CF_D(L"�ɼ������з���ս���ˡ���");
					MyTools::InvokeClassPtr<CFarmMonster>()->Fight();
					continue;
				}

				// Check Answer!
				MyTools::InvokeClassPtr<CPlayerMove>()->FindDlg_By_BindMethod();

				if (!pPersonAttribute->IsMoving())
				{
					// 
					CheckAroundCollectItem(wsCollectItemName);
					continue;
				}

				MyTools::InvokeClassPtr<CPlayerMove>()->Move(wsMapName, itm);
				GameSleep(1000);
			}
		}
	}
	return TRUE;
}

BOOL CCollectItem::Check() CONST
{
	// ����������
	if (GetPetDrinksCount() == 0 && !SupplementPetDrinks())
	{
		LOG_CF_D(L"�����������ʧ��!");
		return FALSE;
	}

	if (!MyTools::InvokeClassPtr<CItemFilter>()->ReadItemFilterFile())
		return FALSE;

	return CScriptServices::CommonCheck();
}

DWORD CCollectItem::GetPetDrinksCount() CONST
{
	return MyTools::InvokeClassPtr<CBagItemExtend>()->GetCount_By_ItemName(L"��������");
}

BOOL CCollectItem::SupplementPetDrinks() CONST
{
	DWORD dwCount = MyTools::InvokeClassPtr<CGameVariable>()->GetRefValue_By_Id(em_TextVar::em_TextVar_AutoBuyPetDrinksCount);
	LOG_CF_D(L"������Ҫ���������������=%d", dwCount);

	dwCount -= GetPetDrinksCount();
	LOG_CF_D(L"��ȥ�����Ļ���,��Ҫ���������������=%d", dwCount);

	if (MyTools::InvokeClassPtr<CBagItemExtend>()->IsBagFull(dwCount / 30 + 1))
	{
		LOG_CF_E(L"�������ˡ���װ������ô��ĳ�������");
		return FALSE;
	}

	return MyTools::InvokeClassPtr<CLogicBagItemAction>()->SupplementItem(L"��������", dwCount);
}

CONST std::vector<Point>* CCollectItem::GetCollectPath_By_MapName(_In_ CONST std::wstring& wsMapName) CONST
{
	auto pVec = MyTools::InvokeClassPtr<CResText>()->GetStructPtr<CONST std::vector<CResText::ResCollectItemPath>*>(L"VecResCollectItemPath");
	if (pVec == nullptr)
		return nullptr;

	auto p = MyTools::CLPublic::Vec_find_if_Const(*pVec, [wsMapName](CONST auto& itm) { return itm.wsMapName == wsMapName; });
	return p == nullptr ? nullptr : &p->VecPoint;
}

BOOL CCollectItem::CheckAroundCollectItem(_In_ CONST std::wstring& wsCollectItemName) CONST
{
	// Collect Around ResItem
	BOOL bExist = MyTools::InvokeClassPtr<CNpcExtend>()->FindPlayer_By_Condition_ExcuteAction([wsCollectItemName](CONST CPlayer& ResItem)
	{
		return wsCollectItemName == ResItem.GetName();;
	}, [](CONST CNpc& ResItem) 
	{
		LOG_CF_D(L"Collecting~");
		ResItem.Collect();
	});

	// Supplement
	UsePetDrinks();

	// Check Bag! Drop Some Item
	MyTools::InvokeClassPtr<CBagItemAction>()->DropItem();
	return bExist;
}

BOOL CCollectItem::UsePetDrinks() CONST
{
	if (MyTools::InvokeClassPtr<CPersonAttribute>()->GetPetPhysicalStrength() < 50)
	{
		LOG_CF_D(L"���������<50, ����һ������!");
		for (int i = 0; GameRun && i < 3 && MyTools::InvokeClassPtr<CBagItemAction>()->UseItem(L"��������"); ++i)
			GameSleep(1000);
	}
	return TRUE;
}

VOID CCollectItem::SetDefaultValue() CONST
{
	
}

BOOL CCollectItem::Collect_In_Home(_In_ CONST std::wstring& wsCollectItemName) CONST
{
	if (!MyTools::InvokeClassPtr<CPersonAttribute>()->IsInHome(TRUE) && !MyTools::InvokeClassPtr<CPlayerMove>()->MoveToHome())
	{
		LOG_CF_E(L"�ؼ�԰ʧ�ܡ���");
		return FALSE;
	}

	//em_TextVar_CollectHome_Furniture
	std::wstring wsFurnitureName = MyTools::InvokeClassPtr<CTextVariable>()->GetRefValue_By_Id(em_TextVar::em_TextVar_CollectHome_Furniture);

	auto pVec = MyTools::InvokeClassPtr<CResText>()->GetStructPtr<CONST std::vector<CResText::ResNameId>*>(L"VecHomeRes");
	if (pVec == nullptr)
		return FALSE;

	auto pElement = MyTools::CLPublic::Vec_find_if_Const(*pVec, [wsFurnitureName](CONST auto& itm) { return itm.wsResName == wsFurnitureName; });
	if (pElement == nullptr)
	{
		LOG_MSG_CF(L"�Ҿ�[%s]��û�м�¼������ϵ�Ϸ����һ��!", wsFurnitureName.c_str());
		return FALSE;
	}

	return Collect_In_Courtyard(pElement->dwResId, wsCollectItemName);
}

BOOL CCollectItem::CollectFurniture(_In_ DWORD dwResId) CONST
{
	while (GameRun && MyTools::InvokeClassPtr<CPersonAttribute>()->GetCurrentMapName() == L"ͥԺ")
	{
		LOG_CF_D(L"��ͥԺ�ص�סլ����");
		MyTools::InvokeClassPtr<CExcuteAction>()->PushPtrToMainThread([]
		{
			MyTools::InvokeClassPtr<CGameCALL>()->MoveToPoint_Mouse(Point(1, 26));
		});
		GameSleep(2000);
	}

	BOOL bExist = MyTools::InvokeClassPtr<CNpcExtend>()->FindPlayer_By_Condition_ExcuteAction([dwResId](CONST CPlayer& itm) { return itm.GetResId() == dwResId; }, [](CONST CNpc& itm) 
	{
		itm.CollectFurniture();
	});

	if (!bExist && GameRun)
	{
		LOG_MSG_CF(L"�ɼ��ļҾ߲�����!!!");
		return FALSE;
	}

	GameSleep(2 * 1000);
	return TRUE;
}

BOOL CCollectItem::Collect_In_Courtyard(_In_ DWORD dwResId, _In_ CONST std::wstring& wsCollectItemName) CONST
{
	while (GameRun)
	{
		GameSleep(1000);
		if (MyTools::InvokeClassPtr<CPersonAttribute>()->GetHomeVitalityValue() < 20)
		{
			LOG_CF_D(L"����ֵ<20,�ȴ��С���");
			GameSleep(10 * 1000);
			continue;
		}

		if (MyTools::InvokeClassPtr<CPersonAttribute>()->GetCurrentMapName() != L"ͥԺ")
		{
			// Move To Yard
			BOOL bExist = MyTools::InvokeClassPtr<CNpcExtend>()->FindPlayer_By_Condition_ExcuteAction([](CONST CPlayer& itm) { return itm.GetName().find(L"�Ĺܼ�") != -1; }, [](CONST CNpc& Npc)
			{
				MyTools::InvokeClassPtr<CPlayerMove>()->MoveToPoint(Npc.GetPoint());
				if (Npc.OpenNpcDlg())
				{
					GameSleep(1000);
					Npc.CLickOption_DisableDlg(L"Move2Garden", L"npcdlg");
				}
				
			});

			if (!bExist)
			{
				LOG_MSG_CF(L"סլ�ܼ��Ҳ����ˡ���");
				return FALSE;
			}
			continue;
		}

		if (GetPetDrinksCount() == 0 && Check())
		{
			LOG_CF_E(L"�ɼ������в�������ʧ�ܡ���");
			return FALSE;
		}

		if (!CheckAroundCollectItem(wsCollectItemName))
		{
			// UnExist Res Packet
			LOG_CF_D(L"��Χû����Դ���ˡ���ȥ�ɼ��Ҿ�!");
			CollectFurniture(dwResId);
		}
	}
	return TRUE;
}
