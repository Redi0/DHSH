#include "stdafx.h"
#include "PurifyWaterTask.h"
#include <MyTools/Log.h>
#include <MyTools/CLPublic.h>
#include "PlayerMove.h"
#include "GameVariable.h"
#include "Npc.h"
#include "NpcExtend.h"
#include "GameUiExtend.h"
#include "PersonAttribute.h"
#include "Task.h"
#include "TaskExtend.h"
#include "BagItemExtend.h"
#include "ResText.h"
#include "ExcuteAction.h"
#include "GameCALL.h"
#include "ScriptServices.h"

#define _SELF L"PurifyWaterTask.cpp"
BOOL CPurifyWaterTask::Run()
{
	while (GameRun && CScriptServices::CommonCheck())
	{
		CTaskObject TaskObject(NULL);
		if (!ExistTask(&TaskObject))
		{
			if (!PickTask())
			{
				LOG_CF_E(L"�Ӿ�ˮ����ʧ��!");
				break;
			}
			continue;
		}

		if (!BuyItem(GetItemName(TaskObject)))
		{
			LOG_CF_E(L"�����������ʧ��~");
			break;
		}

		if (!SumitTask())
		{
			LOG_CF_E(L"��������������ʧ��~");
			break;
		}
	}
	return TRUE;
}

BOOL CPurifyWaterTask::PickTask() CONST
{
	if (!MyTools::InvokeClassPtr<CPlayerMove>()->MoveToResNpc(L"�����", L"���ǡ������"))
	{
		LOG_CF_E(L"�ߵ�������Ա�ʧ��!");
		return FALSE;
	}

	BOOL bOpenDlg = FALSE;
	MyTools::InvokeClassPtr<CNpcExtend>()->FindNpc_By_Name_ExcutePtr(L"���ǡ������", [&bOpenDlg](CONST CNpc& Npc)
	{
		if (Npc.OpenNpcDlg())
		{
			bOpenDlg = TRUE;
			if (MyTools::InvokeClassPtr<CGameUiExtend>()->FindText_In_NpcDlg(L"�������Ľ���"))
			{
				LOG_CF_D(L"��ˮ�������!");
				return;
			}
			else if (MyTools::InvokeClassPtr<CGameUiExtend>()->FindText_In_NpcDlg(L"����"))
			{
				StopGame;
				LOG_MSG_CF(L"%s", MyTools::InvokeClassPtr<CGameUiExtend>()->GetNpcDlgText().c_str());
				return;
			}

			Npc.ClickOption_Once(L"WZCJ,2");
			if (MyTools::InvokeClassPtr<CGameVariable>()->GetRefValue_By_Id(em_TextVar::em_TextVar_PurifyWater_PromiseType) == 0)
			{
				LOG_CF_D(L"��Ǯȥ�Ӿ�ˮ����");
				Npc.ClickOption_Once(L"WZCJ,4");
			}
			else
			{
				LOG_CF_D(L"���ﹱȥ�Ӿ�ˮ����");
				Npc.ClickOption_Once(L"WZCJ,3");
			}
		}
	});
	if (!bOpenDlg)
	{
		LOG_CF_E(L"��Npc[���ǡ������] ʧ��!");
		return FALSE;
	}

	if (MyTools::InvokeClassPtr<CGameUiExtend>()->IsShowNpcDlg())
		MyTools::InvokeClassPtr<CGameUiExtend>()->CloseNpcDlg();

	return TRUE;
}

std::wstring CPurifyWaterTask::GetItemName(_In_ CONST CTaskObject& TaskObject) CONST
{
	std::wstring wsText = TaskObject.GetTaskContent();
	wsText = wsText.substr(wsText.find(L"ȥ��һ��") + 4);
	return wsText.substr(0, wsText.find(L"��������"));
}

BOOL CPurifyWaterTask::ExistTask(_Out_ CTaskObject* pTaskObject) CONST
{
	return MyTools::InvokeClassPtr<CTaskExtend>()->ExistTask_By_PartName_Action(L"[�]���ʳＯ", [&pTaskObject](CONST CTaskObject& TaskObject)
	{
		MyTools::CLPublic::SetPtr(pTaskObject, TaskObject);
	});;
}

BOOL CPurifyWaterTask::BuyItem(_In_ CONST std::wstring& wsItemName) CONST
{
	LOG_CF_D(L"׼��������Ʒ:%s", wsItemName.c_str());

	CResText::ResShopEquiText ShopEquiText;
	if (!MyTools::InvokeClassPtr<CBagItemExtend>()->GetShopEquiResText(wsItemName, ShopEquiText))
	{
		LOG_MSG_CF(L"��Ʒ:[%s]��û��¼~��ϵ������Ӹ���Ʒ!", wsItemName.c_str());
		return FALSE;
	}

	std::wstring wsNpcName;
	if (!MoveToShopNpc(ShopEquiText.dwStoreIndex, wsNpcName))
	{
		LOG_CF_E(L"�ߵ��̵�Npc[%s]��ʧ��!", wsNpcName.c_str());
		return FALSE;
	}
	
	BOOL bOpenDlg = FALSE;
	BOOL bClickOption = FALSE;
	DWORD dwNpcId = 0;
	MyTools::InvokeClassPtr<CNpcExtend>()->FindNpc_By_Name_ExcutePtr(wsNpcName, [&bOpenDlg,&dwNpcId, &bClickOption](CONST CNpc& Npc)
	{
		if (Npc.GetDis() > 6)
			MyTools::InvokeClassPtr<CPlayerMove>()->MoveToPoint(Npc.GetPoint());
		if (Npc.OpenNpcDlg())
		{
			bOpenDlg = TRUE;
			bClickOption = Npc.ClickOption(L"main1", L"shop_npcbuy");
			dwNpcId = Npc.GetId();
		}
	});

	if (!bOpenDlg)
	{
		LOG_CF_E(L"��Npc[%s] ʧ��!", wsNpcName.c_str());
		return FALSE;
	}
	if (!bClickOption)
	{
		LOG_CF_E(L"���Npcѡ��[main1] ʧ��!");
		return FALSE;
	}

	MyTools::InvokeClassPtr<CExcuteAction>()->PushPtrToMainThread([dwNpcId, ShopEquiText]
	{
		MyTools::InvokeClassPtr<CGameCALL>()->Buy_By_Npc(dwNpcId, ShopEquiText.dwItemIndex, 1);
	});

	GameSleep(3000);
	MyTools::InvokeClassPtr<CGameUiExtend>()->CloseDlg(L"shop_npcbuy");
	return TRUE;
}

BOOL CPurifyWaterTask::MoveToShopNpc(_In_ DWORD dwStoreIndex, _Out_ std::wstring& wsNpcName) CONST
{
	switch (static_cast<CResText::em_StoreIndex>(dwStoreIndex))
	{
	case CResText::em_StoreIndex::em_StoreIndex_Ying_Weaspon:
		wsNpcName = L"��Ӧ���������ϰ�";
		if (!MyTools::InvokeClassPtr<CPlayerMove>()->MoveToSpecialMap(L"Ӧ�츮", Point(390, 218), L"������"))
		{
			LOG_CF_E(L"�ߵ�[������]ʧ��!");
			return FALSE;
		}
		break;
	case CResText::em_StoreIndex::em_StoreIndex_Ying_Defence:
		wsNpcName = L"��Ӧ�����ߵ��ϰ�";
		if (!MyTools::InvokeClassPtr<CPlayerMove>()->MoveToSpecialMap(L"Ӧ�츮", Point(441, 236), L"���ߵ�"))
		{
			LOG_CF_E(L"�ߵ�[���ߵ�]ʧ��!");
			return FALSE;
		}
		break;
	case CResText::em_StoreIndex::em_StoreIndex_Ying_Jewelry:
		wsNpcName = L"��Ӧ����Ʒ���ϰ�";
		if (!MyTools::InvokeClassPtr<CPlayerMove>()->MoveToSpecialMap(L"Ӧ�츮", Point(368, 279), L"��Ʒ��"))
		{
			LOG_CF_E(L"�ߵ�[��Ʒ��]ʧ��!");
			return FALSE;
		}
		break;
	case CResText::em_StoreIndex::em_StoreIndex_Xing_Weaspon:
		wsNpcName = L"���ǡ��������ϰ�";
		if (!MyTools::InvokeClassPtr<CPlayerMove>()->MoveToSpecialMap(L"�����", Point(252, 112), L"������"))
		{
			LOG_CF_E(L"�ߵ�[������]ʧ��!");
			return FALSE;
		}
		break;
	case CResText::em_StoreIndex::em_StoreIndex_Xing_Defence:
		wsNpcName = L"���ǡ����ߵ��ϰ�";
		if (!MyTools::InvokeClassPtr<CPlayerMove>()->MoveToSpecialMap(L"�����", Point(221, 179), L"���ߵ�"))
		{
			LOG_CF_E(L"�ߵ�[���ߵ�]ʧ��!");
			return FALSE;
		}
		break;
	case CResText::em_StoreIndex::em_StoreIndex_Xing_Jewelry:
		wsNpcName = L"���ǡ���Ʒ���ϰ�";
		if (!MyTools::InvokeClassPtr<CPlayerMove>()->MoveToSpecialMap(L"�����", Point(31, 41), L"��Ʒ��"))
		{
			LOG_CF_E(L"�ߵ�[��Ʒ��]ʧ��!");
			return FALSE;
		}
		break;
	default:
		LOG_MSG_CF(L"��Ч���̵�����[%d],��ϵ�����޸�!", dwStoreIndex);
		break;
	}
	GameSleep(3 * 1000);
	return TRUE;
}

BOOL CPurifyWaterTask::SumitTask() CONST
{
	return PickTask();
}
