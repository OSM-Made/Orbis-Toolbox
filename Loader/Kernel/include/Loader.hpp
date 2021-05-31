#pragma once

struct OrbisLoader_header
{
    uint32_t magic;
    uint64_t entry;

    uint64_t thr_initial;
    uint8_t ShellCodeComplete;

    char SPRXPath[100];
    uint64_t ModuleHandle;
}__attribute__((packed));

bool Load_SPRX(proc* proc, const char* File);
