#include "Common.h"
#include "Utilities.h"

void klog(const char* fmt, ...)
{
	char Buffer[0x200];

	//Create full string from va list.
	va_list args;
	va_start(args, fmt);
	vsprintf(Buffer, fmt, args);
	va_end(args);

	sceKernelDebugOutText(0, Buffer);
}

void Notify(const char* MessageFMT, ...)
{
	NotifyBuffer Buffer;

	//Create full string from va list.
	va_list args;
	va_start(args, MessageFMT);
	vsprintf(Buffer.Message, MessageFMT, args);
	va_end(args);

	//Populate the notify buffer.
	Buffer.Type = NotifyType::NotificationRequest; //this one is just a standard one and will print what ever is stored at the buffer.Message.
	Buffer.unk3 = 0;
	Buffer.UseIconImageUri = 1; //Bool to use a custom uri.
	Buffer.TargetId = -1; //Not sure if name is correct but is always set to -1.
	strcpy(Buffer.Uri, "https://i.imgur.com/SJPIBGG.png"); //Copy the uri to the buffer.

	//From user land we can call int64_t sceKernelSendNotificationRequest(int64_t unk1, char* Buffer, size_t size, int64_t unk2) which is a libkernel import.
	sceKernelSendNotificationRequest(0, (char*)&Buffer, 3120, 0);

	//What sceKernelSendNotificationRequest is doing is opening the device "/dev/notification0" or "/dev/notification1"
	// and writing the NotifyBuffer we created to it. Somewhere in ShellUI it is read and parsed into a json which is where
	// I found some clues on how to build the buffer.
}

struct Myiovec
{
	void* iov_base;
	size_t iov_len;
};

void build_iovec(Myiovec** iov, int* iovlen, const char* name, const void* val, size_t len)
{
	int i;

	if (*iovlen < 0)
		return;

	i = *iovlen;
	*iov = (Myiovec*)realloc(*iov, sizeof **iov * (i + 2));
	if (*iov == NULL) {
		*iovlen = -1;
		return;
	}

	(*iov)[i].iov_base = strdup(name);
	(*iov)[i].iov_len = strlen(name) + 1;
	++i;

	(*iov)[i].iov_base = (void*)val;
	if (len == (size_t)-1) {
		if (val != NULL)
			len = strlen((const char*)val) + 1;
		else
			len = 0;
	}
	(*iov)[i].iov_len = (int)len;

	*iovlen = ++i;
}


/*

vm:
1 - Seems to be system
2 - seems to be app specific.

type:
1 - System,
2 - VRAM

	System,
	Video,
	SharedVideoSystemAsset,
	SharedVideoHighResoAsset
*/

void Get_Page_Table_Stats(int vm, int type, int* Used, int* Free, int* Total)
{
	int _Total = 0, _Free = 0;

	if (get_page_table_stats(vm, type, &_Total, &_Free) == -1) {
		klog("get_page_table_stats() Failed.\n");
		return;
	}

	if (Used)
		*Used = (_Total - _Free);

	if (Free)
		*Free = _Free;

	if (Total)
		*Total = _Total;
}