#include "stdafx.h"
#include "ScriptServices.h"
#include <MyTools/Log.h>
#include "FarmField.h"
#include "CollectItem.h"
#include "HotFireTask.h"
#include "PurifyWaterTask.h"
#include "BangTask.h"
#include "PersonAction.h"
#include "Examination.h"
#include "BagItemExtend.h"
#include "LogicBagItemAction.h"
#include "PersonAttribute.h"

#define _SELF L"ScriptServices.cpp"
BOOL CScriptServices::Run()
{
	StartGame;
	CommonAction();

	auto& wsText = MyTools::InvokeClassPtr<CTextVariable>()->GetRefValue_By_Id(em_TextVar::em_TextVar_ConfigAction);
	if (wsText == L"Ұ��")
		return RunPtr<CFarmField>();
	else if (wsText == L"�ɼ�")
		return RunPtr<CCollectItem>();
	else if (wsText == L"����")
		return RunPtr<CHotFireTask>();
	else if (wsText == L"��ˮ")
		return RunPtr<CPurifyWaterTask>();
	else if (wsText == L"ʤľ")
		return RunPtr<CBangTask>();
	else if (wsText == L"����")
		return RunPtr<CExamination>();

	LOG_MSG_CF(L"����:[%s] ��û������", wsText.c_str());
	return FALSE;
}

BOOL CScriptServices::CommonCheck()
{
	if (MyTools::InvokeClassPtr<CGameVariable>()->GetRefValue_By_Id(em_TextVar::em_TextVar_AutoBuyExorcism) && MyTools::InvokeClassPtr<CBagItemExtend>()->GetCount_By_ItemName(L"��ħ��") == 0)
	{
		LOG_CF_D(L"�������Զ�������ħ��, �������ϲ���������ħ��! ȥ����ħ��");
		if (!MyTools::InvokeClassPtr<CLogicBagItemAction>()->SupplementItem(L"��ħ��", 10))
			return FALSE;
	}

	if (MyTools::InvokeClassPtr<CGameVariable>()->GetRefValue_By_Id(em_TextVar::em_TextVar_UseExorcism))
	{
		MyTools::InvokeClassPtr<CLogicBagItemAction>()->CheckExorcism();
	}

	if(MyTools::InvokeClassPtr<CGameVariable>()->GetRefValue_By_Id(em_TextVar::em_TextVar_UseExorcism) == 0 && MyTools::InvokeClassPtr<CPersonAttribute>()->ExistPersonBuff_By_PartName(L"��ħ��"))
	{
		LOG_CF_D(L"���ò�ʹ����ħ��, ����ȴ��⵽��������ħ��BUFF, ������ħ������!");
		MyTools::InvokeClassPtr<CLogicBagItemAction>()->RemoveExorcism();
	};

	if (MyTools::InvokeClassPtr<CGameVariable>()->GetRefValue_By_Id(em_TextVar::em_TextVar_AutoBuyReturnSymbol) && MyTools::InvokeClassPtr<CBagItemExtend>()->GetCount_By_ItemPartName(L"���̷�") == 0)
	{
		LOG_CF_D(L"�������Զ����򳬳̷�, ���ϲ���, ȥ���򳬳̷�!");
		if (!MyTools::InvokeClassPtr<CLogicBagItemAction>()->SupplementItem(L"Ӧ�츮���̷�", 1))
		{
			LOG_CF_E(L"�Զ����򳬳̷�ʧ�ܡ���");
			return FALSE;
		}
	}

	if(MyTools::InvokeClassPtr<CGameVariable>()->GetRefValue_By_Id(em_TextVar::em_TextVar_AutoBuyHappyBell) && MyTools::InvokeClassPtr<CBagItemExtend>()->GetCount_By_ItemName(L"������") == 0)
	{
		LOG_CF_D(L"���ϲ������ڻ�����! ȥ������");
		if (!MyTools::InvokeClassPtr<CLogicBagItemAction>()->SupplementItem(L"������", 10))
		{
			LOG_CF_E(L"�Զ���������ʧ�ܡ���");
			return FALSE;
		}
	};

	return TRUE;
}

BOOL CScriptServices::CommonAction() CONST
{
	if (MyTools::InvokeClassPtr<CGameVariable>()->GetRefValue_By_Id(em_TextVar::em_TextVar_ShieldPlayer))
	{
		LOG_CF_D(L"�����������!");
		MyTools::InvokeClassPtr<CPersonAction>()->DisableOtherPlayer();
	}

	return TRUE;
}
