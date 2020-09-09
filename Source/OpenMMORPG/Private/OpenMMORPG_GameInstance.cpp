// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenMMORPG_GameInstance.h"
#include "OpenMMORPG/Network/SocketObject.h"
#include "OpenMMORPG/Network/NetworkConfig.h"
#include "OpenMMORPG/Network/ServerStatusCheckingTh.h"
#include "OpenMMORPG/Public/Network/FTCPSocketListeningTh.h"

void UOpenMMORPG_GameInstance::Init()
{
    GLog->Log("UClientGameInstance::Init()");

    USocketObject::InitSocket(NetworkConfig::address.c_str(), NetworkConfig::tcp_local_port, NetworkConfig::tcp_server_port, NetworkConfig::udp_local_port, NetworkConfig::udp_server_port);

    //Init listening for UDP
    USocketObject::RunUDPSocketReceiver();

    FServerStatusCheckingTh::RunServerChecking();
    FTCPSocketListeningTh::RunSocketListener();
}

void UOpenMMORPG_GameInstance::Shutdown()
{
    FServerStatusCheckingTh::Shutdown();
    FTCPSocketListeningTh::Shutdown();
    // We want to wait a little to make sure all connections are closed. This is kinda of a hack, since its better to implement thread safe singleton
    while (FServerStatusCheckingTh::IsThreadRunning() || FTCPSocketListeningTh::IsThreadRunning())
    {
        FPlatformProcess::Sleep(0.5f); 
    }
    USocketObject::Shutdown();
}
