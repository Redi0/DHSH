#include "stdafx.h"
#include "PlayerMove.h"
#include <MyTools/Character.h>
#include <MyTools/Log.h>
#include <MyTools/TimeTick.h>
#include <MyTools/CLPublic.h>
#include "ExcuteAction.h"
#include "GameCALL.h"
#include "PersonAttribute.h"
#include "FarmMonster.h"
#include "BagItemAction.h"
#include "BagItemExtend.h"
#include "ResText.h"
#include "MapSearch.h"
#include "GameUiExtend.h"
#include "LogicBagItemAction.h"
#include "GameUi.h"
#include "NpcExtend.h"
#include "Npc.h"
#include "ResNpcExtend.h"

#define _SELF L"PlayerMove.cpp"
BOOL CPlayerMove::MoveToPoint(_In_ CONST Point& TarPoint) CONST
{
	return MoveToMapPoint(MyTools::InvokeClassPtr<CPersonAttribute>()->GetCurrentMapName(), TarPoint);
}

BOOL CPlayerMove::MoveToMapPoint(_In_ CONST std::wstring& wsMapName, _In_ CONST Point& TarPoint) CONST
{
	MyTools::CTimeTick TimeTick;
	CONST auto pPersonAttributePtr = MyTools::InvokeClassPtr<CPersonAttribute>();
	CONST auto pGameUiExtend = MyTools::InvokeClassPtr<CGameUiExtend>();
	
	while (GameRun && (MyTools::InvokeClassPtr<CPersonAttribute>()->GetCurrentMapName() != wsMapName || pPersonAttributePtr->GetDis(TarPoint) != 0.0f))
	{
		GameSleep(500);
		if (TimeTick.GetSpentTime(MyTools::CTimeTick::em_TimeTick_Second) >= 30 * 1000)
		{
			CONST auto CurPoint = pPersonAttributePtr->GetPoint();

			LOG_CF_E(L"10s֮�ڶ��޷��ƶ�! ��ǰ��ͼ=%s,Tar=[%d,%d], Now=[%d,%d], dis=%.2f", pPersonAttributePtr->GetCurrentMapName().c_str(), TarPoint.X, TarPoint.Y, CurPoint.X, CurPoint.Y, pPersonAttributePtr->GetDis(TarPoint));
			StopGame;
			return FALSE;
		}
		if (!pPersonAttributePtr->IsMoving())
		{
			if (!Action_When_UnMove(wsMapName, TarPoint))
				return FALSE;

			TimeTick.Reset();
			continue;
		}

		// �����ħ��
		MyTools::InvokeClassPtr<CLogicBagItemAction>()->CheckExorcism();
		TimeTick.Reset();
	}
	return TRUE;
}

BOOL CPlayerMove::TransferToCity(_In_ CONST std::wstring& wsCityName) CONST
{
	auto pPersonAttributePtr = MyTools::InvokeClassPtr<CPersonAttribute>();

	CONST static std::vector<std::wstring> VecItemText = { L"�سǷ�",L"���̷�" };
	while (GameRun && pPersonAttributePtr->GetCurrentMapName() != wsCityName)
	{
		BOOL bExist = FALSE;
		for (CONST auto& itmText : VecItemText)
		{
			if (MyTools::InvokeClassPtr<CBagItemExtend>()->GetCount_By_ItemName(wsCityName + itmText) != 0)
			{
				LOG_CF_D(L"ʹ��[%s]�سǷ�", wsCityName.c_str());
				MyTools::InvokeClassPtr<CBagItemAction>()->UseItem(wsCityName + itmText);
				GameSleep(3000);
				bExist = TRUE;
				break;
			}
		}

		if (!bExist)
		{
			LOG_CF_D(L"�����ڻسǷ���Ʒ[%s]", wsCityName.c_str());
			return FALSE;
		}
	}

	return TRUE;
}

std::wstring CPlayerMove::GetRecentlyCityName() CONST
{
	UINT uCount1, uCount2, uCount3 = 0;
	std::wstring wsCurrentMapName = MyTools::InvokeClassPtr<CPersonAttribute>()->GetCurrentMapName();

	CONST static std::vector<std::wstring> VecMapName = { L"�����", L"Ӧ�츮", L"�꾩��"};
	auto pMapName = MyTools::CLPublic::Vec_find_if_Const(VecMapName, [wsCurrentMapName](CONST std::wstring& wsMapName) { return wsCurrentMapName == wsMapName; });
	if (pMapName != nullptr)
		return *pMapName;

	std::vector<std::wstring> VecToCity;
	uCount1 = MyTools::InvokeClassPtr<CMapSearch>()->Search(wsCurrentMapName, L"�����", VecToCity) ? VecToCity.size() : UINT_MAX;
	
	VecToCity.clear();
	uCount2 = MyTools::InvokeClassPtr<CMapSearch>()->Search(wsCurrentMapName, L"Ӧ�츮", VecToCity) ? VecToCity.size() : UINT_MAX;

	VecToCity.clear();
	uCount3 = MyTools::InvokeClassPtr<CMapSearch>()->Search(wsCurrentMapName, L"�꾩��", VecToCity) ? VecToCity.size() : UINT_MAX;

	if (uCount1 <= uCount2 && uCount1 <= uCount3)
		return std::wstring(L"�����");
	else if (uCount2 <= uCount1 && uCount2 <= uCount3)
		return std::wstring(L"Ӧ�츮");;
	return std::wstring(L"�꾩��");
}

BOOL CPlayerMove::Action_When_UnMove(_In_ CONST std::wstring& wsMapName, _In_ CONST Point& TarPoint) CONST
{
	if (MyTools::InvokeClassPtr<CPersonAttribute>()->IsWar())
	{
		LOG_CF_D(L"����ս��!");
		if (!MyTools::InvokeClassPtr<CFarmMonster>()->Fight())
		{
			LOG_CF_D(L"Fight=FALSE, ������·!");
			return FALSE;
		}
	}
	else if (FindDlg_By_BindMethod())
	{
		// 
		GameSleep(1000);
		return TRUE;
	}

	
	Move(wsMapName, TarPoint);
	GameSleep(1000);
	return  TRUE;
}

BOOL CPlayerMove::ShowNpcDlg_When_Move(CONST CGameUi& npcdlg) CONST
{
	BOOL bFind = npcdlg.FindNpcdlgOption_By_PartText_Excute_ActionPtr(L"(Ѱ·)", [](CONST CGameUi::DlgOpionText& dlgText)
	{
		LOG_CF_D(L"��·�����г�����Npc Ѱ· �Ի���");
		BOOL bFind = MyTools::InvokeClassPtr<CNpcExtend>()->FindPlayer_By_Condition_ExcuteAction([](CONST CPlayer& Player)
		{
			return Player.GetDis() == 0 && Player.GetType() == CPlayer::em_PlayerType_Npc;
		}, [dlgText](CONST CNpc& Npc)
		{
			LOG_CF_D(L"���Npcѡ��[%s]->[%s]", dlgText.wsOptionText.c_str(), dlgText.wsClickText.c_str());
			Npc.ClickOption_By_Condition(dlgText.wsClickText, [] { return !MyTools::InvokeClassPtr<CGameUiExtend>()->IsShowNpcDlg(); });
		});

		if (!bFind)
			LOG_MSG_CF(L"��̫���ܡ���������Ѱ·�ĶԻ���, ���Ǿ���Npc�����Զ���������?");
	});

	if (!bFind)
	{
		LOG_CF_D(L"��·�����г�����Npc�Ի���, �ر�!");
		MyTools::InvokeClassPtr<CGameUiExtend>()->CloseNpcDlg();
	}

	return TRUE;
}

BOOL CPlayerMove::FindDlg_By_BindMethod() CONST
{
	struct UiDlgMethod
	{
		std::wstring wsDlgText;
		std::function<BOOL(CONST CGameUi&)> MethodPtr;
	};

	CONST static std::vector<UiDlgMethod> Vec = 
	{
		{ L"npcdlg", std::bind(&CPlayerMove::ShowNpcDlg_When_Move, this, std::placeholders::_1) },
		{ L"Question", std::bind(&CPlayerMove::ShowQuestion_When_UnMove, this, std::placeholders::_1) },
	};

	std::vector<CGameUi> VecGameUi;
	MyTools::InvokeClassPtr<CGameUiExtend>()->GetVecGameUi(VecGameUi);

	for (CONST auto& itm : VecGameUi)
	{
		for (CONST auto& DlgMethodPtr : Vec)
		{
			if (itm.GetName().find(DlgMethodPtr.wsDlgText) != -1)
				return DlgMethodPtr.MethodPtr(itm);
		}
	}

	return FALSE;
}

BOOL CPlayerMove::MoveToHome() CONST
{
	if (MyTools::InvokeClassPtr<CPersonAttribute>()->GetCurrentMapName() != L"Ӧ�츮")
	{
		if (!TransferToCity(L"Ӧ�츮"))
		{
			LOG_CF_E(L"û��Ӧ�츮�Ĵ��ͷ�������ô�ؼ�԰?");
			return FALSE;
		}
	}

	CResText::ResNpcMapPointText ResNpcPoint;
	if (!MyTools::InvokeClassPtr<CResNpcExtend>()->GetResNpc_By_MapName_NpcName(L"Ӧ�츮",L"���ݹ���Ա", ResNpcPoint))
	{
		LOG_CF_E(L"Npc��Դ���澹Ȼ������[%s:%s],��ϵ�Ϸ�!", L"Ӧ�츮", L"���ݹ���Ա");
		return FALSE;
	}
	

	if (!MoveToPoint(ResNpcPoint.FixPoint))
		return FALSE;


	DWORD dwNpcId = 0;
	BOOL bOpenDlg = FALSE;
	BOOL bClickOption = FALSE;
	MyTools::InvokeClassPtr<CNpcExtend>()->FindNpc_By_Name_ExcutePtr(L"���ݹ���Ա", [&bOpenDlg, &bClickOption, &dwNpcId](CONST CNpc& Npc)
	{
		if (Npc.OpenNpcDlg())
		{
			bOpenDlg = TRUE;
			bClickOption = Npc.CLickOption_DisableDlg(L"DoEnterMyHome", L"npcdlg");
		}
	});
	if (!bOpenDlg)
	{
		LOG_CF_E(L"��Npc[���ݹ���Ա] ʧ��!");
		return FALSE;
	}

	if (!bClickOption)
	{
		LOG_CF_E(L"���Npcѡ��[���ݹ���Ա] ʧ��!");
		return FALSE;
	}

	return TRUE;
}

BOOL CPlayerMove::MoveToResNpc(_In_ CONST std::wstring& wsNpcName) CONST
{
	CResText::ResNpcMapPointText ResNpcPoint;
	if (!MyTools::InvokeClassPtr<CResNpcExtend>()->GetResNpc_By_MapName_NpcName(L"Ӧ�츮", L"���ɴ���Ա", ResNpcPoint))
	{
		LOG_MSG_CF(L"Npc��Դ���澹Ȼ������[%s:%s],��ϵ�Ϸ�!", L"Ӧ�츮", L"���ɴ���Ա");
		return FALSE;
	}

	std::wstring wsCurrentMapName = MyTools::InvokeClassPtr<CPersonAttribute>()->GetCurrentMapName();
	if (!MyTools::InvokeClassPtr<CMapSearch>()->Exist(wsCurrentMapName))
	{
		if (TransferToCity(L"Ӧ�츮") || TransferToCity(L"�����") || TransferToCity(L"�꾩��"))
			LOG_CF_D(L"���ڴ��ڷǴ��ͼ�ķ�Χ����������ʹ�ûسǷ�!");
		else
		{
			LOG_MSG_CF(L"��ǰ��ͼ:[%s] ����ʶ��,�������ϲ����ڻسǷ��������ܵ����ͼ�ٿ�ʼ����?");
			StopGame;
			return FALSE;
		}
	}

	if (!TransferToCity(L"Ӧ�츮"))
	{
		std::wstring wsCityName = GetRecentlyCityName();
		LOG_CF_D(L"��ǰ��������ĳ�����:%s", wsCityName.c_str());
		TransferToCity(wsCityName);
	}

	return MoveToMapPoint(L"Ӧ�츮", ResNpcPoint.FixPoint);

}

BOOL CPlayerMove::MoveToSpecialMap(_In_ CONST std::wstring& wsMapName, _In_ CONST Point& TarPoint, _In_ CONST std::wstring wsSpecialMapName) CONST
{
	MyTools::CTimeTick TimeTick;
	CONST auto pPersonAttributePtr = MyTools::InvokeClassPtr<CPersonAttribute>();
	CONST auto pGameUiExtend = MyTools::InvokeClassPtr<CGameUiExtend>();

	while (GameRun && (MyTools::InvokeClassPtr<CPersonAttribute>()->GetCurrentMapName() != wsSpecialMapName))
	{
		GameSleep(500);
		if (TimeTick.GetSpentTime(MyTools::CTimeTick::em_TimeTick_Second) >= 10 * 1000)
		{
			CONST auto CurPoint = pPersonAttributePtr->GetPoint();

			LOG_CF_E(L"10s֮�ڶ��޷��ƶ�! ��ǰ��ͼ=%s,Tar=[%d,%d], Now=[%d,%d], dis=%.2f", pPersonAttributePtr->GetCurrentMapName().c_str(), TarPoint.X, TarPoint.Y, CurPoint.X, CurPoint.Y, pPersonAttributePtr->GetDis(TarPoint));
			StopGame;
			return FALSE;
		}
		if (!pPersonAttributePtr->IsMoving())
		{
			if (!Action_When_UnMove(wsMapName, TarPoint))
				return FALSE;

			TimeTick.Reset();
			continue;
		}

		// �����ħ��
		MyTools::InvokeClassPtr<CLogicBagItemAction>()->CheckExorcism();
		TimeTick.Reset();
	}
	return TRUE;
}

BOOL CPlayerMove::ShowQuestion_When_UnMove(CONST CGameUi& Question) CONST
{
	CHAR Buffer1[50], Buffer2[50], Buffer3[50], Buffer4[50] = { 0 };

	DWORD dwBase = ReadDWORD(Question.GetNodeBase() + C_dati_yiji);
	if (ReadDWORD(dwBase + 0x264) == 0)
		return FALSE;

	dwBase = ReadDWORD(dwBase + C_dati_erji);

	MyTools::CCharacter::strcpy_my(Buffer1, reinterpret_cast<CHAR*>(ReadDWORD(dwBase + 0 * 0x98 + 0x8)));
	MyTools::CCharacter::strcpy_my(Buffer2, reinterpret_cast<CHAR*>(ReadDWORD(dwBase + 1 * 0x98 + 0x8)));
	MyTools::CCharacter::strcpy_my(Buffer3, reinterpret_cast<CHAR*>(ReadDWORD(dwBase + 2 * 0x98 + 0x8)));
	MyTools::CCharacter::strcpy_my(Buffer4, reinterpret_cast<CHAR*>(ReadDWORD(dwBase + 3 * 0x98 + 0x8)));

	DWORD dwAnswer = 0xFF;
	if (strlen(Buffer1) > 0)
	{
		if (strstr(Buffer1, Buffer2) == NULL)
			dwAnswer = strstr(Buffer2, Buffer3) == NULL ? 1 : 0;
		else if (strstr(Buffer1, Buffer3) == NULL)
			dwAnswer = 2;
		else
			dwAnswer = 3;

		if (dwAnswer != 0xFF)
		{
			LOG_CF_D(L"����, ���ǵ�%d��", dwAnswer + 1);
			MyTools::InvokeClassPtr<CExcuteAction>()->PushPtrToMainThread([dwAnswer, Question]
			{
				MyTools::InvokeClassPtr<CGameCALL>()->AnswerQuestion(dwAnswer, Question.GetNodeBase());
			});
			GameSleep(1000);
		}
	}

	return TRUE;
}

BOOL CPlayerMove::MoveToGeneralStore(_Out_ std::wstring& wsNpcName) CONST
{
	std::wstring wsCurrentMapName = MyTools::InvokeClassPtr<CPersonAttribute>()->GetCurrentMapName();
	if (!MyTools::InvokeClassPtr<CMapSearch>()->Exist(wsCurrentMapName))
	{
		if (TransferToCity(L"Ӧ�츮") || TransferToCity(L"�����") || TransferToCity(L"�꾩��"))
		{
			LOG_CF_D(L"���ڴ��ڷǴ��ͼ�ķ�Χ����������ʹ�ûسǷ�!");
			return MoveToGeneralStore(wsNpcName);
		}
		LOG_MSG_CF(L"��ǰ��ͼ:[%s] ����ʶ��,�������ϲ����ڻسǷ��������ܵ����ͼ�ٿ�ʼ����?");
		StopGame;
		return FALSE;
	}

	std::wstring wsCityName = GetRecentlyCityName();
	if (TransferToCity(wsCityName))
	{
		if(wsCityName == L"�����")
			wsNpcName = L"���ǡ��ӻ�̯�ϰ�";
		else if (wsCityName == L"Ӧ�츮")
			wsNpcName = L"��Ӧ���ӻ�̯�ϰ�";
		else
			wsNpcName = L"���꡿�ӻ�̯�ϰ�";
	}
	else
	{
		LOG_CF_D(L"�����ڳ���[%s]�Ĵ��ͷ�, ���Դ���ȥ��ĳ���!");
		if (TransferToCity(L"�����"))
			wsNpcName = L"���ǡ��ӻ�̯�ϰ�";
		else if (TransferToCity(L"Ӧ�츮"))
			wsNpcName = L"��Ӧ���ӻ�̯�ϰ�";
		else if (TransferToCity(L"�꾩��"))
			wsNpcName = L"���꡿�ӻ�̯�ϰ�";
		else
		{
			LOG_CF_D(L"�������κεĳ��д��ͷ�!");
			return FALSE;
		}
	}
	

	return MoveToGeneralStoreNpc(wsCityName, wsNpcName);
}

VOID CPlayerMove::Move(_In_ CONST std::wstring& wsMapName, _In_ CONST Point& TarPoint) CONST
{
	CONST auto CurPoint = MyTools::InvokeClassPtr<CPersonAttribute>()->GetPoint();
	
	if (MyTools::InvokeClassPtr<CPersonAttribute>()->GetCurrentMapName() == wsMapName)
	{
		LOG_CF_D(L"MoveToPoint, Tar=[%d,%d], Now=[%d,%d], dis=%.2f", TarPoint.X, TarPoint.Y, CurPoint.X, CurPoint.Y, MyTools::InvokeClassPtr<CPersonAttribute>()->GetDis(TarPoint));
		MyTools::InvokeClassPtr<CExcuteAction>()->PushPtrToMainThread([TarPoint]
		{
			MyTools::InvokeClassPtr<CGameCALL>()->MoveToPoint(TarPoint);
		});
	}
	else
	{
		LOG_CF_D(L"MoveToPoint, Tar=[%s,%d,%d], Now=[%d,%d], dis=%.2f", wsMapName.c_str(), TarPoint.X, TarPoint.Y, CurPoint.X, CurPoint.Y, MyTools::InvokeClassPtr<CPersonAttribute>()->GetDis(TarPoint));
		MyTools::InvokeClassPtr<CExcuteAction>()->PushPtrToMainThread([wsMapName, TarPoint]
		{
			MyTools::InvokeClassPtr<CGameCALL>()->MoveToPoint(MyTools::CCharacter::UnicodeToASCII(wsMapName).c_str(), TarPoint);
		});
	}
}

BOOL CPlayerMove::MoveToGeneralStoreNpc(_In_ CONST std::wstring& wsMapName, _In_ CONST std::wstring& wsNpcName) CONST
{
	CResText::ResNpcMapPointText ResNpc;
	if (!MyTools::InvokeClassPtr<CResNpcExtend>()->GetResNpc_By_MapName_NpcName(wsMapName, wsNpcName, ResNpc))
	{
		StopGame;
		LOG_MSG_CF(L"��ǰ��Դ�ļ���Ȼ������Npc[%s]", wsNpcName.c_str());
		return FALSE;
	}
	
	LOG_CF_D(L"�ߵ��ӻ�̯Npc[%s]�Ա�", wsNpcName.c_str());
	if (!MoveToPoint(ResNpc.FixPoint))
	{
		LOG_CF_E(L"�ߵ�%sʧ��!", wsNpcName.c_str());
		return FALSE;
	}

	return TRUE;
}

