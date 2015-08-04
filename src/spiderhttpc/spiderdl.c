/**
 * spiderdl.c by Lunarcookies
 * spiderhax example of how download files 
 */

#include "spiderhttpc.h"

int main();

void _memset(void* dst, int val, unsigned int size)
{
	char *destc = (char *) dst;
	for(unsigned int i = 0; i < size; i++)
	{
		destc[i] = (char) val;
	}
}

__attribute__ ((section (".text.start"), naked))
void entry()
{
	main();
}

Result inline downloadPageToSDCard(httpcContext* context, const short* filename, u32 size)
{
	Result ret = 0;
	u32 pos = 0, sz = 0;

	IFILE file;
	_memset(&file, 0, sizeof(file));
	unsigned int written;
	IFile_Open(&file, filename, FILE_W);
	svcSleepThread(0x400000LL);

	while(pos < size)
	{
		sz = size - pos;
		sz = sz > BUF_LEN ? BUF_LEN : sz;

		ret = httpcReceiveData(context, BUF_LOC, sz);

		if(ret == HTTPC_RESULTCODE_DOWNLOADPENDING)
		{
			ret = httpcGetDownloadSizeState(context, &pos, 0);
			if(ret)
				return ret;
			goto filewrite;
		}
		else if(ret)
			return ret;
		else
		{
			pos += sz;
			filewrite: IFile_Write(&file, &written, BUF_LOC, sz);
			svcSleepThread(0x400000LL);
		}
	}

	return 0;
}

Result downloadPage(Handle httpcHandle, const char* url, const short* filename)
{
	Result ret;
	httpcContext context;
	u32 statuscode, size;

	ret = httpcOpenContext(httpcHandle, &context, url);

	if(!ret)
	{
		ret = httpcBeginRequest(&context);
		if(ret)
			goto exit;
		ret = httpcGetResponseStatusCode(&context, &statuscode);
		if(ret || statuscode != 200)
			goto exit;
		ret = httpcGetDownloadSizeState(&context, 0, &size);
		if(ret)
			goto exit;
		ret = downloadPageToSDCard(&context, filename, size);

		exit: httpcCloseContext(httpcHandle, &context);
	}

	return ret;
}

Result downloadAndExecute(Handle httpcHandle, const char* url)
{
	Result ret;
	httpcContext context;
	u32 statuscode, size;

	ret = httpcOpenContext(httpcHandle, &context, url);

	if(!ret)
	{
		ret = httpcBeginRequest(&context);
		if(ret)
			goto exit;
		ret = httpcGetResponseStatusCode(&context, &statuscode);
		if(ret || statuscode != 200)
			goto exit;
		ret = httpcGetDownloadSizeState(&context, 0, &size);
		if(ret)
			goto exit;

		u32 pos = 0, sz = 0, tpos = 0;
		while(pos < size)
		{
			sz = size - pos;
			sz = sz > BUF_LEN ? BUF_LEN : sz;

			ret = httpcReceiveData(&context, BUF_LOC, sz);
			if(ret == HTTPC_RESULTCODE_DOWNLOADPENDING)
			{
				ret = httpcGetDownloadSizeState(&context, &tpos, 0);
				if(ret)
					return ret;

				sz = tpos - pos;
				pos = tpos;
				goto copy;
			}
			else if(ret)
				return ret;
			else
			{
			copy:
				for(int i = 0; i < 3; i++)
				{
					GSPGPU_FlushDataCache(BUF_LOC, sz);
					GX_SetTextureCopy(BUF_LOC, (char*) CODE_TARGET + 0x4000 + pos, sz, 0, 0, 0, 0, 8);
					svcSleepThread(0x400000LL);
				}

				pos += sz;
			}
		}

		((void(*)())CODE_ENTRY + 0x4000)();

		exit: httpcCloseContext(httpcHandle, &context);
	}

	return ret;
}
