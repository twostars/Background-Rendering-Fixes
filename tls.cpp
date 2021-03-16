#include "stdafx.h"
#include "tls.h"

thread_local TLS g_tls;

TLS::TLS()
{
	ThreadID = GetCurrentThreadId();
}
