#include "spiderdl.h"

const char url[0x100] = "http://";
const wchar_t filename[0x100] = L"dmc:/";

int main()
{
	Handle httpcHandle = 0;

	httpcInit(&httpcHandle);
	downloadFile(httpcHandle, url, filename);
	httpcExit(&httpcHandle);

	return 0;
}
