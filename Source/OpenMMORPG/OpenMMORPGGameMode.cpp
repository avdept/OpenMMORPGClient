// Copyright Epic Games, Inc. All Rights Reserved.

#include "OpenMMORPGGameMode.h"
#include "OpenMMORPGCharacter.h"
#include "UObject/ConstructorHelpers.h"

AOpenMMORPGGameMode::AOpenMMORPGGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/BP_PlayableCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}


void AOpenMMORPGGameMode::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Display, TEXT("##################################      START GM_SERVER_WORLD    #######################################"));
	UE_LOG(LogTemp, Display, TEXT("GM_SERVER_WORLD: BeginPlay"));


	UE_LOG(LogTemp, Display, TEXT("##################################        END GM_SERVER_WORLD    #######################################"));
}

void AOpenMMORPGGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	UE_LOG(LogTemp, Display, TEXT("##################################      Client prelogin    #######################################"));
	FText::FromString(Address);
	UE_LOG(LogTemp, Display, TEXT("Client with address connected on: %s"), *FText::FromString(Address).ToString());
}