#pragma once

/**
 * spiderdl.h by Lunarcookies
 * Partial implementation of http:C service 
 * for using it with spiderhax
 * mostly stolen from ctrulib
 * https://github.com/smealum/ctrulib
 */

#include "types.h"
#include "rop3ds/rop.h"

#define HTTPC_RESULTCODE_DOWNLOADPENDING 0xd840a02b

#define NAKED __attribute__ ((naked))
#define NORETURN __attribute__ ((noreturn))

#define BUF_LOC     ((void*)0x18410000)
#define BUF_LEN			 ((u32)0x10000)

#define FILE_R (0x1)
#define FILE_W (0x6)

typedef struct
{
	s32 s;
	u32 pos;
	u32 size;
	u32 unk[5];
} IFILE;

typedef struct {
	Handle servhandle;
	u32 httphandle;
} httpcContext;

static inline void* getThreadLocalStorage(void) {
    void* ret;
    __asm__ volatile ("mrc p15, 0, %[data], c13, c0, 3" : [data] "=r" (ret));
    return ret;
}

static inline u32* getThreadCommandBuffer(void) {
    return (u32*)((u8*)getThreadLocalStorage() + 0x80);
}

extern int (*IFile_Open)(void *, const short *, int);
extern int (*IFile_Read)(void *, unsigned int *, unsigned int *, unsigned int);
extern int (*IFile_Write)(void *, unsigned int *, void *, unsigned int);
extern int (*GX_SetTextureCopy)(void *, void *, unsigned int, int, int, int, int, int);
extern int (*GSPGPU_FlushDataCache)(void *, unsigned int);
extern int (*svcSleepThread)(unsigned long long);
extern int (*srvGetServiceHandle)(Handle *, char *, unsigned int, unsigned int);

int strlen(char* str);

Result HTTPC_Initialize(Handle handle);
Result HTTPC_CreateContext(Handle handle, char* url, Handle* contextHandle);
Result HTTPC_InitializeConnectionSession(Handle handle, Handle contextHandle);
Result HTTPC_CloseContext(Handle handle, Handle contextHandle);
Result HTTPC_BeginRequest(Handle handle, Handle contextHandle);
Result HTTPC_ReceiveData(Handle handle, Handle contextHandle, u8* buffer, u32 size);
Result HTTPC_GetDownloadSizeState(Handle handle, Handle contextHandle, u32* downloadedsize, u32* contentsize);
Result HTTPC_GetResponseStatusCode(Handle handle, Handle contextHandle, u32* out);

Result httpcInit(Handle* __httpc_servhandle);
void httpcExit(Handle* __httpc_servhandle);

Result httpcOpenContext(Handle __httpc_servhandle, httpcContext *context, char* url);
Result httpcCloseContext(Handle __httpc_servhandle, httpcContext *context);

Result inline httpcBeginRequest(httpcContext *context)
{
	return HTTPC_BeginRequest(context->servhandle, context->httphandle);
}

Result inline httpcReceiveData(httpcContext *context, u8* buffer, u32 size)
{
	return HTTPC_ReceiveData(context->servhandle, context->httphandle, buffer, size);
}

Result inline httpcGetDownloadSizeState(httpcContext *context, u32* downloadedsize, u32* contentsize)
{
	return HTTPC_GetDownloadSizeState(context->servhandle, context->httphandle, downloadedsize, contentsize);
}

Result inline httpcGetResponseStatusCode(httpcContext *context, u32* out)
{
		return HTTPC_GetResponseStatusCode(context->servhandle, context->httphandle, out);
}
