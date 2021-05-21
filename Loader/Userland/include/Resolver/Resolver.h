#pragma once

#include "Resolver-505.h"

/* Util */
extern struct sysentvec* sysvec;
extern struct prison* prison0;
extern struct vnode* rootvnode;

/* STD Lib */
void (*memcpy)(void *dst, const void *src, size_t len);
void *(*memset)(void * ptr, int value, size_t num);
int (*memcmp)(const void * ptr1, const void * ptr2, size_t num);
int (*sprintf)(char* dst, const char *fmt, ...);
int (*vsprintf)(char* dst, const char* fmt, va_list ap);
void(*kprintf)(const char* fmt, ...);

uint64_t (*kmem_alloc)(vm_map_t map, uint64_t size);
extern struct vm_map* kernel_map;

void Kern_Resolve(uint64_t kernbase);
void Userland_Resolve();