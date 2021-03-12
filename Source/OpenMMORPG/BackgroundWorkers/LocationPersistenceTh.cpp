#include "LocationPersistenceTh.h"
#include "OpenMMORPG_GameInstance.h"
#include "../Network/SocketObject.h"
#include "../Services/PlayerLocationService.h"
#include "Network/NetworkSocketManager.h"

FLocationPersistenceTh::FLocationPersistenceTh()
{
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

		auto msg = FString("Running location persistence thread for: ") + FString(this->CurrentCharacter->CharacterName.ToString());
		GLog->Log(msg);
		if (CurrentCharacter != nullptr)
		{
			auto service = new PlayerLocationService(CurrentCharacter);
			service->SendNewLocation();
			delete service;
		}
	}
	return 0;
}

FLocationPersistenceTh* FLocationPersistenceTh::RunLocationPersisting(AOpenMMORPGCharacter* Character)
{
	if (FPlatformProcess::SupportsMultithreading())
	{
		auto Th  = new FLocationPersistenceTh();
		Th->CurrentCharacter = Character;
		return Th;
	}
	return nullptr;
}

void FLocationPersistenceTh::Shutdown()
{
	this->bIsThreadRunning = false;
	GLog->Log("FLocationPersistenceTh::Shutdown()");

	if (Thread)
	{
		Thread->Kill();
		delete Thread;
		Thread = nullptr;
	}
}





