#ifndef __DHSH_GAMESERVER_ACCOUNT_ACCOUNT_H__
#define __DHSH_GAMESERVER_ACCOUNT_ACCOUNT_H__

#include <atomic>
#include <MyTools/ToolsPublic.h>

class CAccount
{
public:
	CAccount(_In_ DWORD dwAccountId, _In_ CONST std::wstring& wsAccountName, _In_ CONST std::wstring& wsAccountPass, _In_ BOOL bForzen, _In_ INT uHour);
	~CAccount() = default;

	// �ʺ�ID
	DWORD GetAccountId() CONST _NOEXCEPT;

	// ��ȡ��Чʱ��
	UINT GetEffectiveHour() CONST _NOEXCEPT;

	// �Ƿ񱻶�����
	BOOL IsForzen() CONST _NOEXCEPT;

	// �ʺ���
	CONST std::wstring& GetAccountName() CONST _NOEXCEPT;

	//  ����
	CONST std::wstring& GetAccountPass() CONST _NOEXCEPT;

	VOID SetToken(_In_ DWORD ulToken) _NOEXCEPT;

	DWORD GetToken() CONST _NOEXCEPT;

	enum class em_Token_Action { em_Token_Action_Add, em_Token_Action_Dec };
	VOID SetTokenCount(em_Token_Action emAction);

	DWORD GetTokenCount() CONST;


	VOID SetNewTime(_In_ int nTime);
protected:
	std::wstring            _wsAccountName;
	std::wstring            _wsAccountPass;
	BOOL                    _bForzen;
	UINT                    _uEffectiveHour;
	ULONGLONG               _ulEffectiveTick;
	DWORD                   _dwAccountId;
	DWORD					_ulToken;
	DWORD					_dwTokenCount;
};




#endif // !__DHSH_GAMESERVER_ACCOUNT_ACCOUNT_H__
