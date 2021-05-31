#pragma once
#include "Common.h"

class System_Monitor
{
public:
	struct Memory
	{
		int Used;
		int Free;
		int Total;
		float Percentage;
	};

	static int Thread_Count;
	static float Usage[8];
	static float Average_Usage;
	static int CPU_Temp;
	static int SOC_Temp;
	static Memory RAM;
	static Memory VRAM;

	static void Init();
	static void Term();

private:
	struct thread_usages
	{
		OrbisKernelTimespec current_time;	//0x00
		int Thread_Count;					//0x10
		char padding0[0x4];					//0x14
		Proc_Stats Threads[3072];			//0x18
	};

	static bool Should_Run_Thread;
	static Proc_Stats Stat_Data[3072];
	static thread_usages gThread_Data[2];

	static void calc_usage(unsigned int idle_tid[8], thread_usages* cur, thread_usages* prev, float usage_out[8]);
	static void* Monitor_Thread(void* args);

};
