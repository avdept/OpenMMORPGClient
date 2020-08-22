#pragma once

#include "Runtime/Core/Public/HAL/Runnable.h"
#include "Runtime/Core/Public/HAL/RunnableThread.h"

class OPENMMORPG_API FServerStatusCheckingTh : public FRunnable
{

    static FServerStatusCheckingTh* Runnable;

    FRunnableThread* Thread;
    static bool bIsThreadRunning;
public:
    FServerStatusCheckingTh();
    ~FServerStatusCheckingTh();

    virtual bool Init() override;
    virtual uint32 Run() override;


    static FServerStatusCheckingTh* RunServerChecking();

    static void Shutdown();
};
