#pragma once

extern thread* kproc_Main_Thread;

//Event Handlers
extern eventhandler_entry* ProcessStartEvent;
extern eventhandler_entry* ProcessExitEvent;

void kproc_Init();
void kproc_Term();
