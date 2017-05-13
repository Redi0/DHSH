#include "stdafx.h"
#include "PersonPetAction.h"
#include <MyTools/Log.h>
#include "PersonPetExtend.h"
#include "PersonPet.h"
#include "BagItemAction.h"
#include "ExcuteAction.h"
#include "GameCALL.h"
#include "GameVariable.h"

#define _SELF L"PersonPetAction.cpp"
BOOL CPersonPetAction::SupplementLoyalty() CONST
{
	BOOL bExist = FALSE;
	MyTools::InvokeClassPtr<CPersonPetExtend>()->Action_By_JoinWarPet([&bExist] (CONST CPersonPet& Pet)
	{
		if (Pet.GetLoyalty() < 90)
		{
			bExist = MyTools::InvokeClassPtr<CBagItemAction>()->UseItem(L"������");
		}
	});

	return bExist;
}

BOOL CPersonPetAction::SupplementHp() CONST
{
	BOOL bRetCode = FALSE;
	MyTools::InvokeClassPtr<CPersonPetExtend>()->Action_By_JoinWarPet([&bRetCode](CONST CPersonPet& Pet)
	{
		CONST auto dwValue = MyTools::InvokeClassPtr<CGameVariable>()->GetRefValue_By_Id(em_TextVar::em_TextVar_Pet_SupplementHp);
		if (Pet.GetPercentHp() < dwValue)
		{
			LOG_CF_D(L"�����HP=%d, �����趨��ֵ=%d, ��ҩ!", Pet.GetPercentHp(), dwValue);
			MyTools::InvokeClassPtr<CExcuteAction>()->PushPtrToMainThread([] {MyTools::InvokeClassPtr<CGameCALL>()->AddHp(0x3); });
		}

		bRetCode = Pet.GetPercentHp() >= dwValue;
	});

	return bRetCode;
}

BOOL CPersonPetAction::SupplementMp() CONST
{
	BOOL bRetCode = FALSE;
	MyTools::InvokeClassPtr<CPersonPetExtend>()->Action_By_JoinWarPet([&bRetCode](CONST CPersonPet& Pet)
	{
		CONST auto dwValue = MyTools::InvokeClassPtr<CGameVariable>()->GetRefValue_By_Id(em_TextVar::em_TextVar_Pet_SupplementMp);
		if (Pet.GetPercentMp() < dwValue)
		{
			LOG_CF_D(L"�����MP=%d, �����趨��ֵ=%d, ��ҩ!", Pet.GetPercentMp(), dwValue);
			MyTools::InvokeClassPtr<CExcuteAction>()->PushPtrToMainThread([] {MyTools::InvokeClassPtr<CGameCALL>()->AddHp(0x4); });
		}

		bRetCode = Pet.GetPercentMp() >= dwValue;
	});

	return bRetCode;
}
