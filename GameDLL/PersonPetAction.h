#ifndef __DHSH_GAMEDLL_GAME_OBJECT_PET_PERSONPETACTION_H__
#define __DHSH_GAMEDLL_GAME_OBJECT_PET_PERSONPETACTION_H__

#include "GameBase.h"
class CPersonPetAction : public MyTools::CRelfexBaseClass
{
public:
	CPersonPetAction() = default;
	~CPersonPetAction() = default;
	
	// �����ս�����ҳ϶�
	BOOL SupplementLoyalty() CONST;
	
	// ����HP
	VOID SupplementHp() CONST;

	// ����MP
	VOID SupplementMp() CONST;

	// ���ó�ս����
	BOOL SetPetEnterWar() CONST;

public:
	static CPersonPetAction* CreateInstance()
	{
		return new CPersonPetAction;
	}
	
	virtual VOID ReleaseInstance(_In_ LPVOID lpObjectAddr) CONST
	{
		delete reinterpret_cast<CPersonPetAction*>(lpObjectAddr);
	}
};



#endif // !__DHSH_GAMEDLL_GAME_OBJECT_PET_PERSONPETACTION_H__
