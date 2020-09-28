// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenMMORPG_GameInstance.h"

#include "WorldStaticManager.h"
#include "Network/WorldServerEntity.h"
#include "Network/WorldServerManager.h"
#include "OpenMMORPG/Network/HTTPRequestManager.h"
#include "OpenMMORPG/Network/SocketObject.h"
#include "OpenMMORPG/Network/NetworkConfig.h"
#include "OpenMMORPG/Network/ServerStatusCheckingTh.h"
#include "OpenMMORPG/Public/Network/FTCPSocketListeningTh.h"

void UOpenMMORPG_GameInstance::Init()
{
    GLog->Log("UClientGameInstance::Init()");

    SetCommandLineArgs();
    WorldStaticManager::GameInstance = this;

    USocketObject::InitSocket(NetworkConfig::address.c_str(), NetworkConfig::tcp_local_port, NetworkConfig::tcp_server_port, NetworkConfig::udp_local_port, NetworkConfig::udp_server_port);

    UHTTPRequestManager* HttpClientInstance = UHTTPRequestManager::Create();
    HttpClientInstance->Request_OnFinished.AddDynamic(this, &UOpenMMORPG_GameInstance::OnServerListFetched);
    HttpClientInstance->GetRequest(FString("http://10.0.1.7:4000/api/servers"));
    
    //Init listening for UDP
    USocketObject::RunUDPSocketReceiver();

    //FServerStatusCheckingTh::RunServerChecking();
    //FTCPSocketListeningTh::RunSocketListener();
}

void UOpenMMORPG_GameInstance::Shutdown()
{
    //FServerStatusCheckingTh::Shutdown();
   // FTCPSocketListeningTh::Shutdown();
    // We want to wait a little to make sure all connections are closed. This is kinda of a hack, since its better to implement thread safe singleton
    //while (FServerStatusCheckingTh::IsThreadRunning() || FTCPSocketListeningTh::IsThreadRunning())
    //{
     //   FPlatformProcess::Sleep(0.5f); 
    //}
    USocketObject::Shutdown();
    WorldStaticManager::GameInstance = nullptr;
}

void UOpenMMORPG_GameInstance::OnServerListFetched(bool bSuccess, UHTTPRequestManager* RequestManager,
    ERequestResult status)
{
    UWorldServerManager::ServerList = UWorldServerEntity::FromJSON(RequestManager->Data);
    auto Request = bSuccess;
    GLog->Log("Server list fetched");
}

void UOpenMMORPG_GameInstance::SetCommandLineArgs()
{
    if (!FParse::Value(FCommandLine::Get(), TEXT("token="), AuthToken))
    {
    #if WITH_EDITOR
        // Purely for play in editor purpose, since PIE  does not allow to pass command line args.
        AuthToken = FString("aaabbbccc111222333");
    #endif

    }
}

