#pragma once

enum NotifyType
{
	NotificationRequest = 0,
	SystemNotification = 1,
	SystemNotificationWithUserId = 2,
	SystemNotificationWithDeviceId = 3,
	SystemNotificationWithDeviceIdRelatedToUser = 4,
	SystemNotificationWithText = 5,
	SystemNotificationWithTextRelatedToUser = 6,
	SystemNotificationWithErrorCode = 7,
	SystemNotificationWithAppId = 8,
	SystemNotificationWithAppName = 9,
	SystemNotificationWithAppInfo = 9,
	SystemNotificationWithAppNameRelatedToUser = 10,
	SystemNotificationWithParams = 11,
	SendSystemNotificationWithUserName = 12,
	SystemNotificationWithUserNameInfo = 13,
	SendAddressingSystemNotification = 14,
	AddressingSystemNotificationWithDeviceId = 15,
	AddressingSystemNotificationWithUserName = 16,
	AddressingSystemNotificationWithUserId = 17,

	UNK_1 = 100,
	TrcCheckNotificationRequest = 101,
	NpDebugNotificationRequest = 102,
	UNK_2 = 102,
};

struct NotifyBuffer
{ //Naming may be incorrect.
	NotifyType Type;		//0x00 
	int ReqId;				//0x04
	int Priority;			//0x08
	int MsgId;				//0x0C
	int TargetId;			//0x10
	int UserId;				//0x14
	int unk1;				//0x18
	int unk2;				//0x1C
	int AppId;				//0x20
	int ErrorNum;			//0x24
	int unk3;				//0x28
	char UseIconImageUri; 	//0x2C
	char Message[1024]; 	//0x2D
	char Uri[1024]; 		//0x42D
	char unkstr[1024];		//0x82D
}; //Size = 0xC30

#define ARRAY_COUNT(arry) sizeof(arry) / sizeof(arry[0])

typedef unsigned char vm_prot_t;	/* protection codes */

#define	VM_PROT_NONE		((vm_prot_t) 0x00)
#define	VM_PROT_READ		((vm_prot_t) 0x01)
#define	VM_PROT_WRITE		((vm_prot_t) 0x02)
#define	VM_PROT_EXECUTE		((vm_prot_t) 0x04)
#define	VM_PROT_COPY		((vm_prot_t) 0x08)	/* copy-on-read */

#define	VM_PROT_ALL		(VM_PROT_READ|VM_PROT_WRITE|VM_PROT_EXECUTE)
#define VM_PROT_RW		(VM_PROT_READ|VM_PROT_WRITE)
#define	VM_PROT_DEFAULT		VM_PROT_ALL

#define	MNT_UPDATE	0x0000000000010000ULL /* not real mount, just update */

void klog(const char* fmt, ...);
void Notify(const char* MessageFMT, ...);

void Get_Page_Table_Stats(int vm, int type, int* Used, int* Free, int* Total);