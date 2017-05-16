#include "stdafx.h"
#include "LogicBagItemAction.h"
#include <MyTools/Log.h>
#include <MyTools/CLPublic.h>
#include "PersonAttribute.h"
#include "GameVariable.h"
#include "BagItemAction.h"
#include "PersonPetAction.h"
#include "PersonAction.h"
#include "BagItemExtend.h"
#include "PlayerMove.h"
#include "Npc.h"
#include "NpcExtend.h"
#include "ResText.h"
#include "ExcuteAction.h"
#include "GameCALL.h"
#include "GameUiExtend.h"
#include "PersonPetExtend.h"

#define _SELF L"LogicBagItemAction.cpp"
BOOL CLogicBagItemAction::AfterFight_Item() CONST
{
	CONST auto pPersonAttributePtr = MyTools::InvokeClassPtr<CPersonAttribute>();
	if (pPersonAttributePtr->IsWar())
		return TRUE;

	// �����ħ��
	if (!CheckExorcism())
	{
		LOG_CF_D(L"��ħ�㲻��!");
		return FALSE;
	}

	// ����Լ���HP MP
	CONST auto pPersonActionPtr = MyTools::InvokeClassPtr<CPersonAction>();
	if (!pPersonActionPtr->SupplementHp())
	{
		LOG_CF_D(L"HPҩ����!");
		return FALSE;
	}
	else if (!pPersonActionPtr->SupplementMp())
	{
		LOG_CF_D(L"MPҩ����!");
		return FALSE;
	}

	// �������HP MP
	CONST auto pPersonPetAction = MyTools::InvokeClassPtr<CPersonPetAction>();
	if (!pPersonPetAction->SupplementHp())
	{
		LOG_CF_D(L"HPҩ����!");
		return FALSE;
	}

	if (!pPersonPetAction->SupplementMp())
	{
		LOG_CF_D(L"MPҩ����!");
		return FALSE;
	}

	// ��������ҳ϶�
	if (!pPersonPetAction->SupplementLoyalty())
	{
		LOG_CF_D(L"�����岻��!");
		return FALSE;
	}

	return TRUE;
}

BOOL CLogicBagItemAction::CheckExorcism() CONST
{
	BOOL bExist = TRUE;
	MyTools::InvokeClassPtr<CGameVariable>()->Action_For_EqualValue_By_Id(em_TextVar::em_TextVar_UseExorcism, 1, [&bExist]
	{
		if (!MyTools::InvokeClassPtr<CPersonAttribute>()->ExistPersonBuff_By_PartName(L"��ħ��"))
			bExist = MyTools::InvokeClassPtr<CBagItemAction>()->UseItem(L"��ħ��");
	});

	return bExist;
}

BOOL CLogicBagItemAction::Check() CONST
{
	BOOL bFaild = FALSE;
	MyTools::InvokeClassPtr<CGameVariable>()->Action_For_EqualValue_By_Id(em_TextVar::em_TextVar_AutoBuyExorcism, 1, [this, &bFaild]
	{
		if (MyTools::InvokeClassPtr<CBagItemExtend>()->GetCount_By_ItemName(L"��ħ��") == 0)
		{
			LOG_CF_D(L"�������Զ�������ħ��, �������ϲ���������ħ��! ȥ����ħ��");
			bFaild = !SupplementItem(L"��ħ��", 10);
		}
	});
	if (bFaild)
		return FALSE;

	MyTools::InvokeClassPtr<CGameVariable>()->Action_For_EqualValue_By_Id(em_TextVar::em_TextVar_AutoBuyHappyBell, 1, [this, &bFaild]
	{
		if (MyTools::InvokeClassPtr<CBagItemExtend>()->GetCount_By_ItemName(L"������") == 0)
		{
			LOG_CF_D(L"���ϲ������ڻ�����! ȥ������");
			bFaild = !SupplementItem(L"������", 10);
		}
	});
	if (bFaild)
		return FALSE;

	MyTools::InvokeClassPtr<CGameVariable>()->Action_For_EqualValue_By_Id(em_TextVar::em_TextVar_UseExorcism, 0, [this]
	{
		if (MyTools::InvokeClassPtr<CPersonAttribute>()->ExistPersonBuff_By_PartName(L"��ħ��"))
		{
			LOG_CF_D(L"���ò�ʹ����ħ��, ����ȴ��⵽��������ħ��BUFF, ������ħ������!");
			RemoveExorcism();
		}
	});

	// Set Enter War Pet!
	MyTools::InvokeClassPtr<CPersonPetAction>()->SetPetEnterWar();



	return AfterFight_Item();
}

BOOL CLogicBagItemAction::SupplementItem(_In_ CONST std::wstring& wsItemName, _In_ UINT uCount) CONST
{
	// �����Ƿ��пո�
	if (MyTools::InvokeClassPtr<CBagItemExtend>()->IsBagFull())
	{
		LOG_CF_E(L"��������!");
		return FALSE;
	}

	std::wstring wsNpcName;
	if (!MyTools::InvokeClassPtr<CPlayerMove>()->MoveToGeneralStore(wsNpcName))
		return FALSE;

	DWORD dwNpcId = 0;
	BOOL bOpenDlg = FALSE;
	BOOL bClickOption = FALSE;
	MyTools::InvokeClassPtr<CNpcExtend>()->FindNpc_By_Name_ExcutePtr(wsNpcName, [&bOpenDlg, &bClickOption, &dwNpcId](CONST CNpc& Npc)
	{ 
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
		LOG_CF_E(L"���Npcѡ��[%s] ʧ��!", wsNpcName.c_str());
		return FALSE;
	}

	auto pVecResItem = MyTools::InvokeClassPtr<CResText>()->GetStructPtr<CONST std::vector<CResText::ResStoreItemText>*>(L"ResShopItemText");
	if (pVecResItem == nullptr)
		return FALSE;

	auto pResItem = MyTools::CLPublic::Vec_find_if_Const(*pVecResItem, [wsItemName, wsNpcName](CONST CResText::ResStoreItemText& ResItem) { return ResItem.wsItemName == wsItemName && ResItem.wsNpcName == wsNpcName; });
	if (pResItem == nullptr)
	{
		LOG_CF_E(L"��������Դ[%s],[%s]", wsNpcName.c_str(), wsItemName.c_str());
		return FALSE;
	}

	// ������Ǯ!
	if (MyTools::InvokeClassPtr<CPersonAttribute>()->GetMoney() < pResItem->dwSinglePrice * uCount)
	{
		LOG_CF_E(L"����Ǯ������Ʒ����[%d],����[%d],�ܼ�[%d],����ֻ��[%d]", pResItem->dwSinglePrice, uCount, pResItem->dwSinglePrice * uCount, MyTools::InvokeClassPtr<CPersonAttribute>()->GetMoney());
		return FALSE;
	}

	LOG_CF_D(L"������Ʒ:[%s], ����:[%d]", pResItem->wsItemName.c_str(), uCount);
	MyTools::InvokeClassPtr<CExcuteAction>()->PushPtrToMainThread([dwNpcId, pResItem, uCount]
	{
		MyTools::InvokeClassPtr<CGameCALL>()->Buy_By_Npc(dwNpcId, pResItem->dwStoreIndex, uCount);
	});

	GameSleep(1000);

	// Close shop dlg
	if (!MyTools::InvokeClassPtr<CGameUiExtend>()->CloseDlg(L"shop_npcbuy"))
		return FALSE;

	if (MyTools::InvokeClassPtr<CBagItemExtend>()->GetCount_By_ItemName(wsItemName) == 0)
	{
		LOG_CF_E(L"���粨�������ӳٵ�����[%s]ʧ��!", wsItemName.c_str());
		return FALSE;
	}
	return TRUE;
}

VOID CLogicBagItemAction::RemoveExorcism() CONST
{
	MyTools::InvokeClassPtr<CExcuteAction>()->PushPtrToMainThread([] { MyTools::InvokeClassPtr<CGameCALL>()->AbandonTask(0x7E6); });
}

