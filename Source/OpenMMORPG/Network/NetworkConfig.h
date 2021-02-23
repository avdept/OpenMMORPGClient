#pragma once

#include <string>

class NetworkConfig
{
public:

	static std::string address;

	static size_t tcp_local_port;

	static size_t web_port;

	static size_t tcp_server_port;

	static size_t udp_local_port;

	static size_t udp_server_port;

	static FString tcp_server_url;
	static FString WebServerUrl;

	static FString ServersListUrl;

	static FString CharacterServerUrl(int ID);
	static bool bEnableCrypt;

};
