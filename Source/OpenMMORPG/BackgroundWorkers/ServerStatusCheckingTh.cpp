
#include "ServerStatusCheckingTh.h"

#include "OpenMMORPG_GameInstance.h"
#include "../Network/SocketObject.h"
#include "Network/NetworkSocketManager.h"

FServerStatusCheckingTh * FServerStatusCheckingTh::Runnable = nullptr;
bool FServerStatusCheckingTh::bIsThreadRunning = false;

FServerStatusCheckingTh::FServerStatusCheckingTh()
{
    Thread = FRunnableThread::Create(this, TEXT("ServerStatusChecking"), 0, TPri_BelowNormal);
}

FServerStatusCheckingTh::~FServerStatusCheckingTh()
{
    delete Thread;
    Thread = nullptr;
}

bool FServerStatusCheckingTh::Init()
{
    bIsThreadRunning = true;
    return true;
}

uint32 FServerStatusCheckingTh::Run()
{
    while (bIsThreadRunning)
    {
        FPlatformProcess::Sleep(1.f);

        
        if (!USocketObject::bIsConnected)
        {
            USocketObject::Reconnect();
        } else
        {
            USocketObject::bIsConnected = USocketObject::Alive();
        }

        //GLog->Log("Connect state (bIsConnected) = " + FString::FromInt(static_cast<int32>(USocketObject::bIsConnected)) + " | FServerStatusCheckingTh::CheckServer");
    }

   
    return 0;
}

FServerStatusCheckingTh* FServerStatusCheckingTh::RunServerChecking()
{
    if (!Runnable && FPlatformProcess::SupportsMultithreading())
    {
        Runnable = new FServerStatusCheckingTh();
    }
    return Runnable;
}

void FServerStatusCheckingTh::Shutdown()
{
    bIsThreadRunning = false;
    GLog->Log("FServerStatusCheckingTh::Shutdown()");

    if (Runnable)
    {
        delete Runnable;
        Runnable = nullptr;
    }
}





