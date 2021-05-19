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

void Get_Page_Table_Stats(int *cpuUsed, int *cpuTotal, int *gpuUsed, int *gpuTotal)
{
	int CPU_Available, GPU_Available;
	if (!sceKernelGetPageTableStats(cpuTotal, &CPU_Available, gpuTotal, &GPU_Available))
	{
		*cpuUsed = (*cpuTotal - CPU_Available);
		*gpuUsed = (*gpuTotal - GPU_Available);
	}
}

/*unsigned long Syscall(unsigned int n, ...) {
	asm(".intel_syntax noprefix");
	asm("xor %rax, %rax");
	asm("mov %r10, %rcx");
	asm("syscall");
	asm("ret");
}*/

/*int nmount(Myiovec *iov, uint32_t niov, int flags)
{
	return Syscall(378, iov, niov, flags);
}

int unmount(const char *dir, int flags)
{
	return Syscall(22, dir, flags);
}

int mount_large_fs(const char* device, const char* mountpoint, const char* fstype, const char* mode, unsigned int flags) 
{
	Myiovec* iov = NULL;
	int iovlen = 0;

	//unmount(mountpoint, 0);

	build_iovec(&iov, &iovlen, "fstype", fstype, -1);
	build_iovec(&iov, &iovlen, "fspath", mountpoint, -1);
	build_iovec(&iov, &iovlen, "from", device, -1);
	build_iovec(&iov, &iovlen, "large", "yes", -1);
	build_iovec(&iov, &iovlen, "timezone", "static", -1);
	build_iovec(&iov, &iovlen, "async", "", -1);
	build_iovec(&iov, &iovlen, "ignoreacl", "", -1);

	if (mode) {
		build_iovec(&iov, &iovlen, "dirmask", mode, -1);
		build_iovec(&iov, &iovlen, "mask", mode, -1);
	}

	return nmount(iov, iovlen, flags);
}*/