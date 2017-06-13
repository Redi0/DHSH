#include "stdafx.h"
#include "Examination.h"
#include <MyTools/Log.h>
#include <MyTools/Character.h>
#include <MyTools/CLPublic.h>
#include "Task.h"
#include "TaskExtend.h"
#include "PlayerMove.h"
#include "Npc.h"
#include "GameUi.h"
#include "GameUiExtend.h"
#include "NpcExtend.h"
#include "ScriptServices.h"

#define _SELF L"Examination.cpp"

BOOL CExamination::Run()
{
	CTaskObject TaskObject(NULL);
	if (!Signup())
	{
		LOG_MSG_CF(L"����ʧ��!");
		return TRUE;
	}
	/*if (!ExistTask(&TaskObject))
	{
		LOG_MSG_CF(L"��ȡ����ʧ��!");
		return TRUE;
	}*/

	MapLocation MapLocation_;
	/*if (!GetTaskMapLocation(&TaskObject, MapLocation_))
	{
		LOG_MSG_CF(L"��ȡ���Եص�ʧ��! Text=%s", TaskObject.GetTaskContent().c_str());
		return TRUE;
	}*/

	while (GameRun && CScriptServices::CommonCheck() && ExistTask(&TaskObject) && GetTaskMapLocation(&TaskObject, MapLocation_) && AnswerTaskQuestion(MapLocation_));
	return TRUE;
}

BOOL CExamination::Signup()
{
	// Exist Task
	if (ExistTask(nullptr))
		return TRUE;

	if (!MyTools::InvokeClassPtr<CPlayerMove>()->MoveToSpecialMap(L"Ӧ�츮", Point(441, 130),L"����"))
		return FALSE;

	GameSleep(3 * 1000);

	struct NpcOptionText
	{
		std::wstring wsDlgPartText1;
		std::wstring wsDlgPartText2;
		std::wstring wsNpcOptionText;
	};

	CONST static std::vector<NpcOptionText> Vec =
	{
		{ L"��֮͢��",L"����",L"XiangShi" },
		{ L"���˳ƺ�",L"",L"ShengShi,3" },
		{ L"�ؿ�����",L"",L"ShengShi,2" },
	};

	BOOL bOpenDlg = FALSE;
	MyTools::InvokeClassPtr<CNpcExtend>()->FindNpc_By_Name_ExcutePtr(L"�ƾٱ�����Ա", [&bOpenDlg](CONST CNpc& Npc)
	{
		if (Npc.OpenNpcDlg())
		{
			bOpenDlg = TRUE;

			auto p = MyTools::CLPublic::Vec_find_if_Const(Vec, [](CONST auto& itm) 
			{
				return MyTools::InvokeClassPtr<CGameUiExtend>()->FindText_In_NpcDlg(itm.wsDlgPartText1) && (itm.wsDlgPartText2.empty() || MyTools::InvokeClassPtr<CGameUiExtend>()->FindText_In_NpcDlg(itm.wsDlgPartText2));
			});

			if (p == nullptr)
				return;


			LOG_CF_D(L"���Npcѡ��:[%s,%s,%s]", p->wsDlgPartText1.c_str(), p->wsDlgPartText2.c_str(), p->wsNpcOptionText.c_str());
			Npc.ClickOption_Once(p->wsNpcOptionText);
		}
	});
	if (!bOpenDlg)
	{
		LOG_CF_E(L"��Npc[�ƾٱ�����Ա] ʧ��!");
		return FALSE;
	}

	GameSleep(2 * 1000);
	return ExistTask(nullptr);
}

BOOL CExamination::ExistTask(_Out_ CTaskObject* pTaskObject) CONST
{
	auto fnSetTaskObjectPtr = [&pTaskObject](CONST CTaskObject& Obj)
	{
		MyTools::CLPublic::SetPtr(pTaskObject, Obj);
	};
	return MyTools::InvokeClassPtr<CTaskExtend>()->ExistTask_By_PartName_Action(L"#C20[�]����", fnSetTaskObjectPtr) || 
		   MyTools::InvokeClassPtr<CTaskExtend>()->ExistTask_By_PartName_Action(L"#C20[�]����", fnSetTaskObjectPtr);
}

BOOL CExamination::GetTaskMapLocation(_In_ CONST CTaskObject* pTaskObject, _Out_ MapLocation& MapLocation_) CONST
{
	auto wsContent = pTaskObject->GetTaskContent();
	
	if (wsContent.find(L"��ȥ") == -1)
		return FALSE;

	MyTools::CCharacter::GetRemoveRight(wsContent, L"��ȥ", MapLocation_.wsMapName);
	MyTools::CCharacter::GetRemoveLeft(MapLocation_.wsMapName, L"��", MapLocation_.wsMapName);

	std::wstring wsPoint;
	MyTools::CCharacter::GetRemoveRight(wsContent, L"��", wsPoint);
	MyTools::CCharacter::GetRemoveLeft(wsPoint, L"��", wsPoint);
	if (wsPoint.find(L"��") == -1)
		return FALSE;

	MapLocation_.TarPoint.X = std::stoi(wsPoint.substr(0, wsPoint.find(L"��")));
	MapLocation_.TarPoint.Y = std::stoi(wsPoint.substr(wsPoint.find(L"��") + 1));

	MyTools::CCharacter::GetRemoveRight(wsContent, L"{", MapLocation_.wsNpcName);
	MyTools::CCharacter::GetRemoveLeft(MapLocation_.wsNpcName, L"|", MapLocation_.wsNpcName);

	LOG_CF_D(L"���Եص�:[%s], Npc:[%s], Point:[%d,%d]", MapLocation_.wsMapName.c_str(), MapLocation_.wsNpcName.c_str(), MapLocation_.TarPoint.X, MapLocation_.TarPoint.Y);
	return TRUE;
}

BOOL CExamination::AnswerTaskQuestion(_In_ CONST MapLocation& MapLocation_) CONST
{
	if (!MyTools::InvokeClassPtr<CPlayerMove>()->MoveToMapPoint(MapLocation_.wsMapName, MapLocation_.TarPoint))
		return FALSE;

	BOOL bOpenDlg = FALSE;
	MyTools::InvokeClassPtr<CNpcExtend>()->FindNpc_By_Name_ExcutePtr(MapLocation_.wsNpcName, [&bOpenDlg](CONST CNpc& Npc)
	{
		if (Npc.OpenNpcDlg())
		{
			bOpenDlg = TRUE;
			Npc.ClickOption_Once(L"Result,1");
		}
	});
	if (!bOpenDlg)
	{
		LOG_CF_E(L"��Npc[�ƾٱ�����Ա] ʧ��!");
		return FALSE;
	}

	GameSleep(1 * 1000);
	if(MyTools::InvokeClassPtr<CGameUiExtend>()->IsShowNpcDlg())
		MyTools::InvokeClassPtr<CGameUiExtend>()->CloseNpcDlg();
	GameSleep(1 * 1000);
	return TRUE;
}
