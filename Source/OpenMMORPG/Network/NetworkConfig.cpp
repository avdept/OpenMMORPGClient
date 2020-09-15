#include "NetworkConfig.h"

bool NetworkConfig::bEnableCrypt = true;

std::string NetworkConfig::address = "10.0.1.6";

size_t NetworkConfig::tcp_local_port = 7678;

size_t NetworkConfig::tcp_server_port = 1057;

size_t NetworkConfig::udp_local_port = 7679;

size_t NetworkConfig::udp_server_port = 7681;