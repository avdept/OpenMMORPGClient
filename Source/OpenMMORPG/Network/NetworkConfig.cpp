#include "NetworkConfig.h"

bool NetworkConfig::bEnableCrypt = true;

std::string NetworkConfig::address = "10.0.1.7";

size_t NetworkConfig::tcp_local_port = 7678;

size_t NetworkConfig::tcp_server_port = 1057;

size_t NetworkConfig::udp_local_port = 7679;

size_t NetworkConfig::udp_server_port = 7681;



FString NetworkConfig::tcp_server_url = FString::Format(TEXT("{0}:{1}"), TArray<FStringFormatArg> {FString(address.c_str()), FString(std::to_string(tcp_server_port).c_str())});