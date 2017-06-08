#include "stdafx.h"
#include "ScriptServices.h"
#include <MyTools/Log.h>
#include "FarmField.h"
#include "CollectItem.h"
#include "HotFireTask.h"
#include "PurifyWaterTask.h"
#include "BangTask.h"

#define _SELF L"ScriptServices.cpp"
BOOL CScriptServices::Run()
{
	StartGame;
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

	LOG_MSG_CF(L"����:[%s] ��û������", wsText.c_str());
	return FALSE;
}
