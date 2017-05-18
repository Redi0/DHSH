#ifndef __DHSH_GAMEDLL_GAME_OBJECT_PERSON_PERSONATTRIBUTE_H__
#define __DHSH_GAMEDLL_GAME_OBJECT_PERSON_PERSONATTRIBUTE_H__

#include "GameBase.h"

class CPersonAttribute : public MyTools::CRelfexBaseClass
{
public:
	struct PersonBuff
	{
		std::wstring wsName;
		DWORD dwSurpTime;
		DWORD dwObjAddr;
	};
public:
	CPersonAttribute() = default;
	~CPersonAttribute() = default;
	
	// ��ǰ���ڵĵ�ͼ��
	CONST std::wstring GetCurrentMapName() CONST;
	
	// �ȼ�
	DWORD GetLevel() CONST;

	// HP
	DWORD GetHp() CONST;
	DWORD GetMaxHp() CONST;
	DWORD GetPercentHp() CONST;

	// MP
	DWORD GetMp() CONST;
	DWORD GetMaxMp() CONST;
	DWORD GetPercentMp() CONST;

	// Exp
	DWORD GetExp() CONST;

	// InWar?
	BOOL IsWar() CONST;

	BOOL IsMoving() CONST;

	// Point
	Point GetPoint() CONST;

	// dis
	float GetDis(_In_ CONST Point& TarPoint) CONST;

	// 
	DWORD GetMoney() CONST;
	DWORD GetJiaoZiMoney() CONST;

	// ��������
	DWORD GetPetPhysicalStrength() CONST;

	//
	UINT GetVecPersonBuff(_Out_ std::vector<PersonBuff>& Vec, _In_ std::function<BOOL(CONST PersonBuff&)> FilterPtr) CONST;
	BOOL ExistPersonBuff_By_PartName(_In_ CONST std::wstring& wsPartName) CONST;

	//
	BOOL IsCollecting() CONST;

	//
	BOOL IsInHome(_In_ BOOL bIncludeCoutryard = TRUE) CONST;

	// ��԰����ֵ
	DWORD GetHomeVitalityValue() CONST;
public:
	static CPersonAttribute* CreateInstance()
	{
		return new CPersonAttribute;
	}
	
	virtual VOID ReleaseInstance(_In_ LPVOID lpObjectAddr) CONST
	{
		delete reinterpret_cast<CPersonAttribute*>(lpObjectAddr);
	}
};

#endif // !__DHSH_GAMEDLL_GAME_OBJECT_PERSON_PERSONATTRIBUTE_H__
