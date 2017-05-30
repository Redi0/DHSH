#ifndef __DHSH_GAMEDLL_GAME_COMMON_GAMEBASE_H__
#define __DHSH_GAMEDLL_GAME_COMMON_GAMEBASE_H__

#include <MyTools/RelfexClass.h>

#define C_renwu_zhanghao  0x91B644
#define C_renwu_qu        0x91B6B0
#define C_renwu_name      0x91B678
#define C_renwu_base      0x8F5EA8
#define C_renwu_DQHP      0x5E8
#define C_renwu_ְҵ      0x4F8
#define C_renwu_MHP       C_renwu_DQHP + 0x0C
#define C_renwu_DQMP      C_renwu_DQHP + 04 
#define C_renwu_MMP       C_renwu_DQHP + 0x10 
#define C_renwu_lev       C_renwu_DQHP - 8 
#define C_renwu_ID        C_renwu_DQHP - 0x0F4
#define C_renwu_qianli    C_renwu_DQHP - 0x0C
///////////////////////////////////////////////////////����
#define C_renwu_Skill_base   0x8F60E0
#define C_Skill_office       0x498
#define C_Skill_Num          0x264

#define C_Pet_Skill_base     0x943808
#define C_Get_Skill_name_CALL  0x006CD930
////////////////////////////////////////////////////////
//����
///////////////////////////////////////////////////////
#define C_chong_base     0x8F98C0
#define C_chong_yiji     0x598
#define C_chong_shu      C_chong_yiji + 4
#define C_chong_DQHP     0x46
#define C_chong_MHP      0x52
#define C_chong_DQMP     0x4A
#define C_chong_MMP      0x56
#define C_chong_ID       0x0
#define C_chong_zhongcheng 0x1D
#define C_chong_name     0x9
#define C_chong_qianli   0x40
#define C_chong_chuzhan  0x7C
/////////////////////////////////////////////////////
//��ǰ��ս��������
/////////////////////////////////////////////////////
#define C_chuzhan_base   0x8F9DF4//?
#define C_chuzhan_dq     0x5Bc
#define C_EXP            0x588//[588]+258
#define C_chuzhan_MAX    0x5C4
////////////////////////////////////////////////////
//��ͼ����
///////////////////////////////////////////////////
#define C_map_base       0x8F494C    //+10 ���ھ��
#define C_map_name       0x184
#define C_map_Addr       0x910
#define C_map_AddX       0x2a
#define C_map_AddY       0x2e
#define C_ZOU_pi_1       0xA8
#define C_ZOU_pi_2       0x20

#define C_map_isRun      0x74//��ͼ�Ƿ���Ѱ·��
///////////////////////////////////////////////////
//����
///////////////////////////////////////////////////
#define C_jiasu          0x1BC
#define C_dizhi          0x00666B00
#define C_duiyuan_HOOK   0x005FA556
///////////////////////////////////////////////////
//��Ǯ
///////////////////////////////////////////////////
#define C_money_base     0xAE7C8C
#define C_money_yiji     0x10
#define C_money_erji     0x14
#define C_money_siji     0x10
#define C_money_wuji     0x10
#define C_money_liuji    4
#define C_money_jiao     8
#define C_money_money    0
//////////////////////////////////////////////////
//����
/////////////////////////////////////////////////
#define C_Box_base      0x8F8C08//?
#define C_Box_yiji      4
#define C_Box_num       8
#define C_Box_jia       0xc
#define C_BOX_name1     0x4b
#define C_box_name      8
#define C_Box_lei       4
#define C_Box_ID        0
#define C_BOX_shuxing1  0x33
//////////////////////////////////////////////////
//����
//////////////////////////////////////////////////
#define	C_huan_base     0x921190
#define C_huan_yiji     4
#define C_huan_name     4
#define C_huan_ID       0x24
#define C_huan_X        C_map_AddX
#define C_huan_Y        C_map_AddY
#define C_huan_tiaojian 0x33
#define C_huan_si       0x78 //1Ϊ����
//////////////////////////////////////////////
//״̬
//////////////////////////////////////////////
#define C_state_base    0x8F5EF0//?
#define C_state_yiji    0x4a0
#define C_state_erji    0x290
#define C_state_shu     0xac   //290+ac ������
#define C_state_surp1   0x78
#define C_state_surp    0x24
#define C_state_name    0x8
////////////////////////////////////////////
//NPC ״̬
////////////////////////////////////////////
#define C_NPC_state    0x8F6070// ?
#define C_NPC_xuan_base  0x454
#define C_NPC_xuan_shu   0x458
#define C_NPC_xuan_str   0x1c0
#define C_NPC_xuan_dianji 0x50
#define C_NPC_state1      0x16C
#define C_NPC_neirong     0x148
////////////////////////////////////////////
//����
////////////////////////////////////////////
#define C_task_base      0x8F9DDC
#define C_task_yiji      0x149C
#define C_task_erji      0x390
#define C_task_name      0x54
#define C_task_shu       C_task_erji + 4
#define C_task_tishi     0x104
#define C_task_tishi1    0x24
////////////////////////////////////////////
//����
////////////////////////////////////////////
//#define C_dati_base      0x9D7100
//#define C_yanzheng_yiji  0xc0
//#define C_zudui_kuang    0x1a0
//#define C_yanzheng_erji  0xc4
#define C_dati_yiji      0x4A8
#define C_dati_erji      C_state_erji
//
//#define C_dati_state     0x83709C
//#define C_dati_kuang     0x210
///////////////////////////////////////////
//�̻�
///////////////////////////////////////////
#define C_shanghui_base  0x83DEA8
#define C_shanghui_Offset 0x484
#define C_shanghui_Offset1 0x278
#define C_shanghui_Offset2 0x9c
#define C_shanghui_name    0x4
#define C_shanghui_jiage   0x2b
#define C_shanghui_win_zhuangtai   0x160
///////////////////////////////////////////
//�ɼ�
///////////////////////////////////////////
#define C_caiji_base        0x8F8DA4//?
#define C_caiji_offset      0x8
#define C_caiji_chong_base   0x8F8D94

//#define C_chongwu_chushi    0x0050DEB0
//#define C_chongwu_base      0x0050CE40 
#define C_chongwu_tili      0x26
#define C_caiji_CALL_chushi 0x0051C7E0
#define C_caiji_CALL        0x0051CA70
////////////////////////////////////////////
//UI ��������
////////////////////////////////////////////
#define C_Monster_base           0x8F6110
#define C_Monster_UI             0x49C
#define C_Monster_UI_1           0x290
///////////////////////////////////////////////
/////��������
///////////////////////////////////////////////
#define C_window_base         0xAE7BA0 //?
#define C_window_offset       0x0C8


/////////////////////////////////////////////
//����
////////////////////////////////////////////
#define C_Team_base               0x8F85A4
#define C_Team_����               0x75 //1Ϊ���� 0Ϊ���
#define C_Team_Lev                0x14
#define C_Team_Map                0x7E
#define C_Team_ID                 0x18
#define C_Team_����               0x2F0
////////////////////////////////////////////
//CALL ����
////////////////////////////////////////////

//#define C_CALL_dati         0x007491D0  0x5332B0
#define C_CALL_dati         0x5340B0 //������CALL �ϲ�
#define C_CALL_dati1        0x007D4285
#define C_CALL_dati2        0x00534040
#define C_CALL_fabao_base   0x8F4950
#define C_CALL_fabao        0x0064FAD0
#define C_CALL_close_maimai  C_NPC_state
#define C_CALL_CloseNPC     0x00752700
//#define C_CALL_hanhua       0x005FEAD0
#define C_CALL_zoulu        0x0042FB80
#define  C_xunlu_base       0x8F5E64 //C_CALL_xunlu1 ��
#define C_CALL_xunlu1       0x0042FAD0
#define C_CALL_xunlu        0x00430360
#define C_call_dianji       0x006505C0//?
#define C_Set_Skill_base    0x8F60B8
#define C_CALL_Set_Skill    0x00489980
#define C_base_kuaijie      0x8F8CBC// ��ݼ���ַ ??
#define C_CALL_kuaijie      0x004FA260// ��ݼ�CALL ??
#define C_base_dianguai     0x8FB14C//ս����������ַ //?
#define C_zhandou_huihe     0x0c      //ս���غ���
#define C_base_xing         0x40      // ��ݼ���̬
#define C_base_baobaoxing   0x84     //������ݼ���̬
#define	C_base_zhuangtai    0x10      // ս����������״̬ 5,����δ����  6����δ���� 0 δ����ս��  3�������ڹ���
#define C_CALL_dianguai     0x00642440// ���ܵ������
#define C_CALL_ADDHP        0x006506E0
#define C_CALL_maichu      C_CALL_ADDHP
#define C_chaxun_Offset     0x6E4 //--
#define C_chushi_Offset     0x4C8 //--
#define C_CALL_chushi       0x0053A130 //--
#define C_CALL_chaxun       0x00539EE0 //--
#define C_call_jianwu       C_CALL_ADDHP   //
#define C_CALL_taopao       0x00642810

//#define C_jiasu_xuhanshu    0x7E8644
#define C_zidong_base       0x8FB148
#define C_CALL_zidong       0x0063F710

#define C_Auto_Skill        C_CALL_taopao

#define C_Auto_Skill_      0x00645740
#define C_CALL_IsFly       0x006AC180//0x00693580

#define C_CALL_NPC_Ѱ·CALL  C_CALL_xunlu1
#define C_CALL_NPC_Ѱ·CALL1  0x004305F0


#define C_CALL_��ȭ          0x00641A30

#define C_CALL_Open_give    0x004C9EC0
#define C_CALL_Start_�����  0x00650740
#define C_CALL_give_Enter            0x004C9C00//ȷ�ϸ���
#define C_CALL_Give_Set            0x0076D800
#define C_CALL_Give_Set1            0x00770C00
#define C_CALL_Give_Set2           0x0076DAB0



#define C_verify_Base           0x90B630
#define C_CALL_Key_InPut        0x00772DD0
#define C_CALL_Login_Base       0x8F8CCC
#define C_CALL_Login             0x004FC920
#define C_CALL_Check_Btn        0x0077F8C0

#define �Զ���·CALL 0x409350
#define NPCѡ���ı�ƫ�� 0x24C
#define NPCѡ���ƫ�� 0x58
#define ��԰��ַ 0x8F8B60
#define ��԰ƫ�� 0x498
#define ��԰����ƫ�� 0x2B

#define ReadDWORD(Addr) MyTools::CCharacter::ReadDWORD(Addr)

#define WM_CUSTOME_HOME 0x400 + 0x52
#define WM_CUSTOME_END  0x400 + 0x53

#define SERVERIP L"192.168.0.104"
#define SERVERPORT 12345

#define SZSHARENAME_CONSOLE L"CL_SHARE_DHSH"

enum class em_GameStatus
{
	em_GameStatus_None,
	em_GameStatus_Running,
	em_GameStatus_Stop
};

extern em_GameStatus g_GameStatus;
#define GameRun (g_GameStatus == em_GameStatus::em_GameStatus_Running)
#define StopGame  g_GameStatus = em_GameStatus::em_GameStatus_Stop
#define StartGame g_GameStatus = em_GameStatus::em_GameStatus_Running

extern VOID GameSleep(_In_ DWORD dwTime);

enum class em_TextVar
{
	// �Ƿ������ڲ���ģʽ��
	em_TextVar_IsRunDlg,
	// �Ƿ�ʹ����ħ�� = 1
	em_TextVar_UseExorcism,
	// ����HP<70 ��ҩ
	em_TextVar_Person_SupplementHp,
	// ����MP<70 ��ҩ
	em_TextVar_Person_SupplementMp,
	// ����HP<70 ��ҩ
	em_TextVar_Pet_SupplementHp,
	// ����MP<70 ��ҩ
	em_TextVar_Pet_SupplementMp,
	// �����ս��ģʽ
	em_TextVar_PersonFightMode,
	// �����ս��ģʽ
	em_TextVar_PetFightMode,
	// �Զ�������ħ��
	em_TextVar_AutoBuyExorcism,
	// �Զ���������
	em_TextVar_AutoBuyHappyBell,
	// ���������������
	em_TextVar_AutoBuyPetDrinksCount,
	// ��԰�ɼ��ļҾ�����
	em_TextVar_CollectHome_Furniture
};


#define GAMEVERSION 1.01f
enum em_Sock_Msg
{
	em_Sock_Msg_KeepLive = 0x1,
	em_Sock_Msg_QueryCard,
	em_Sock_Msg_Recharge,
	em_Sock_Msg_ReadConfig,
	em_Sock_Msg_WriteConfig,
	em_Sock_Msg_AccountLogin,
	em_Sock_Msg_GameLogin,
	em_Sock_Msg_Register,
	em_Sock_Msg_QueryHour,
	em_Sock_Msg_ServerText,
};


typedef struct _Point
{
	DWORD X, Y;

	_Point()
	{
		X = Y = NULL;
	}

	_Point(_In_ DWORD dwX, _In_ DWORD dwY) : X(dwX), Y(dwY)
	{

	}

	bool operator == (CONST _Point& Point_) CONST
	{
		return this->X == Point_.X && this->Y == Point_.Y;
	}

	bool operator != (CONST _Point& Point_) CONST
	{
		return this->X != Point_.X || this->Y != Point_.Y;
	}

	DWORD Encode()
	{
		return X << 16 | Y;
	}

	static _Point Decode(_In_ DWORD dwValue)
	{
		_Point Pt;
		Pt.Y = dwValue & 0xFFFF;
		Pt.X = dwValue >> 16;
		return Pt;
	}

}Point;


struct ConfigText
{
	std::wstring wsConfigName;
	std::wstring wsConfigValue;
};


///////Account Share///////////////////////////////////////////////////////////////////

struct GameAccountContent
{
	WCHAR wszPlayerName[64];
};

struct GameAccountStatus
{
	BOOL bExist;
	BOOL bLogining;
	BOOL bClose;
	UINT uLoginFaildCount;
	ULONGLONG ulOnlineTick;
	BOOL bAlive;
	DWORD dwPid;
	HWND hGameWnd;
};

enum em_PersonFightMode : int
{
	em_PersonFightMode_None = -1,
	em_PersonFightMode_FixF1,
	em_PersonFightMode_NormalAttack,
	em_PersonFightMode_SwitchF1F2
};

enum em_PetFightMode : int
{
	em_PetFightMode_None = -1,
	em_PersonFightMode_Denfence,
	em_PersonFightMode_Skill
};

struct GameAccountConfig
{
	
};

typedef struct GameAccountShareContent
{
	GameAccountContent AccountContent;
	GameAccountStatus  AccountStatus;
	GameAccountConfig  AccountConfig;
	UINT               uIndex;
}*PGameAccountContent;

struct GameShareGlobalConfig
{
	WCHAR	  wszAccountName[32];
	WCHAR     wszAccountPass[32];
	DWORD     dwToken;
	DWORD     dwHour;
	ULONGLONG ulTimeTick;
	DWORD     dwConsoleTitle;
};

enum 
{
	em_Max_GameAccount_Count = 10
};
typedef struct GameShareContent
{
	WCHAR wszConsolePath[MAX_PATH];
	WCHAR wszGamePath[MAX_PATH];

	UINT  uAccountCount;
	GameAccountShareContent arrGameArrount[em_Max_GameAccount_Count];
	GameShareGlobalConfig  GlobalConfig;
}*PGameShareContent;

#endif
