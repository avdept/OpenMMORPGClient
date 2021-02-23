#include "LocationPersistenceTh.h"


#include "OpenMMORPG_GameInstance.h"
#include "../Network/SocketObject.h"
#include "../Services/PlayerLocationService.h"
#include "Network/NetworkSocketManager.h"

FLocationPersistenceTh * FLocationPersistenceTh::Runnable = nullptr;
bool FLocationPersistenceTh::bIsThreadRunning = false;

FLocationPersistenceTh::FLocationPersistenceTh(AOpenMMORPGCharacter* Character)
{
	CurrentCharacter = Character;
	Thread = FRunnableThread::Create(this, TEXT("LocationPersistanceThread"), 0, TPri_Normal);
}

FLocationPersistenceTh::~FLocationPersistenceTh()
{
	delete Thread;
	Thread = nullptr;
}

bool FLocationPersistenceTh::Init()
{
	bIsThreadRunning = true;
	return true;
}

uint32 FLocationPersistenceTh::Run()
{
	while (bIsThreadRunning)
	{
		// We send new location every 1 second 
		FPlatformProcess::Sleep(1.f);

		GLog->Log("Running location persistence th");
		if (CurrentCharacter != nullptr)
		{
			GLog->Log("Sending new location");
			auto service = new PlayerLocationService(CurrentCharacter);
			service->SendNewLocation();
			delete service;
		}

		//GLog->Log("Connect state (bIsConnected) = " + FString::FromInt(static_cast<int32>(USocketObject::bIsConnected)) + " | FServerStatusCheckingTh::CheckServer");
	}

   
	return 0;
}

FLocationPersistenceTh* FLocationPersistenceTh::RunLocationPersisting(AOpenMMORPGCharacter* Character)
{
	if (!Runnable && FPlatformProcess::SupportsMultithreading())
	{
		Runnable = new FLocationPersistenceTh(Character);
	}
	return Runnable;
}

void FLocationPersistenceTh::Shutdown()
{
	bIsThreadRunning = false;
	GLog->Log("FLocationPersistenceTh::Shutdown()");

	if (Runnable)
	{
		delete Runnable;
		Runnable = nullptr;
	}
}





