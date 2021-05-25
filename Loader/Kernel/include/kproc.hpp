#pragma once

//Event Handlers
extern eventhandler_entry* ProcessStartEvent;
extern eventhandler_entry* ProcessExitEvent;

void kproc_Init();
void kproc_Term();
