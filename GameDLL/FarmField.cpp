#include "stdafx.h"
#include "FarmField.h"
#include <MyTools/Log.h>
#include <MyTools/Character.h>
#include "PersonAttribute.h"
#include "PlayerMove.h"
#include "FarmMonster.h"
#include "ExcuteAction.h"
#include "GameCALL.h"
#include "GameVariable.h"
#include "BagItemAction.h"
#include "BagItemExtend.h"
#include "LogicBagItemAction.h"
#include "PersonPetAction.h"
#include "TextVariable.h"

#define _SELF L"FarmField.cpp"
BOOL CFarmField::Run() CONST
{
	FieldConfig Config_;
	if (!GetFieldConfig(Config_) || !Check())
		return FALSE;

	LOG_CF_D(L"Ұ��ˢ�֡���Ŀ�ĵ���[%s,%d,%d]", Config_.wsMapName.c_str(), Config_.MapPoint.X, Config_.MapPoint.Y);
	CONST auto pPersonAttributePtr = MyTools::InvokeClassPtr<CPersonAttribute>();
	while (GameRun)
	{
		GameSleep(500);
		if (pPersonAttributePtr->GetCurrentMapName() != Config_.wsMapName)
		{
			LOG_CF_D(L"��ǰ��ͼ=[%s], !=Ŀ�ĵ�[%s], ����Ѱ·!", pPersonAttributePtr->GetCurrentMapName().c_str(), Config_.wsMapName.c_str());
			if (!MyTools::InvokeClassPtr<CPlayerMove>()->MoveToMapPoint(Config_.wsMapName, Config_.MapPoint))
			{
				LOG_CF_D(L"MoveToMapPoint Faild!");
				break;
			}

			continue;
		}

		if (!pPersonAttributePtr->IsMoving())
		{
			LOG_CF_D(L"ֹͣ��·�ˡ���");
			if (pPersonAttributePtr->IsWar())
			{
				LOG_CF_D(L"��������!��ʼս��!");
				if (!MyTools::InvokeClassPtr<CFarmMonster>()->Fight())
				{
					LOG_CF_D(L"Fight Faild!");
					break;
				}

				GameSleep(2 * 1000);
				continue;
			}

			// Check Answer!
			MyTools::InvokeClassPtr<CPlayerMove>()->FindDlg_By_BindMethod();

			// ALT+Z
			LOG_CF_D(L"����ֹͣ������,��ALT+Z��ʼ�Զ���·!");
			MyTools::InvokeClassPtr<CExcuteAction>()->PushPtrToMainThread([] { MyTools::InvokeClassPtr<CGameCALL>()->SetAutoMove(TRUE); });
			continue;
		}
	}

	MyTools::InvokeClassPtr<CPlayerMove>()->MoveToPoint(pPersonAttributePtr->GetPoint());
	return TRUE;
}

BOOL CFarmField::Check() CONST
{
	BOOL bFaild = FALSE;
	MyTools::InvokeClassPtr<CGameVariable>()->Action_For_EqualValue_By_Id(em_TextVar::em_TextVar_AutoBuyExorcism, 1, [this, &bFaild]
	{
		if (MyTools::InvokeClassPtr<CBagItemExtend>()->GetCount_By_ItemName(L"��ħ��") == 0)
		{
			LOG_CF_D(L"�������Զ�������ħ��, �������ϲ���������ħ��! ȥ����ħ��");
			bFaild = !MyTools::InvokeClassPtr<CLogicBagItemAction>()->SupplementItem(L"��ħ��", 10);
		}
	});
	if (bFaild)
		return FALSE;

	MyTools::InvokeClassPtr<CGameVariable>()->Action_For_EqualValue_By_Id(em_TextVar::em_TextVar_AutoBuyHappyBell, 1, [this, &bFaild]
	{
		if (MyTools::InvokeClassPtr<CBagItemExtend>()->GetCount_By_ItemName(L"������") == 0)
		{
			LOG_CF_D(L"���ϲ������ڻ�����! ȥ������");
			bFaild = !MyTools::InvokeClassPtr<CLogicBagItemAction>()->SupplementItem(L"������", 10);
		}
	});
	if (bFaild)
		return FALSE;

	MyTools::InvokeClassPtr<CGameVariable>()->Action_For_EqualValue_By_Id(em_TextVar::em_TextVar_UseExorcism, 0, [this]
	{
		if (MyTools::InvokeClassPtr<CPersonAttribute>()->ExistPersonBuff_By_PartName(L"��ħ��"))
		{
			LOG_CF_D(L"���ò�ʹ����ħ��, ����ȴ��⵽��������ħ��BUFF, ������ħ������!");
			MyTools::InvokeClassPtr<CLogicBagItemAction>()->RemoveExorcism();
		}
	});


	return MyTools::InvokeClassPtr<CLogicBagItemAction>()->AfterFight_Item();
}

BOOL CFarmField::GetFieldConfig(_Out_ FieldConfig& FieldConfig_) CONST
{
	FieldConfig_.wsMapName = MyTools::InvokeClassPtr<CTextVariable>()->GetRefValue_By_Id(em_TextVar::em_TextVar_Field_MapName);
	
	std::vector<std::wstring> VecPoint;
	if (MyTools::CCharacter::Split(MyTools::InvokeClassPtr<CTextVariable>()->GetRefValue_By_Id(em_TextVar::em_TextVar_Field_MapPoint), L",", VecPoint, Split_Option_RemoveEmptyEntries) != 2)
	{
		LOG_MSG_CF(L"�����ʽ����!");
		return FALSE;
	}

	FieldConfig_.MapPoint.X = std::stoi(VecPoint.at(0));
	FieldConfig_.MapPoint.Y = std::stoi(VecPoint.at(1));
	return TRUE;
}

