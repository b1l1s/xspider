#pragma once
/**
 * spiderdl.h by Lunarcookies
 * Partial implementation of http:C service
 * for using it with spiderhax
 * mostly stolen from ctrulib
 * https://github.com/smealum/ctrulib
 */

#include "types.h"

Result httpcInit(Handle* __httpc_servhandle);
void httpcExit(Handle* __httpc_servhandle);

Result downloadPage(Handle httpcHandle, const char* url, const short* filename);
Result downloadAndExecute(Handle httpcHandle, const char* url);
