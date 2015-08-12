#pragma once

#include "types.h"

/**
 * common.h by Lunarcookies
 * Partial implementation of http:C service
 * for using it with spiderhax
 * mostly stolen from ctrulib
 * https://github.com/smealum/ctrulib
 */

typedef struct
{
	s32 s;
	u32 pos;
	u32 size;
	u32 unk[5];
} IFILE;

void _memset(void* dst, int val, unsigned int size);

extern int (*IFile_Open)(void *, const wchar_t*, int);
extern int (*IFile_Read)(void *, unsigned int *, void *, unsigned int);
extern int (*IFile_Close)(void*);
extern int (*IFile_Write)(void *, unsigned int *, void *, unsigned int);
extern int (*GX_SetTextureCopy)(void *, void *, unsigned int, int, int, int, int, int);
extern int (*GSPGPU_FlushDataCache)(void *, unsigned int);
extern int (*svcSleepThread)(unsigned long long);
extern int (*srvGetServiceHandle)(Handle *, char *, unsigned int, unsigned int);
