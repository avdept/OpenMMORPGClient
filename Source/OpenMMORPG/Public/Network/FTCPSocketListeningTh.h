// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "OpenMMORPG_GameInstance.h"
#include "Runtime/Core/Public/HAL/Runnable.h"
#include "Runtime/Core/Public/HAL/RunnableThread.h"

/**
 * 
 */
class OPENMMORPG_API FTCPSocketListeningTh : public FRunnable
{

	FRunnableThread *Thread;
	static FTCPSocketListeningTh *Runnable;
	static bool bIsThreadRunning;
public:
	FTCPSocketListeningTh();
	~FTCPSocketListeningTh();

	virtual bool Init() override;
	virtual uint32 Run() override;

	static FTCPSocketListeningTh* RunSocketListener();
	static void Shutdown();
};
