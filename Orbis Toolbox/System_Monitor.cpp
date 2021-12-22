#include "Common.h"
#include "System_Monitor.h"
#include "Game_Overlay.h"

int System_Monitor::Thread_Count = 0;
float System_Monitor::Usage[8] = { 0 };
float System_Monitor::Average_Usage;
int System_Monitor::CPU_Temp;
int System_Monitor::SOC_Temp;
System_Monitor::Memory System_Monitor::RAM;
System_Monitor::Memory System_Monitor::VRAM;

bool System_Monitor::Should_Run_Thread = true;
Proc_Stats System_Monitor::Stat_Data[3072];
System_Monitor::thread_usages System_Monitor::gThread_Data[2];

void System_Monitor::calc_usage(unsigned int idle_tid[8], thread_usages* cur, thread_usages* prev, float usage_out[8])
{
	if (cur->Thread_Count <= 0 || prev->Thread_Count <= 0) //Make sure our banks have threads
		return;

	//Calculate the Current time difference from the last bank to the current bank.
	float Current_Time_Total = ((prev->current_time.tv_sec + (prev->current_time.tv_nsec / 1000000000.0f)) - (cur->current_time.tv_sec + (cur->current_time.tv_nsec / 1000000000.0f)));

	//Here this could use to be improved but essetially what its doing is finding the thread information for the idle threads using their thread Index stored from before.
	struct Data_s
	{
		Proc_Stats* Cur;
		Proc_Stats* Prev;
	}Data[8];

	for (int i = 0; i < cur->Thread_Count; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (idle_tid[j] == cur->Threads[i].td_tid)
				Data[j].Cur = &cur->Threads[i];
		}
	}

	for (int i = 0; i < prev->Thread_Count; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (idle_tid[j] == prev->Threads[i].td_tid)
				Data[j].Prev = &prev->Threads[i];
		}
	}

	//Here we loop through each core to calculate the total usage time as its split into user/sustem
	for (int i = 0; i < 8; i++)
	{
		float Prev_Usage_Time = (Data[i].Prev->system_cpu_usage_time.tv_sec + (Data[i].Prev->system_cpu_usage_time.tv_nsec / 1000000.0f));
		Prev_Usage_Time += (Data[i].Prev->user_cpu_usage_time.tv_sec + (Data[i].Prev->user_cpu_usage_time.tv_nsec / 1000000.0f));

		float Cur_Usage_Time = (Data[i].Cur->system_cpu_usage_time.tv_sec + (Data[i].Cur->system_cpu_usage_time.tv_nsec / 1000000.0f));
		Cur_Usage_Time += (Data[i].Cur->user_cpu_usage_time.tv_sec + (Data[i].Cur->user_cpu_usage_time.tv_nsec / 1000000.0f));

		//We calculate the usage using usage time difference between the two samples divided by the current time difference.
		float Idle_Usage = ((Prev_Usage_Time - Cur_Usage_Time) / Current_Time_Total);

		if (Idle_Usage > 1.0f)
			Idle_Usage = 1.0f;

		if (Idle_Usage < 0.0f)
			Idle_Usage = 0.0f;

		//Get inverse of idle percentage and express in percent.
		usage_out[i] = (1.0f - Idle_Usage) * 100.0f;
	}
}

void* System_Monitor::Monitor_Thread(void* args)
{
	//klog("[System Monitor] Thread Started\n");

	unsigned int Idle_Thread_ID[8];

	int Thread_Count = 3072;
	if (!sceKernelGetCpuUsage((Proc_Stats*)&Stat_Data, (int*)&Thread_Count) && Thread_Count > 0)
	{
		char Thread_Name[0x40];
		int Core_Count = 0;
		for (int i = 0; i < Thread_Count; i++)
		{
			if (!sceKernelGetThreadName(Stat_Data[i].td_tid, Thread_Name) && sscanf(Thread_Name, "SceIdleCpu%d", &Core_Count) == 1 && Core_Count <= 7)
			{
				//klog("[System Monitor][SceIdleCpu%d] -> %i\n", Core_Count, Stat_Data[i].td_tid);

				Idle_Thread_ID[Core_Count] = Stat_Data[i].td_tid;
			}
		}
	}

	//klog("[System Monitor] Starting Monitor...\n");
	int Current_Bank = 0;
	while (Should_Run_Thread)
	{
		//klog("Getting Bank %i\n", Current_Bank);
		if (Game_Overlay::Show_CPU_Usage || Game_Overlay::Show_Thread_Count)
		{
			//grab thread data with max threads of 3072.
			gThread_Data[Current_Bank].Thread_Count = 3072;
			if (!sceKernelGetCpuUsage((Proc_Stats*)&gThread_Data[Current_Bank].Threads, &gThread_Data[Current_Bank].Thread_Count))
			{
				//Store the thread count.
				System_Monitor::Thread_Count = gThread_Data[Current_Bank].Thread_Count;

				//klog("ThreadCount[%i] = %i\n", Current_Bank, gThread_Data[Current_Bank].Thread_Count);

				//Set the current time.
				sceKernelClockGettime(4, &gThread_Data[Current_Bank].current_time);

				//flip to other bank.
				Current_Bank = !Current_Bank;

				//make sure bank has threads
				if (gThread_Data[Current_Bank].Thread_Count <= 0)
					continue;

				//Calculate usage using thread data.
				calc_usage(Idle_Thread_ID, &gThread_Data[!Current_Bank], &gThread_Data[Current_Bank], Usage);

				/*klog("CPU Utilization: %2.0f%% %2.0f%% %2.0f%% %2.0f%% %2.0f%% %2.0f%% %2.0f%% %2.0f%%\n",
					Usage[0], Usage[1], Usage[2], Usage[3],
					Usage[4], Usage[5], Usage[6], Usage[7]);*/

				System_Monitor::Average_Usage = ((Usage[0] + Usage[1] + Usage[2] + Usage[3] + Usage[4] + Usage[5] + Usage[6] + Usage[7]) / 8.0f);
			}
		}

		if (Game_Overlay::Show_CPU_Temp)
		{
			sceKernelGetCpuTemperature(&CPU_Temp);
		}

		if (Game_Overlay::Show_SOC_Temp)
		{
			sceKernelGetSocSensorTemperature(0, &SOC_Temp);
		}

		if (Game_Overlay::Show_ram)
		{
			Get_Page_Table_Stats(1, 1, &RAM.Used, &RAM.Free, &RAM.Total);
			RAM.Percentage = (((float)RAM.Used / (float)RAM.Total) * 100.0f);
		}

		if (Game_Overlay::Show_vram)
		{
			Get_Page_Table_Stats(1, 2, &VRAM.Used, &VRAM.Free, &VRAM.Total);
			VRAM.Percentage = (((float)VRAM.Used / (float)VRAM.Total) * 100.0f);
		}

		sceKernelSleep(2);
	}

	Should_Run_Thread = true;
	klog("[System Monitor] Thread Shutdown.\n");
	void* res;
	scePthreadExit(res);
	return res;
}

void System_Monitor::Init()
{
	klog("[System Monitor] Starting System Monitor Thread...\n");

	OrbisPthreadAttr attr;
	scePthreadAttrInit(&attr);

	scePthreadAttrSetstacksize(&attr, 0x80000);

	OrbisPthread* id;
	scePthreadCreate(&id, &attr, Monitor_Thread, NULL, "System Monitor Thread");
}

void System_Monitor::Term()
{
	Should_Run_Thread = false;
	while (!Should_Run_Thread) { sceKernelUsleep(1000 * 10); }
}