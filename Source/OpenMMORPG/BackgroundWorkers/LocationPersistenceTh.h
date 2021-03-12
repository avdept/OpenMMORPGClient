#pragma once

#include "OpenMMORPG/OpenMMORPGCharacter.h"
#include "Runtime/Core/Public/HAL/Runnable.h"
class AOpenMMORPGCharacter;

class OPENMMORPG_API FLocationPersistenceTh : public FRunnable
{
protected:
	FRunnableThread* Thread;
	AOpenMMORPGCharacter * CurrentCharacter;
	bool bIsThreadRunning;
public:
	
	FLocationPersistenceTh();
	~FLocationPersistenceTh();

	virtual bool Init() override;
	virtual uint32 Run() override;

	static FLocationPersistenceTh* RunLocationPersisting(AOpenMMORPGCharacter*);

	void Shutdown();
};
