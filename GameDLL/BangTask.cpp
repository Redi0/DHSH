#include "stdafx.h"
#include "BangTask.h"
#include <MyTools/Log.h>
#include <MyTools/TimeTick.h>
#include "BagItemExtend.h"
#include "LogicBagItemAction.h"
#include "GameVariable.h"
#include "PlayerMove.h"
#include "NpcExtend.h"
#include "Npc.h"
#include "PersonAttribute.h"
#include "GameUiExtend.h"
#include "Task.h"
#include "TaskExtend.h"

#define _SELF L"BangTask.cpp"
BOOL CBangTask::Check() CONST
{
	if (MyTools::InvokeClassPtr<CBagItemExtend>()->GetCount_By_ItemName(L"��ħ��") == 0)
	{
		LOG_CF_D(L"�������Զ�������ħ��, �������ϲ���������ħ��! ȥ����ħ��");
		if (!MyTools::InvokeClassPtr<CLogicBagItemAction>()->SupplementItem(L"��ħ��", 10))
		{
			LOG_CF_E(L"�Զ�������ħ��ʧ�ܡ���");
			return FALSE;
		}
	}

	if (MyTools::InvokeClassPtr<CBagItemExtend>()->GetCount_By_ItemPartName(L"���̷�") == 0 && MyTools::InvokeClassPtr<CGameVariable>()->GetRefValue_By_Id(em_TextVar::em_TextVar_AutoBuyReturnSymbol))
	{
		LOG_CF_D(L"�������Զ����򳬳̷�, ���ϲ���, ȥ���򳬳̷�!");
		if (!MyTools::InvokeClassPtr<CLogicBagItemAction>()->SupplementItem(L"Ӧ�츮���̷�", 1))
		{
			LOG_CF_E(L"�Զ����򳬳̷�ʧ�ܡ���");
			return FALSE;
		}
	}
	
	// 
	if (!MyTools::InvokeClassPtr<CBagItemExtend>()->IsBagFull(2))
	{
		LOG_MSG_CF(L"��������Ҫ��2������!");
		return FALSE;
	}
	return TRUE;
}

BOOL CBangTask::MoveToBang() CONST
{
	if (!MyTools::InvokeClassPtr<CPlayerMove>()->MoveToResNpc(L"���ɴ���Ա"))
	{
		LOG_CF_E(L"�ߵ����ɴ���Աʧ��!");
		return FALSE;
	}

	DWORD dwNpcId = 0;
	BOOL bOpenDlg = FALSE;
	BOOL bClickOption = FALSE;
	MyTools::InvokeClassPtr<CNpcExtend>()->FindNpc_By_Name_ExcutePtr(L"���ɴ���Ա", [&bOpenDlg, &bClickOption, &dwNpcId](CONST CNpc& Npc)
	{
		if (Npc.OpenNpcDlg())
		{
			bOpenDlg = TRUE;
			bClickOption = Npc.CLickOption_DisableDlg(L"enter_HQ", L"npcdlg");
		}
	});
	if (!bOpenDlg)
	{
		LOG_CF_E(L"��Npc[���ɴ���Ա] ʧ��!");
		return FALSE;
	}

	if (!bClickOption)
	{
		LOG_CF_E(L"���Npcѡ��[�ص��Լ�����] ʧ��!");
		return FALSE;
	}

	GameSleep(3 * 1000);
	return MyTools::InvokeClassPtr<CPersonAttribute>()->GetCurrentMapName() == L"����";
}

BOOL CBangTask::MoveToManagerNpc() CONST
{
	if (!MoveToBang())
		return FALSE;

	return MyTools::InvokeClassPtr<CPlayerMove>()->MoveToSpecialMap(L"����", Point(21, 30), L"���");
}

BOOL CBangTask::ExistBangTask() CONST
{
	return MyTools::InvokeClassPtr<CTaskExtend>()->ExistTask_By_PartName_Action(L"[����]", nullptr);
}

BOOL CBangTask::PickBangTask() CONST
{
	if (!MoveToManagerNpc())
		return FALSE;

	MyTools::CTimeTick TimeTick;
	while (GameRun && TimeTick.GetSpentTime(MyTools::CTimeTick::em_TimeTick::em_TimeTick_Minute) < 7 && !ExistBangTask())
	{
		DWORD dwNpcId = 0;
		BOOL bOpenDlg = FALSE;
		BOOL bClickOption = FALSE;
		MyTools::InvokeClassPtr<CNpcExtend>()->FindNpc_By_Name_ExcutePtr(L"������Ա", [&bOpenDlg, &bClickOption, &dwNpcId](CONST CNpc& Npc)
		{
			if (Npc.OpenNpcDlg())
			{
				bOpenDlg = TRUE;
				if (!Npc.CLickOption_DisableDlg(L"AcceptTask", L"npcdlg") && MyTools::InvokeClassPtr<CGameUiExtend>()->IsShowNpcDlg())
					MyTools::InvokeClassPtr<CGameUiExtend>()->CloseNpcDlg();
			}
		});
		if (!bOpenDlg)
		{
			LOG_CF_E(L"��Npc[������Ա] ʧ��!");
		}

		GameSleep(5 * 1000);
	}

	if (MyTools::InvokeClassPtr<CGameUiExtend>()->IsShowNpcDlg())
		MyTools::InvokeClassPtr<CGameUiExtend>()->CloseNpcDlg();

	return ExistBangTask();
}

BOOL CBangTask::Run()
{
	while (GameRun)
	{
		if (!PickBangTask())
		{
			LOG_MSG_CF(L"�Ӱ�������ʧ��!");
			break;
		}


	}

	return TRUE;
}

