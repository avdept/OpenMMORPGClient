#pragma once

#include "OpenMMORPG/OpenMMORPGCharacter.h"
#include "Runtime/Core/Public/HAL/Runnable.h"


class OPENMMORPG_API FLocationPersistenceTh : public FRunnable
{

	static FLocationPersistenceTh* Runnable;

	FRunnableThread* Thread;
	AOpenMMORPGCharacter* CurrentCharacter;
	static bool bIsThreadRunning;
	public:
	FLocationPersistenceTh(AOpenMMORPGCharacter*);
	~FLocationPersistenceTh();

	virtual bool Init() override;
	virtual uint32 Run() override;

	static bool IsThreadRunning() { return bIsThreadRunning; };

	static FLocationPersistenceTh* RunLocationPersisting(AOpenMMORPGCharacter*);

	static void Shutdown();
};
