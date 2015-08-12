/**
 * spiderhttpc.c by Lunarcookies
 * Partial implementation of http:C service
 * for using it with spiderhax
 * mostly stolen from ctrulib
 * https://github.com/smealum/ctrulib
 */

#include "spiderhttpc.h"

int strlen(const char* str)
{
	int l = 0;
	while(*(str++))
		l++;
	return l;
}

int NAKED svcSendSyncRequest(Handle h)
{
	__asm__ volatile ("svc 0x32");
	__asm__ volatile ("bx lr");
}

int NAKED svcCloseHandle(Handle h)
{
	__asm__ volatile ("svc 0x23");
	__asm__ volatile ("bx lr");
}

Result httpcInit(Handle* __httpc_servhandle)
{
	Result ret=0;
	if(__httpc_servhandle==0)return 0;
	if(*__httpc_servhandle)return 0;
	if((ret=srvGetServiceHandle(__httpc_servhandle, "http:C", 6, 0)))return ret;

	ret = HTTPC_Initialize(*__httpc_servhandle);
	if(ret!=0)return ret;

	return 0;
}

void httpcExit(Handle* __httpc_servhandle)
{
	if(__httpc_servhandle==0)return;
	if(*__httpc_servhandle==0)return;

	svcCloseHandle(*__httpc_servhandle);
}

Result httpcOpenContext(Handle __httpc_servhandle, httpcContext *context, const char* url)
{
	Result ret = 0;

	ret = HTTPC_CreateContext(__httpc_servhandle, url, &context->httphandle);
	if(ret != 0)
		return ret;

	ret = srvGetServiceHandle(&context->servhandle, "http:C", 6, 0);
	if(ret != 0)
	{
		HTTPC_CloseContext(__httpc_servhandle, context->httphandle);
		return ret;
	}

	ret = HTTPC_InitializeConnectionSession(context->servhandle, context->httphandle);
	if(ret != 0)
	{
		svcCloseHandle(context->servhandle);
		HTTPC_CloseContext(__httpc_servhandle, context->httphandle);
		return ret;
	}

	return 0;
}

Result httpcCloseContext(Handle __httpc_servhandle, httpcContext *context)
{
	Result ret=0;

	svcCloseHandle(context->servhandle);
	ret = HTTPC_CloseContext(__httpc_servhandle, context->httphandle);

	return ret;
}

Result HTTPC_Initialize(Handle handle)
{
	u32* cmdbuf=getThreadCommandBuffer();

	cmdbuf[0]=0x10044; //request header code
	cmdbuf[1]=0x1000; //unk
	cmdbuf[2]=0x20;//processID header, following word is set to processID by the arm11kernel.
	cmdbuf[4]=0;
	cmdbuf[5]=0;//Some sort of handle.

	Result ret=0;
	if((ret=svcSendSyncRequest(handle)))return ret;

	return cmdbuf[1];
}

Result HTTPC_CreateContext(Handle handle, const char* url, Handle* contextHandle)
{
	u32* cmdbuf=getThreadCommandBuffer();
	u32 l=strlen(url)+1;

	cmdbuf[0]=0x20082; //request header code
	cmdbuf[1]=l;
	cmdbuf[2]=0x01; // GET
	cmdbuf[3]=(l<<4)|0xA;
	cmdbuf[4]=(u32)url;

	Result ret=0;
	if((ret=svcSendSyncRequest(handle)))return ret;

	if(contextHandle)*contextHandle=cmdbuf[2];

	return cmdbuf[1];
}

Result HTTPC_InitializeConnectionSession(Handle handle, Handle contextHandle)
{
	u32* cmdbuf=getThreadCommandBuffer();

	cmdbuf[0]=0x80042; //request header code
	cmdbuf[1]=contextHandle;
	cmdbuf[2]=0x20; //unk, constant afaict

	Result ret=0;
	if((ret=svcSendSyncRequest(handle)))return ret;

	return cmdbuf[1];
}

Result HTTPC_CloseContext(Handle handle, Handle contextHandle)
{
	u32* cmdbuf=getThreadCommandBuffer();

	cmdbuf[0]=0x30040; //request header code
	cmdbuf[1]=contextHandle;

	Result ret=0;
	if((ret=svcSendSyncRequest(handle)))return ret;

	return cmdbuf[1];
}

Result HTTPC_BeginRequest(Handle handle, Handle contextHandle)
{
	u32* cmdbuf=getThreadCommandBuffer();

	cmdbuf[0]=0x90040; //request header code
	cmdbuf[1]=contextHandle;

	Result ret=0;
	if((ret=svcSendSyncRequest(handle)))return ret;

	return cmdbuf[1];
}

Result HTTPC_ReceiveData(Handle handle, Handle contextHandle, u8* buffer, u32 size)
{
	u32* cmdbuf=getThreadCommandBuffer();

	cmdbuf[0]=0xB0082; //request header code
	cmdbuf[1]=contextHandle;
	cmdbuf[2]=size;
	cmdbuf[3]=(size<<4)|12;
	cmdbuf[4]=(u32)buffer;

	Result ret=0;
	if((ret=svcSendSyncRequest(handle)))return ret;

	return cmdbuf[1];
}

Result HTTPC_GetDownloadSizeState(Handle handle, Handle contextHandle, u32* downloadedsize, u32* contentsize)
{
	u32* cmdbuf=getThreadCommandBuffer();

	cmdbuf[0]=0x60040; //request header code
	cmdbuf[1]=contextHandle;

	Result ret=0;
	if((ret=svcSendSyncRequest(handle)))return ret;

	if(downloadedsize)*downloadedsize = cmdbuf[2];
	if(contentsize)*contentsize = cmdbuf[3];

	return cmdbuf[1];
}

Result HTTPC_GetResponseStatusCode(Handle handle, Handle contextHandle, u32* out)
{
	u32* cmdbuf=getThreadCommandBuffer();

	cmdbuf[0]=0x220040; //request header code
	cmdbuf[1]=contextHandle;

	Result ret=0;
	if((ret=svcSendSyncRequest(handle)))return ret;

	*out = cmdbuf[2];

	return cmdbuf[1];
}
