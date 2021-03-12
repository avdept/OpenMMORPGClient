// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenMMORPG_GameInstance.h"

#include "WorldStaticManager.h"
#include "Network/WorldServerEntity.h"
#include "Network/WorldServerManager.h"
#include "OpenMMORPG/Network/HTTPRequestManager.h"
#include "OpenMMORPG/Network/SocketObject.h"
#include "OpenMMORPG/Network/NetworkConfig.h"
#include "OpenMMORPG/BackgroundWorkers//ServerStatusCheckingTh.h"
#include "OpenMMORPG/Public/Network/FTCPSocketListeningTh.h"

void UOpenMMORPG_GameInstance::Init()
{
    GLog->Log("UClientGameInstance::Init()");

    SetCommandLineArgs();
    WorldStaticManager::GameInstance = this;

    USocketObject::InitSocket(NetworkConfig::address.c_str(), NetworkConfig::tcp_local_port, NetworkConfig::tcp_server_port, NetworkConfig::udp_local_port, NetworkConfig::udp_server_port);

    //Init listening for UDP
    USocketObject::RunUDPSocketReceiver();

 
    FServerStatusCheckingTh::RunServerChecking();

    
    //FTCPSocketListeningTh::RunSocketListener();
}

void UOpenMMORPG_GameInstance::Shutdown()
{
    USocketObject::DisconnectFromWorldServer();
    FServerStatusCheckingTh::Shutdown();
   // FTCPSocketListeningTh::Shutdown();
    // We want to wait a little to make sure all connections are closed. This is kinda of a hack, since its better to implement thread safe singleton
    //while (FServerStatusCheckingTh::IsThreadRunning() || FTCPSocketListeningTh::IsThreadRunning())
    //{
     //   FPlatformProcess::Sleep(0.5f); 
    //}
    USocketObject::Shutdown();
    WorldStaticManager::GameInstance = nullptr;
}

void UOpenMMORPG_GameInstance::SetCommandLineArgs()
{
    if (!FParse::Value(FCommandLine::Get(), TEXT("token="), AuthToken))
    {
    #if WITH_EDITOR
        // Purely for play in editor purpose, since PIE  does not allow to pass command line args.
        AuthToken = FString("eyJhbGciOiJIUzUxMiIsInR5cCI6IkpXVCJ9.eyJhdWQiOiJvcGVubW1vcnBnX2xvZ2luc2VydmVyIiwiZXhwIjoxNjE3ODI0NzU5LCJpYXQiOjE2MTU0MDU1NTksImlzcyI6Im9wZW5tbW9ycGdfbG9naW5zZXJ2ZXIiLCJqdGkiOiJjZWU5NjE4Zi00YzliLTQyM2MtYjQzOS04YTE2ZGQ2OTIzZGMiLCJuYmYiOjE2MTU0MDU1NTgsInN1YiI6IjEiLCJ0eXAiOiJhY2Nlc3MifQ.NT_e6rwPGbsbIgeaCVD3FFcgMCSPPMEvzpJ51JH2Uv4hZ7O_Wt2ODRdKFWzZExiqUXofD7xwVD7SW_6Fg091_w");
    #endif

    }
}

