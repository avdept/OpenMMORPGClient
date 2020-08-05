// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenMMORPG_GameInstance.h"
#include "OpenMMORPG/Network/SocketObject.h"
#include "OpenMMORPG/Network/NetworkConfig.h"

void UOpenMMORPG_GameInstance::Init()
{
    GLog->Log("UClientGameInstance::Init()");

    USocketObject::InitSocket(NetworkConfig::address.c_str(), NetworkConfig::tcp_local_port, NetworkConfig::tcp_server_port, NetworkConfig::udp_local_port, NetworkConfig::udp_server_port);

    //Init listening for UDP
    USocketObject::RunUDPSocketReceiver();
}