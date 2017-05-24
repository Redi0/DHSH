#include "EchoPacket.h"
#include <MyTools/CLPublic.h>
#include <MyTools/CLFile.h>
#include <MyTools/Log.h>
#include "AccountAction.h"
#include "GameClient.h"
#include "Account.h"
#include "../GameDLL/GameBase.h"
#include "DbManager.h"
#include "AccountConfigExtend.h"
#include "AccountExtend.h"

#define _SELF L"EchoPacket.cpp"

CEchoPacket::CEchoPacket() : _LockVersion(L"CEchoPacket._LockVersion")
{

}

BOOL CEchoPacket::AccountLogin(_In_ CGameClient* pGameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST
{
	struct LoginPacket
	{
		std::wstring wsAccountName;
		std::wstring wsAccountPass;
		std::wstring wsGameName;
		float		 fVersion;
	};
	
	auto LoginContent_ = ExtractPacket<LoginPacket>([&pSocketBuffer]
	{
		LoginPacket LoginContent_;
		*pSocketBuffer >> LoginContent_.wsAccountName >> LoginContent_.wsAccountPass >> LoginContent_.wsGameName >> LoginContent_.fVersion;
		return LoginContent_;
	});

	pSocketBuffer->clear();
	if (!CAccountAction::GetInstance().Login(LoginContent_.wsAccountName, LoginContent_.wsAccountPass, pGameClient->GetAccount()))
	{
		pSocketBuffer->InitializeHead(em_Sock_Msg::em_Sock_Msg_AccountLogin);
		*pSocketBuffer << FALSE;
		return TRUE;
	}
	else if (!CheckVersion(LoginContent_.wsGameName, LoginContent_.fVersion))
	{
		pSocketBuffer->InitializeHead(em_Sock_Msg::em_Sock_Msg_ServerText);
		*pSocketBuffer << L"�汾�Ų��ԡ������ȡ���µİ汾!";
		return TRUE;
	}
	
	pSocketBuffer->InitializeHead(em_Sock_Msg::em_Sock_Msg_AccountLogin);
	pGameClient->SetToken(static_cast<ULONGLONG>(MyTools::CCharacter::GetRand_For_DWORD()));
	pGameClient->GetAccount()->SetToken(pGameClient->GetToken());

	*pSocketBuffer << TRUE << pGameClient->GetToken();
	return TRUE;
}

BOOL CEchoPacket::GameLogin(_In_ CGameClient* pGameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST
{
	struct GameLoginPacket
	{
		std::wstring wsAccountName;
		std::wstring wsAccountPass;
		DWORD		 dwToken;
	};

	auto LoginContent_ = ExtractPacket<GameLoginPacket>([&pSocketBuffer]
	{
		GameLoginPacket LoginContent_;
		*pSocketBuffer >> LoginContent_.wsAccountName >> LoginContent_.wsAccountPass << LoginContent_.dwToken;;
		return LoginContent_;
	});

	pSocketBuffer->clear();
	if (!CAccountAction::GetInstance().Login(LoginContent_.wsAccountName, LoginContent_.wsAccountPass, pGameClient->GetAccount()))
	{
		pSocketBuffer->InitializeHead(em_Sock_Msg::em_Sock_Msg_GameLogin);
		*pSocketBuffer << FALSE;
		return TRUE;
	}
	else if (!CheckIsLogin(pGameClient, pSocketBuffer))
		return TRUE;
	else if (pGameClient->GetAccount()->GetTokenCount() >= 10)
	{
		pSocketBuffer->InitializeHead(em_Sock_Msg::em_Sock_Msg_ServerText);
		*pSocketBuffer << L"��¼�Ѵﵽ��������10����!";
		return TRUE;
	}


	pGameClient->GetAccount()->SetTokenCount(CAccount::em_Token_Action::em_Token_Action_Add);
	return TRUE;
}

BOOL CEchoPacket::Register(_In_ CGameClient* pGameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST
{
	struct RegisterPacket
	{
		std::wstring wsAccountName;
		std::wstring wsAccountPass;
	};

	auto RegisterContent = ExtractPacket<RegisterPacket>([&pSocketBuffer]
	{
		RegisterPacket LoginContent_;
		*pSocketBuffer >> LoginContent_.wsAccountName >> LoginContent_.wsAccountPass;
		return LoginContent_;
	});

	

	std::wstring wsText;
	if (!CDbManager::GetInstance().RegisterAccount(RegisterContent.wsAccountName, RegisterContent.wsAccountPass, pGameClient->GetIP(), wsText))
	{
		pSocketBuffer->clear();
		pSocketBuffer->InitializeHead(em_Sock_Msg::em_Sock_Msg_ServerText);
		*pSocketBuffer << L"�����ʺ�ʧ�ܡ�����ϵ���߲�ѯ��ϸԭ��!";
		return TRUE;
	}
	
	pSocketBuffer->clear();
	pSocketBuffer->InitializeHead(em_Sock_Msg::em_Sock_Msg_Register);
	*pSocketBuffer << wsText;
	return TRUE;
}

BOOL CEchoPacket::KeepALive(_In_ CGameClient* pGameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST
{
	pGameClient->SetKeepALiveTick();
	pSocketBuffer->clear();
	return TRUE;
}

BOOL CEchoPacket::QueryAccount(_In_ CGameClient* pGameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST
{
	std::vector<std::wstring> Vec;
	CDbManager::GetInstance().GetAccountCardLog(pGameClient->GetAccount()->GetAccountId(), Vec);

	pSocketBuffer->clear();
	pSocketBuffer->InitializeHead(em_Sock_Msg::em_Sock_Msg_QueryCard);

	
	*pSocketBuffer << Vec.size();
	for (CONST auto& itm : Vec)
		*pSocketBuffer << itm;

	return TRUE;
}

BOOL CEchoPacket::RechargeCard(_In_ CGameClient* pGameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST
{
	std::wstring wsCardNo;
	*pSocketBuffer >> wsCardNo;

	pSocketBuffer->clear();
	pSocketBuffer->InitializeHead(em_Sock_Msg::em_Sock_Msg_Recharge);

	std::wstring wsText;
	if (CDbManager::GetInstance().RechargeCard(pGameClient->GetAccount()->GetAccountId(), wsCardNo, wsText))
		*pSocketBuffer << wsText;
	else
		*pSocketBuffer << L"��ֵʧ�ܡ�������ѯ���߲�ѯ��ϸԭ��!";

	return TRUE;
}

BOOL CEchoPacket::ReadConfig(_In_ CGameClient* pGameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST
{
	auto Vec = ExtractPacket<std::vector<std::wstring>>([&pSocketBuffer]
	{
		std::vector<std::wstring> Vec;

		DWORD dwSize = 0;
		*pSocketBuffer >> dwSize;

		std::wstring wsConfigName;
		for (decltype(dwSize) i = 0; i < dwSize; ++i)
		{
			*pSocketBuffer >> wsConfigName;
			Vec.push_back(wsConfigName);
		}

		return Vec;
	});

	pSocketBuffer->clear();
	pSocketBuffer->InitializeHead(em_Sock_Msg::em_Sock_Msg_ReadConfig);

	auto AccountId = pGameClient->GetAccount()->GetAccountId();
	for (CONST auto& itm : Vec)
	{
		std::wstring wsConfigValue = L"Empty";
		CAccountConfigExtend::GetInstance().FindConfig(AccountId, itm, wsConfigValue);
		
		*pSocketBuffer << itm << wsConfigValue;
	}
	return TRUE;
}

BOOL CEchoPacket::WriteConfig(_In_ CGameClient* pGameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST
{
	auto Vec = ExtractPacket<std::vector<CAccountConfigExtend::AccountConfig>>([&pSocketBuffer]
	{
		std::vector<CAccountConfigExtend::AccountConfig> Vec;

		DWORD dwSize = 0;
		*pSocketBuffer >> dwSize;
		for (decltype(dwSize) i = 0;i < dwSize; ++i)
		{
			CAccountConfigExtend::AccountConfig ConfigPacket_;
			*pSocketBuffer >> ConfigPacket_.wsConfigName >> ConfigPacket_.wsConfigValue;
			Vec.push_back(std::move(ConfigPacket_));
		}
	
		return Vec;
	});

	pSocketBuffer->clear();
	pSocketBuffer->InitializeHead(em_Sock_Msg::em_Sock_Msg_WriteConfig);

	auto AccountId = pGameClient->GetAccount()->GetAccountId();
	CAccountConfigExtend::GetInstance().WriteConfig(AccountId, Vec);

	return TRUE;
}

BOOL CEchoPacket::QueryTime(_In_ CGameClient* pGameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST
{
	pSocketBuffer->InitializeHead(em_Sock_Msg::em_Sock_Msg_QueryHour);
	*pSocketBuffer << pGameClient->GetAccount()->GetEffectiveHour();
	return TRUE;
}

BOOL CEchoPacket::CheckIsLogin(_In_ CGameClient* pGameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST
{
	if (pGameClient->GetAccount() == nullptr)
	{
		pSocketBuffer->clear();
		pSocketBuffer->InitializeHead(em_Sock_Msg::em_Sock_Msg_ServerText);
		*pSocketBuffer << L"���ʺŻ�û��¼";
		return FALSE;
	}
	else if (pGameClient->GetToken() != pGameClient->GetAccount()->GetToken())
	{
		pSocketBuffer->clear();
		pSocketBuffer->InitializeHead(em_Sock_Msg::em_Sock_Msg_ServerText);
		*pSocketBuffer << L"���Ѿ������������ˡ���";
		return FALSE;
	}
	else if (pGameClient->GetAccount()->IsForzen())
	{
		pSocketBuffer->clear();
		pSocketBuffer->InitializeHead(em_Sock_Msg::em_Sock_Msg_ServerText);
		*pSocketBuffer << L"���ʺ��Ѿ���������!";
		return FALSE;
	}
	else if (pGameClient->GetAccount()->GetToken() != pGameClient->GetToken())
	{
		pSocketBuffer->clear();
		pSocketBuffer->InitializeHead(em_Sock_Msg::em_Sock_Msg_ServerText);
		*pSocketBuffer << L"����ʺ��Ѿ������µ�¼�ˡ���";
		return FALSE;
	}

	return TRUE;
}

BOOL CEchoPacket::ReadLocalVersion()
{
	auto fnAddVersion = [this](CONST std::wstring& wsGameName, float fVersion)
	{
		_VecVersion.push_back(VersionContent{ wsGameName, fVersion });
	};

	std::wstring wsDHSHVersion;
	if (!MyTools::CLFile::ReadUnicodeConfig(L"\\Config.ini", L"Version", L"DHSH", wsDHSHVersion))
		return FALSE;

	_VecVersion.clear();
	fnAddVersion(L"DHSH",static_cast<float>(_wtof(wsDHSHVersion.c_str())));

	return TRUE;
}


BOOL CEchoPacket::CheckVersion(_In_ CONST std::wstring& wsGameName, _In_ float fVersion) CONST
{
	BOOL bExist = FALSE;
	_LockVersion.Access([=, &bExist]
	{
		bExist = MyTools::CLPublic::Vec_find_if_Const(_VecVersion, [=](CONST VersionContent& VC) { return VC.wsGameName == wsGameName && fVersion == VC.fVersion; }) != nullptr;
	});

	return bExist;
}