#pragma once

struct TLS
{
	DWORD ThreadID;
	std::string ThreadName;

	TLS();
};

extern thread_local TLS g_tls;
