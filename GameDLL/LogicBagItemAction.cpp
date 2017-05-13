#include "stdafx.h"
#include "LogicBagItemAction.h"
#include <MyTools/Log.h>
#include "PersonAttribute.h"
#include "GameVariable.h"
#include "BagItemAction.h"
#include "PersonPetAction.h"
#include "PersonAction.h"

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
	BOOL bExist = FALSE;
	MyTools::InvokeClassPtr<CGameVariable>()->Action_For_EqualValue_By_Id(em_TextVar::em_TextVar_UseExorcism, 1, [&bExist]
	{
		if (!MyTools::InvokeClassPtr<CPersonAttribute>()->ExistPersonBuff_By_PartName(L"��ħ��ʣ��ʱ��"))
			bExist = MyTools::InvokeClassPtr<CBagItemAction>()->UseItem(L"��ħ��");
		else
			bExist = TRUE;
	});

	return bExist;
}

