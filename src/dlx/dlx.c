#include "spiderdl.h"

const char url[0x100] = "http://";

int main()
{
	Handle httpcHandle = 0;

	httpcInit(&httpcHandle);
	downloadAndExecute(httpcHandle, url);
	httpcExit(&httpcHandle);

	return 0;
}
