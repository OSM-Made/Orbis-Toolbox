#pragma once

#include "Resolver-505.h"
#include "Resolver-672.h"
#include "Resolver-702.h"
#include "Resolver-755.h"
#include "Resolver-900.h"

/* Util */
extern struct sysentvec* ksysvec;
extern struct prison* kprison0;
extern struct vnode* krootvnode;

/* STD Lib */
void (*kmemcpy)(void *dst, const void *src, size_t len);
void *(*kmemset)(void * ptr, int value, size_t num);
int (*kmemcmp)(const void * ptr1, const void * ptr2, size_t num);
int (*ksprintf)(char* dst, const char *fmt, ...);
int (*kvsprintf)(char* dst, const char* fmt, va_list ap);
void(*kprintf)(const char* fmt, ...);

void Kern_Resolve();

//Kernel
int(*sceKernelDebugOutText)(int dbg_channel, const char* text);
int(*sceKernelMkdir)(const char *path, mode_t mode);
int(*sceKernelOpen)(const char* path, int flags, mode_t mode);
int(*sceKernelWrite)(int fd, void *data, size_t len);
int(*sceKernelClose)(int fd);

//libc
void *(*malloc)(size_t size);
void(*free)(void *ptr);
void *(*calloc)(size_t num, size_t size);
void *(*realloc)(void* ptr, size_t size);
void *(*memset)(void *destination, int value, size_t num);
void *(*memcpy)(void *destination, const void *source, size_t num);
int(*memcmp)(const void *s1, const void *s2, size_t n);
char *(*strcpy)(char *destination, const char *source);
char *(*strncpy)(char *destination, const char *source, size_t num);
char *(*strcat)(char *dest, const char *src);
char *(*strncat)(char *dest, const char *src, size_t n);
size_t(*strlen)(const char *s);
int(*strcmp)(const char *s1, const char *s2);
int(*strncmp)(const char *s1, const char *s2, size_t n);
int(*sprintf)(char *str, const char *format, ...);
int(*snprintf)(char *str, size_t size, const char *format, ...);
int(*vsprintf)(char * s, const char * format, va_list arg);
int(*sscanf)(const char *str, const char *format, ...);
char *(*strchr)(const char *s, int c);
char *(*strrchr)(const char *s, int c);
char *(*strstr)(char *str1, char *str2);
int(*atoi)(const char * str);
double(*atof)(const char * str);
double(*sin)(double x);
double(*cos)(double x);
double(*atan2)(double x, double y);
double(*sqrt)(double vec);
char *(*strdup)(const char *s);

void Userland_Resolve();