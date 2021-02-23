// Created by Alex Sinelnikov - sinelniko@gmail.com


#include "Levels/L_BaseLevel.h"

#include "OpenMMORPG/OpenMMORPGCharacter.h"

void AL_BaseLevel::BeginPlay()
{
	Super::BeginPlay();

	GLog->Log("Before spawning FP Actor");
	#if !WITH_SERVER_CODE
	GLog->Log("Spawning FP Actor");
	
	#endif



}

void AL_BaseLevel::SpawnPlayer()
{
	auto ctrl = GetWorld()->GetFirstPlayerController();
	auto actor = GetWorld()->SpawnActor<AOpenMMORPGCharacter>();
	ctrl->Possess(actor);
}
