#include "spiderdl.h"

int main()
{
	Handle httpcHandle = 0;

	const char* url = "http://slashdot.org";
	const short* filename = L"dmc:/file.html";

	httpcInit(&httpcHandle);

	downloadPage(httpcHandle, url, filename);

	httpcExit(&httpcHandle);

	return 0;
}
