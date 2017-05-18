#include "stdafx.h"
#include "PersonAction.h"
#include <MyTools/Log.h>
#include "ExcuteAction.h"
#include "GameCALL.h"
#include "PersonAttribute.h"
#include "GameVariable.h"

#define _SELF L"PersonAction.cpp"
VOID CPersonAction::SupplementHp() CONST
{
	CONST auto pPersonAttributePtr = MyTools::InvokeClassPtr<CPersonAttribute>();
	CONST auto dwValue = MyTools::InvokeClassPtr<CGameVariable>()->GetRefValue_By_Id(em_TextVar::em_TextVar_Person_SupplementHp);

	if (pPersonAttributePtr->GetPercentHp() < dwValue)
	{
		LOG_CF_D(L"�����HP=%d, �����趨��ֵ=%d, ��ҩ!", pPersonAttributePtr->GetPercentHp(), dwValue);
		MyTools::InvokeClassPtr<CExcuteAction>()->PushPtrToMainThread([] {MyTools::InvokeClassPtr<CGameCALL>()->AddHp(0x1); });
	}

}

VOID CPersonAction::SupplementMp() CONST
{
	CONST auto pPersonAttributePtr = MyTools::InvokeClassPtr<CPersonAttribute>();
	CONST auto dwValue = MyTools::InvokeClassPtr<CGameVariable>()->GetRefValue_By_Id(em_TextVar::em_TextVar_Person_SupplementMp);

	if (pPersonAttributePtr->GetPercentMp() < dwValue)
	{
		LOG_CF_D(L"�����MP=%d, �����趨��ֵ=%d, ��ҩ!", pPersonAttributePtr->GetPercentMp(), dwValue);
		MyTools::InvokeClassPtr<CExcuteAction>()->PushPtrToMainThread([] {MyTools::InvokeClassPtr<CGameCALL>()->AddHp(0x2); });
	}
}
