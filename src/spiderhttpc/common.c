#include "common.h"
#include "rop3ds/rop.h"

int (*IFile_Open)(void *, const wchar_t*, int) = (void*)FOPEN_LOC;
int (*IFile_Close)(void*) = (void*)FCLOSE_LOC;
int (*IFile_Read)(void *, unsigned int *, void *, unsigned int) = (void*)FREAD_LOC;
int (*IFile_Write)(void *, unsigned int *, void *, unsigned int) = (void*)FWRITE_LOC;
int (*GX_SetTextureCopy)(void *, void *, unsigned int, int, int, int, int, int) = (void*)GX_SETTEXTURECOPY_LOC;
int (*GSPGPU_FlushDataCache)(void *, unsigned int) = (void*)GSGPU_FLUSHDATACACHE_LOC;
int (*svcSleepThread)(unsigned long long) = (void*)SVCSLEEPTHREAD_LOC;
int (*srvGetServiceHandle)(Handle *, char *, unsigned int, unsigned int) = (void*)SRVGETSERVICEHANDLE_LOC;

void _memset(void* dst, int val, unsigned int size)
{
	char *destc = (char *) dst;
	for(unsigned int i = 0; i < size; i++)
	{
		destc[i] = (char) val;
	}
}
