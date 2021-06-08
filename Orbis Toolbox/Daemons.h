#pragma once
bool Start_Daemon(char* TitleId);
bool Stop_Daemon(char* TitleId);
bool Is_Daemon_Running(char* TitleId);
bool Start_Stop_Daemon(char* TitleId, bool Restart = false);

void Add_Daemon(char* dent);
void Remove_Daemon(char* dent);