#include "Account.h"

CAccount::CAccount(_In_ DWORD dwAccountId, _In_ CONST std::wstring& wsAccountName, _In_ CONST std::wstring& wsAccountPass, _In_ BOOL bForzen, _In_ INT uHour) :
	_dwAccountId(dwAccountId),
	_wsAccountName(wsAccountName),
	_wsAccountPass(wsAccountPass),
	_ulToken(NULL),
	_dwTokenCount(NULL),
	_bForzen(bForzen),
	_uEffectiveHour(uHour < 0 ? 0 : uHour)
{
	_ulEffectiveTick = ::GetTickCount64();
}

DWORD CAccount::GetAccountId() CONST _NOEXCEPT
{
	return _dwAccountId;
}

UINT CAccount::GetEffectiveHour() CONST _NOEXCEPT
{
	UINT uHour = static_cast<decltype(uHour)>((::GetTickCount64() - _ulEffectiveTick) / 1000 / 60 / 60);
	return (_uEffectiveHour == 0 || _ulEffectiveTick == NULL || uHour >= _uEffectiveHour) ? 0 : _uEffectiveHour - uHour;
}

BOOL CAccount::IsForzen() CONST _NOEXCEPT
{
	return _bForzen;
}

CONST std::wstring& CAccount::GetAccountName() CONST _NOEXCEPT
{
	return _wsAccountName;
}

CONST std::wstring& CAccount::GetAccountPass() CONST _NOEXCEPT
{
	return _wsAccountPass;
}

VOID CAccount::SetToken(_In_ DWORD ulToken) _NOEXCEPT
{
	_ulToken = ulToken;
}

DWORD CAccount::GetToken() CONST _NOEXCEPT
{
	return _ulToken;
}

VOID CAccount::SetTokenCount(em_Token_Action emAction)
{
	switch (emAction)
	{
	case CAccount::em_Token_Action::em_Token_Action_Add:
		_dwTokenCount += 1;
		break;
	case CAccount::em_Token_Action::em_Token_Action_Dec:
		_dwTokenCount -= 1;
		break;
	default:
		break;
	}
}

DWORD CAccount::GetTokenCount() CONST
{
	return _dwTokenCount;
}

VOID CAccount::SetNewTime(_In_ int nTime)
{
	if (nTime > 0)
	{
		_uEffectiveHour = nTime;
		_ulEffectiveTick = ::GetTickCount64();
	}
}
