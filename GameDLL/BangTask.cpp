#include "stdafx.h"
#include "BangTask.h"
#include <MyTools/Log.h>
#include <MyTools/TimeTick.h>
#include <MyTools/CLPublic.h>
#include <MyTools/Character.h>
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
#include "BagItem.h"
#include "FarmMonster.h"

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

	// ʹ����ħ��
	MyTools::InvokeClassPtr<CLogicBagItemAction>()->CheckExorcism();
	
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
	if (!Check())
		return FALSE;

	if (!MyTools::InvokeClassPtr<CPlayerMove>()->MoveToResNpc(L"Ӧ�츮", L"���ɴ���Ա"))
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
	if (MyTools::InvokeClassPtr<CPersonAttribute>()->GetCurrentMapName() != L"����" && !MoveToBang())
		return FALSE;

	LOG_C_D(L"�ߵ�[������԰����Ա]");
	if (!MyTools::InvokeClassPtr<CPlayerMove>()->MoveToPoint(Point(91, 93)))
		return FALSE;

	BOOL bOpenDlg = FALSE;
	MyTools::InvokeClassPtr<CNpcExtend>()->FindNpc_By_Name_ExcutePtr(L"������԰����Ա", [&bOpenDlg](CONST CNpc& Npc)
	{
		if (Npc.OpenNpcDlg())
		{
			bOpenDlg = TRUE;
			if (!Npc.CLickOption_DisableDlg(L"moveto,6", L"npcdlg") && MyTools::InvokeClassPtr<CGameUiExtend>()->IsShowNpcDlg())
				MyTools::InvokeClassPtr<CGameUiExtend>()->CloseNpcDlg();
		}
	});
	if (!bOpenDlg)
	{
		LOG_CF_E(L"��Npc[������԰����Ա] ʧ��!");
		return FALSE;
	}
	return TRUE;
}

BOOL CBangTask::ExistBangTask(_Out_ CTaskObject* pTaskObject) CONST
{
	return MyTools::InvokeClassPtr<CTaskExtend>()->ExistTask_By_PartName_Action(L"[����]", [&pTaskObject](CONST CTaskObject& TaskObject)
	{
		MyTools::CLPublic::SetPtr(pTaskObject, TaskObject);
	});;
}

BOOL CBangTask::MoveToBangDelivery() CONST
{
	// 43.37
	LOG_C_D(L"�߳����");
	if (!MyTools::InvokeClassPtr<CPlayerMove>()->MoveToSpecialMap(L"���", Point(43, 37), L"����"))
		return  FALSE;

	LOG_C_D(L"�ߵ�[������԰����Ա]");
	return MyTools::InvokeClassPtr<CPlayerMove>()->MoveToPoint(Point(91, 93));
}

std::wstring CBangTask::GetBangTaskRequestItemQuality(_In_ CONST CTaskObject& TaskObject) CONST
{
	std::wstring wsItemQuality = TaskObject.GetTaskContent();
	if (wsItemQuality.find(L"�����") == -1)
		return std::wstring();

	// �Ѱ��ɵĻ��콣�������Ʒ�ʽ���������Ա�����ص��������԰�����������ǰ��2�Σ�
	wsItemQuality = wsItemQuality.substr(wsItemQuality.find(L"�����") + 3);
	wsItemQuality = wsItemQuality.substr(0, wsItemQuality.find(L"Ʒ��"));
	return wsItemQuality;
}

BOOL CBangTask::Task_MakeEquiment(_In_ CONST CTaskObject& TaskObject, _In_ CONST std::wstring& wsItemQuality) CONST
{
	if (!MoveToBangDelivery())
		return FALSE;

	BOOL bOpenDlg = FALSE;
	MyTools::InvokeClassPtr<CNpcExtend>()->FindNpc_By_Name_ExcutePtr(L"������԰����Ա", [&bOpenDlg](CONST CNpc& Npc)
	{
		if (Npc.OpenNpcDlg())
		{
			bOpenDlg = TRUE;
			if (!Npc.CLickOption_DisableDlg(L"moveto,1", L"npcdlg") && MyTools::InvokeClassPtr<CGameUiExtend>()->IsShowNpcDlg())
				MyTools::InvokeClassPtr<CGameUiExtend>()->CloseNpcDlg();
		}
	});
	if (!bOpenDlg)
	{
		LOG_CF_E(L"��Npc[������԰����Ա] ʧ��!");
		return FALSE;
	}

	struct BangTask_MakeEquiment_Text
	{
		std::wstring wsMapName;
		std::wstring wsNpcName;
	};

	CONST static std::vector<BangTask_MakeEquiment_Text> Vec = 
	{
		{ L"Ӧ�츮",L"Ӧ�츮����" },
		{ L"�����",L"���������" },
		{ L"����С��",L"����С������" },
		{ L"������",L"����������" },
		{ L"���ׯ",L"���ׯ����" },
	};

	for (CONST auto& itm : Vec)
	{
		if (!Task_MoveToBlackSmith(itm.wsMapName, itm.wsNpcName))
			return FALSE;

		if (BangTask_IsFinishMakeEquiment(wsItemQuality))
		{
			LOG_CF_D(L"��������[���쾫Ʒ]���!");
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CBangTask::Task_MoveToBlackSmith(_In_ CONST std::wstring& wsMapName, _In_ CONST std::wstring& wsNpcName) CONST
{
	if (!MyTools::InvokeClassPtr<CPlayerMove>()->MoveToResNpc(wsMapName, wsNpcName))
		return FALSE;

	BOOL bOpenDlg = FALSE;
	MyTools::InvokeClassPtr<CNpcExtend>()->FindNpc_By_Name_ExcutePtr(wsNpcName, [&bOpenDlg](CONST CNpc& Npc)
	{
		if (Npc.OpenNpcDlg())
		{
			bOpenDlg = TRUE;
			Npc.ClickOption(L"CheckMake", L"npcdlg");
			Npc.CLickOption_DisableDlg(L"DoMake", L"npcdlg");
		}
	});
	if (!bOpenDlg)
	{
		LOG_CF_E(L"��Npc[%s] ʧ��!", wsNpcName.c_str());
		return FALSE;
	}

	return TRUE;
}

BOOL CBangTask::BangTask_IsFinishMakeEquiment(_In_ CONST std::wstring& wsItemQuality) CONST
{
	BOOL bFinish = FALSE;
	MyTools::InvokeClassPtr<CBagItemExtend>()->FindItem_By_PartName_To_ExcutePtr(L"���ɵ�", [wsItemQuality, &bFinish](CONST CBagItem& itm)
	{
		bFinish = itm.GetItemQuality() == wsItemQuality;
	});

	return bFinish;
}

BOOL CBangTask::KillRobber(_In_ CONST CTaskObject& TaskObject) CONST
{
	RobberContent RobberContent_;
	if (!GetRobberPoint(TaskObject, RobberContent_))
		return FALSE;

	if (!MoveToBangDelivery())
		return FALSE;

	if (!MyTools::InvokeClassPtr<CPlayerMove>()->MoveToMapPoint(RobberContent_.wsMapName, RobberContent_.TarPoint))
		return FALSE;

	BOOL bOpenDlg = FALSE;
	MyTools::InvokeClassPtr<CNpcExtend>()->FindNpc_By_Name_ExcutePtr(RobberContent_.wsMonsterName, [&bOpenDlg](CONST CNpc& Npc)
	{
		if (Npc.OpenNpcDlg())
		{
			bOpenDlg = TRUE;
			if (!Npc.CLickOption_DisableDlg(L"DoFight", L"npcdlg") && MyTools::InvokeClassPtr<CGameUiExtend>()->IsShowNpcDlg())
				MyTools::InvokeClassPtr<CGameUiExtend>()->CloseNpcDlg();
		}
	});

	if (!bOpenDlg)
	{
		LOG_CF_E(L"��Monster[%s] ʧ��!", RobberContent_.wsMonsterName.c_str());
		return FALSE;
	}

	if (!MyTools::InvokeClassPtr<CFarmMonster>()->Fight())
		return FALSE;

	GameSleep(2 * 1000);
	if (MyTools::InvokeClassPtr<CGameUiExtend>()->IsShowNpcDlg())
		MyTools::InvokeClassPtr<CGameUiExtend>()->CloseNpcDlg();

	return TRUE;
}

BOOL CBangTask::GetRobberPoint(_In_ CONST CTaskObject& TaskObject, _Out_ RobberContent& RobberContent_) CONST
{
	auto wsContent = TaskObject.GetTaskContent();

	static CONST std::vector<std::wstring> VecText = { L"[",L"]" };

	std::vector<std::wstring> VecResult;
	MyTools::CCharacter::GetSplit_By_List(wsContent, VecText, VecResult, Split_Option_RemoveEmptyEntries);
	if (VecResult.size() == 0)
	{
		LOG_CF_E(L"GetRobberPoint Faild! Text=%s", wsContent.c_str());
		return FALSE;
	}

	// ǿ���׸���|POSӦ�츮��|12|14
	std::vector<std::wstring> VecRobberContent;
	if (MyTools::CCharacter::Split(VecResult.at(0), L"|", VecRobberContent, Split_Option_RemoveEmptyEntries) != 4)
	{
		LOG_CF_E(L"VecResult=%s", VecResult.at(0).c_str());
		return FALSE;
	}

	RobberContent_.wsMonsterName = VecRobberContent.at(0);
	RobberContent_.wsMapName = VecRobberContent.at(1).substr(3);
	RobberContent_.TarPoint = Point(std::stoi(VecRobberContent.at(2)), std::stoi(VecRobberContent.at(3)));
	LOG_CF_D(L"ǿ������[%s], ���ڵ�ͼ[%s],����[%d,%d]", RobberContent_.wsMonsterName.c_str(), RobberContent_.wsMapName.c_str(), RobberContent_.TarPoint.X, RobberContent_.TarPoint.Y);
	return TRUE;
}

BOOL CBangTask::PickBangTask(_Out_ CTaskObject* pTaskObject) CONST
{
	if (MyTools::InvokeClassPtr<CPersonAttribute>()->GetCurrentMapName() != L"���" && !MoveToManagerNpc())
		return FALSE;

	MyTools::CTimeTick TimeTick;
	while (GameRun && TimeTick.GetSpentTime(MyTools::CTimeTick::em_TimeTick::em_TimeTick_Minute) < 7 && !ExistBangTask(nullptr))
	{
		DWORD dwNpcId = 0;
		BOOL bOpenDlg = FALSE;
		BOOL bClickOption = FALSE;
		MyTools::InvokeClassPtr<CNpcExtend>()->FindNpc_By_Name_ExcutePtr(L"������Ա", [&bOpenDlg, &bClickOption, &dwNpcId](CONST CNpc& Npc)
		{
			if (Npc.OpenNpcDlg())
			{
				bOpenDlg = TRUE;

				// get dlg Text = '���Ǹ����Ľ�����Ҫ��Ϊ���ɾ�Ӫ������Ŷ'
				if (MyTools::InvokeClassPtr<CGameUiExtend>()->FindText_In_NpcDlg(L"���Ǹ����Ľ���") || MyTools::InvokeClassPtr<CGameUiExtend>()->FindText_In_NpcDlg(L"����������")) // Show 
				{
					if (!Npc.CLickOption_DisableDlg(L"AcceptTask", L"npcdlg") && MyTools::InvokeClassPtr<CGameUiExtend>()->IsShowNpcDlg())
						MyTools::InvokeClassPtr<CGameUiExtend>()->CloseNpcDlg();
				}
				else
				{
					LOG_C_D(L"�����������Ҫ�ȴ�һ��ʱ�䡭�����õȴ�30s���ٽ�����");
					GameSleep(30 * 1000);
				}
			}
		});
		if (!bOpenDlg)
		{
			LOG_CF_E(L"��Npc[������Ա] ʧ��!");
		}

		GameSleep(1000);
	}

	if (MyTools::InvokeClassPtr<CGameUiExtend>()->IsShowNpcDlg())
		MyTools::InvokeClassPtr<CGameUiExtend>()->CloseNpcDlg();

	return ExistBangTask(pTaskObject);
}

BOOL CBangTask::Run()
{
	while (GameRun)
	{
		CTaskObject CurrentTaskObject(NULL);
		if (!PickBangTask(&CurrentTaskObject))
		{
			LOG_MSG_CF(L"�Ӱ�������ʧ��!");
			break;
		}

		switch (CurrentTaskObject.GetBangTaskType())
		{
		case CTaskObject::em_Task_Gang_Type::em_Task_Gang_Type_MakeEqui:	
			Task_MakeEquiment(CurrentTaskObject, GetBangTaskRequestItemQuality(CurrentTaskObject));
			break;
		case CTaskObject::em_Task_Gang_Type::em_Task_Gang_Type_KillMonster:
			KillRobber(CurrentTaskObject);
			break;
		default:
			break;
		}
	}

	return TRUE;
}

