#include "stdafx.h"
#include "GameConfig.h"
#include "GameVariable.h"
#include <MyTools/Character.h>
#include <MyTools/CLPublic.h>
#include <MyTools/Log.h>
#include "FarmField.h"
#include "TextVariable.h"

#define _SELF L"GameConfig.cpp"
BOOL CGameConfig::ExistConfigFile() CONST
{
	return MyTools::CLPublic::FileExist(GetConfigPath().c_str());
}

BOOL CGameConfig::ReadConfig(_In_ em_Config_Type emConfigType, _In_ CONST std::wstring& wsConfigName, _Out_ std::wstring& wsConfigValue) CONST
{
	auto& pShareContent = MyTools::InvokeClassPtr<CGameVariable>()->GetGameShareContent();
	if (pShareContent == nullptr)
	{
		wsConfigValue = L"Error";
		return FALSE;
	}

	std::wstring wsAppText;
	if (!GetConfigAppText(emConfigType, wsAppText))
	{
		LOG_MSG_CF(L"�����ڸ÷���");
		return FALSE;
	}

	
	WCHAR wszConfigText[MAX_PATH] = { 0 };
	BOOL bRetCode = ::GetPrivateProfileStringW(wsAppText.c_str(), wsConfigName.c_str(), L"Empty", wszConfigText, MAX_PATH, GetConfigPath().c_str());
	wsConfigValue = wszConfigText;
	return wsConfigValue != L"Empty" && bRetCode;
}

BOOL CGameConfig::GetConfigAppText(_In_ em_Config_Type emConfigType, _Out_ std::wstring& wsText) CONST
{
	struct ConfigTypeText
	{
		em_Config_Type ConfigType;
		std::wstring   wsConfigTypeText;
	};

	CONST static std::vector<ConfigTypeText> Vec =
	{
		{ em_Config_Type::em_Config_Type_Action , L"����"},
		{ em_Config_Type::em_Config_Type_Field ,L"Ұ��" },
		{ em_Config_Type::em_Config_Type_Collect ,L"�ɼ�" },
		{ em_Config_Type::em_Config_Type_HotFire ,L"����" },
		{ em_Config_Type::em_Config_Type_PurifyWater ,L"��ˮ" },
		{ em_Config_Type::em_Config_Type_BangTask ,L"ʤľ" },
		{ em_Config_Type::em_Config_Type_Exam ,L"����" },
		{ em_Config_Type::em_Config_Type_Fight ,L"ս��" },
		{ em_Config_Type::em_Config_Type_Common ,L"����" },
	};

	auto p = MyTools::CLPublic::Vec_find_if_Const(Vec, [emConfigType](CONST ConfigTypeText& itm) { return itm.ConfigType == emConfigType; });
	if (p == nullptr)
		return FALSE;

	wsText = p->wsConfigTypeText;
	return TRUE;
}

std::wstring CGameConfig::GetConfigPath() CONST
{
	if (MyTools::InvokeClassPtr<CGameVariable>()->GetRefValue_By_Id(em_TextVar::em_TextVar_IsRunDlg))
	{
		std::wstring wsPath = L"C:\\";
		wsPath += MyTools::InvokeClassPtr<CGameVariable>()->GetAccountShareContent()->AccountContent.wszPlayerName;
		wsPath += L".ini";
		return std::move(wsPath);
	}

	std::wstring wsPath = MyTools::InvokeClassPtr<CGameVariable>()->GetGameShareContent()->wszConsolePath;
	wsPath += L"\\";
	wsPath += MyTools::InvokeClassPtr<CGameVariable>()->GetAccountShareContent()->AccountContent.wszPlayerName;
	wsPath += L".ini";

	return std::move(wsPath);
}

BOOL CGameConfig::Initialize() CONST
{
	std::wstring wsValue;
	if (!ReadConfig(em_Config_Type::em_Config_Type_Action, L"����", wsValue))
	{
		LOG_MSG_CF(L"������������:[����]");
		return FALSE;
	}

	if (SetCommonConfig())
		return FALSE;

	struct BindConfigMethod
	{
		std::wstring wsConfigValue;
		std::function<BOOL(VOID)> MethodPtr;
	};

	CONST static std::vector<BindConfigMethod> Vec = 
	{
		{ L"Ұ��", std::bind(&CGameConfig::ReadConfig_Field, this) },
		{ L"�ɼ�", std::bind(&CGameConfig::ReadConfig_Collect, this) },
		{ L"����", std::bind(&CGameConfig::ReadConfig_HotFire, this) },
		{ L"��ˮ", std::bind(&CGameConfig::ReadConfig_PurifyWater, this) },
		{ L"ʤľ", std::bind(&CGameConfig::ReadConfig_BangTask, this) },
	};

	auto p = MyTools::CLPublic::Vec_find_if_Const(Vec, [wsValue](CONST BindConfigMethod& itm) { return itm.wsConfigValue == wsValue; });
	if (p == nullptr)
	{
		LOG_MSG_CF(L"����������ֵ:����=%s", wsValue.c_str());
		return FALSE;
	}

	MyTools::InvokeClassPtr<CTextVariable>()->SetValueAndGetOldValue_By_Id(em_TextVar::em_TextVar_ConfigAction, wsValue);
	LOG_CF_D(L"Action=%s", wsValue.c_str());
	return p->MethodPtr();
}

BOOL CGameConfig::SetConfigValue(_In_ em_Config_Type emConfigType, _In_ CONST std::wstring& wsConfigName, _In_ em_TextVar emTextVar, _In_ std::function<BOOL(em_TextVar, CONST std::wstring &)> SetValuePtr) CONST
{
	std::wstring wsValue;
	if (!ReadConfig(emConfigType, wsConfigName, wsValue))
	{
		LOG_MSG_CF(L"������������:[%s]", wsConfigName.c_str());
		return FALSE;
	}

	return SetValuePtr(emTextVar, wsValue);
}

BOOL CGameConfig::ReadConfig_Field() CONST
{
	auto fnSetTextPtr = [](em_TextVar emTextVar, CONST std::wstring& wsValue)
	{
		MyTools::InvokeClassPtr<CTextVariable>()->SetValueAndGetOldValue_By_Id(emTextVar, wsValue);
		return TRUE;
	};

	if (!SetConfigValue(em_Config_Type::em_Config_Type_Field, L"��ͼ", em_TextVar::em_TextVar_Field_MapName, fnSetTextPtr))
		return FALSE;

	if (!SetConfigValue(em_Config_Type::em_Config_Type_Field, L"����", em_TextVar::em_TextVar_Field_MapPoint, fnSetTextPtr))
		return FALSE;
	
	//
	MyTools::InvokeClassPtr<CGameVariable>()->SetValueAndGetOldValue_By_Id(em_TextVar::em_TextVar_UseExorcism, 0);

	return TRUE;
}

BOOL CGameConfig::ReadConfig_Collect() CONST
{
	auto fnSetValuePtr = [](em_TextVar emTextVar, CONST std::wstring& wsValue)
	{
		MyTools::InvokeClassPtr<CGameVariable>()->SetValueAndGetOldValue_By_Id(emTextVar, std::stoi(wsValue));
		return TRUE;
	};

	auto fnSetTextPtr = [](em_TextVar emTextVar, CONST std::wstring& wsValue)
	{
		MyTools::InvokeClassPtr<CTextVariable>()->SetValueAndGetOldValue_By_Id(emTextVar, wsValue);
		return TRUE;
	};

	if (!SetConfigValue(em_Config_Type::em_Config_Type_Collect, L"�Զ�������ħ��", em_TextVar::em_TextVar_AutoBuyExorcism, fnSetValuePtr))
		return FALSE;

	if (!SetConfigValue(em_Config_Type::em_Config_Type_Collect, L"ʹ����ħ��", em_TextVar::em_TextVar_UseExorcism, fnSetValuePtr))
		return FALSE;

	if (!SetConfigValue(em_Config_Type::em_Config_Type_Collect, L"�Զ������������", em_TextVar::em_TextVar_AutoBuyPetDrinksCount, fnSetValuePtr))
		return FALSE;

	if (!SetConfigValue(em_Config_Type::em_Config_Type_Collect, L"�Զ����������������", em_TextVar::em_TextVar_AutoBuyPetDrinksCount, fnSetValuePtr))
		return FALSE;

	if (!SetConfigValue(em_Config_Type::em_Config_Type_Collect, L"�ɼ���ͼ", em_TextVar::em_TextVar_Collect_MapName, fnSetTextPtr))
		return FALSE;

	if (!SetConfigValue(em_Config_Type::em_Config_Type_Collect, L"�ɼ���", em_TextVar::em_TextVar_Collect_ItemName, fnSetTextPtr))
		return FALSE;

	if (!SetConfigValue(em_Config_Type::em_Config_Type_Collect, L"��԰�Ҿ�", em_TextVar::em_TextVar_CollectHome_Furniture, fnSetTextPtr))
		return FALSE;

	return TRUE;
}

BOOL CGameConfig::ReadConfig_BangTask() CONST
{
	auto fnSetValuePtr = [](em_TextVar emTextVar, CONST std::wstring& wsValue)
	{
		MyTools::InvokeClassPtr<CGameVariable>()->SetValueAndGetOldValue_By_Id(emTextVar, std::stoi(wsValue));
		return TRUE;
	};

	if (!SetConfigValue(em_Config_Type::em_Config_Type_BangTask, L"�Զ�������ħ��", em_TextVar::em_TextVar_AutoBuyExorcism, fnSetValuePtr))
		return FALSE;

	if (!SetConfigValue(em_Config_Type::em_Config_Type_BangTask, L"��������Ӧ�츮���̷�", em_TextVar::em_TextVar_AutoBuyReturnSymbol, fnSetValuePtr))
		return FALSE;

	return TRUE;
}

BOOL CGameConfig::ReadConfig_PurifyWater() CONST
{
	auto fnSetValuePtr = [](em_TextVar emTextVar, CONST std::wstring& wsValue)
	{
		if(wsValue == L"����")
			MyTools::InvokeClassPtr<CGameVariable>()->SetValueAndGetOldValue_By_Id(emTextVar, 0);
		else if (wsValue == L"�ﹱ")
			MyTools::InvokeClassPtr<CGameVariable>()->SetValueAndGetOldValue_By_Id(emTextVar, 1);
		else
		{
			LOG_MSG_CF(L"���������:[%s]", wsValue.c_str());
			return FALSE;
		}
		return TRUE;
	};

	if (!SetConfigValue(em_Config_Type::em_Config_Type_PurifyWater, L"���������", em_TextVar::em_TextVar_PurifyWater_PromiseType, fnSetValuePtr))
		return FALSE;

	return TRUE;
}

BOOL CGameConfig::ReadConfig_HotFire() CONST
{
	auto fnSetValuePtr = [](em_TextVar emTextVar, CONST std::wstring& wsValue)
	{
		if (wsValue == L"����")
			MyTools::InvokeClassPtr<CGameVariable>()->SetValueAndGetOldValue_By_Id(emTextVar, 0);
		else if (wsValue == L"�ﹱ")
			MyTools::InvokeClassPtr<CGameVariable>()->SetValueAndGetOldValue_By_Id(emTextVar, 1);
		else
		{
			LOG_MSG_CF(L"���������:[%s]", wsValue.c_str());
			return FALSE;
		}
		return TRUE;
	};

	if (!SetConfigValue(em_Config_Type::em_Config_Type_HotFire, L"���������", em_TextVar::em_TextVar_PurifyWater_PromiseType, fnSetValuePtr))
		return FALSE;

	return TRUE;
}

BOOL CGameConfig::SetFightConfig() CONST
{
	auto fnSetValuePtr = [](em_TextVar emTextVar, CONST std::wstring& wsValue)
	{
		MyTools::InvokeClassPtr<CGameVariable>()->SetValueAndGetOldValue_By_Id(emTextVar, std::stoi(wsValue));
		return TRUE;
	};

	if (!SetConfigValue(em_Config_Type::em_Config_Type_Fight, L"����HP���ڳ�ҩ", em_TextVar::em_TextVar_Person_SupplementHp, fnSetValuePtr))
		return FALSE;

	if (!SetConfigValue(em_Config_Type::em_Config_Type_Fight, L"����MP���ڳ�ҩ", em_TextVar::em_TextVar_Person_SupplementMp, fnSetValuePtr))
		return FALSE;

	if (!SetConfigValue(em_Config_Type::em_Config_Type_Fight, L"����HP���ڳ�ҩ", em_TextVar::em_TextVar_Pet_SupplementHp, fnSetValuePtr))
		return FALSE;

	if (!SetConfigValue(em_Config_Type::em_Config_Type_Fight, L"����MP���ڳ�ҩ", em_TextVar::em_TextVar_Pet_SupplementMp, fnSetValuePtr))
		return FALSE;

	if (!SetConfigValue(em_Config_Type::em_Config_Type_Fight, L"����ս��ģʽ", em_TextVar::em_TextVar_PersonFightMode, [](em_TextVar emTextVar, CONST std::wstring& wsValue)
	{
		if (wsValue == L"�̶�F1")
			MyTools::InvokeClassPtr<CGameVariable>()->SetValueAndGetOldValue_By_Id(emTextVar, em_PersonFightMode::em_PersonFightMode_FixF1);
		else if (wsValue == L"��ͨ����")
			MyTools::InvokeClassPtr<CGameVariable>()->SetValueAndGetOldValue_By_Id(emTextVar, em_PersonFightMode::em_PersonFightMode_NormalAttack);
		else if (wsValue == L"����F1����F2")
			MyTools::InvokeClassPtr<CGameVariable>()->SetValueAndGetOldValue_By_Id(emTextVar, em_PersonFightMode::em_PersonFightMode_SwitchF1F2);
		else
		{
			LOG_MSG_CF(L"������ģʽ:[%s]", wsValue.c_str());
			return FALSE;
		}

		return TRUE;
	}))
	{
		return FALSE;
	}

	if (!SetConfigValue(em_Config_Type::em_Config_Type_Fight, L"����ս��ģʽ", em_TextVar::em_TextVar_PetFightMode, [](em_TextVar emTextVar, CONST std::wstring& wsValue)
	{
		if (wsValue == L"����")
			MyTools::InvokeClassPtr<CGameVariable>()->SetValueAndGetOldValue_By_Id(emTextVar, em_PetFightMode::em_PersonFightMode_Denfence);
		else if (wsValue == L"�̶�F2")
			MyTools::InvokeClassPtr<CGameVariable>()->SetValueAndGetOldValue_By_Id(emTextVar, em_PetFightMode::em_PersonFightMode_Skill);
		else
		{
			LOG_MSG_CF(L"������ģʽ:[%s]", wsValue.c_str());
			return FALSE;
		}

		return TRUE;
	}))
	{
		return FALSE;
	}

	if (!SetConfigValue(em_Config_Type::em_Config_Type_Fight, L"�Զ���������", em_TextVar::em_TextVar_AutoBuyHappyBell, fnSetValuePtr))
		return FALSE;

	return TRUE;
}

BOOL CGameConfig::SetCommonConfig() CONST
{
	auto fnSetValuePtr = [](em_TextVar emTextVar, CONST std::wstring& wsValue)
	{
		MyTools::InvokeClassPtr<CGameVariable>()->SetValueAndGetOldValue_By_Id(emTextVar, std::stoi(wsValue));
		return TRUE;
	};

	if (!SetConfigValue(em_Config_Type::em_Config_Type_Common, L"�Զ�������ħ��", em_TextVar::em_TextVar_AutoBuyExorcism, fnSetValuePtr))
		return FALSE;

	if (!SetConfigValue(em_Config_Type::em_Config_Type_Common, L"ʹ����ħ��", em_TextVar::em_TextVar_UseExorcism, fnSetValuePtr))
		return FALSE;

	//
	if (!SetConfigValue(em_Config_Type::em_Config_Type_Common, L"�������", em_TextVar::em_TextVar_ShieldPlayer, fnSetValuePtr))
		return FALSE;

	if (!SetConfigValue(em_Config_Type::em_Config_Type_Common, L"ʹ�ó��̷�", em_TextVar::em_TextVar_UseReturnSymbol, fnSetValuePtr))
		return FALSE;

	return SetFightConfig();
}
