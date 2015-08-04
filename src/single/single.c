#include "spiderdl.h"

int main()
{
	Handle httpcHandle = 0;

	char * url = "http://slashdot.org";
	unsigned short * filename = L"dmc:/file.html";

	httpcInit(&httpcHandle);

	downloadPage(httpcHandle, url, (const short *) filename);

	httpcExit(&httpcHandle);

	return 0;
}
