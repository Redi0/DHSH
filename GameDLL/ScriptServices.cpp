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
