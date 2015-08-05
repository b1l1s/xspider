#include "spiderdl.h"

int main()
{
	Handle httpcHandle = 0;

	const char* url = "http://someurl/downfiles.bin";

	httpcInit(&httpcHandle);

	downloadAndExecute(httpcHandle, url);

	httpcExit(&httpcHandle);

	return 0;
}
