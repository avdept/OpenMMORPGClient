// Copyright Epic Games, Inc. All Rights Reserved.

#include "OpenMMORPGGameMode.h"
#include "OpenMMORPGCharacter.h"
#include "OpenMMORPGPlayerController.h"
#include "OpenMMORPG_GameInstance.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "Network/SocketObject.h"
#include "Network/Proto/MessageModels.pb.h"
#include "Services/UserService.h"

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

	//#if WITH_SERVER_CODE

		UE_LOG(LogTemp, Display, TEXT("##################################      START GM_SERVER_WORLD    #######################################"));
		UE_LOG(LogTemp, Display, TEXT("GM_SERVER_WORLD: BeginPlay"));


		UE_LOG(LogTemp, Display, TEXT("##################################        END GM_SERVER_WORLD    #######################################"));
	//#endif
	TestUDPMessage();
}

void AOpenMMORPGGameMode::TestUDPMessage()
{
	GLog->Log("send ->>>");

	std::shared_ptr<Utility> utility(new Utility);
	utility->set_alive(true);

	USocketObject::SendByUDP(utility.get());
}

void AOpenMMORPGGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	UE_LOG(LogTemp, Display, TEXT("##################################      Client prelogin    #######################################"));

	
	UE_LOG(LogTemp, Display, TEXT("Client with address connected on: %s"), *FText::FromString(Address).ToString());

	UE_LOG(LogTemp, Display, TEXT("Connect params:: %s"), *FText::FromString(Options).ToString());
	

}

APlayerController* AOpenMMORPGGameMode::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal,
	const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	auto Ctrl = Cast<AOpenMMORPGPlayerController>(Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage));
	
	auto PlayerService = new UserService(UGameplayStatics::ParseOption(Options, "Token"), UGameplayStatics::ParseOption(Options, "PlayerID"));
	proto_messages::Player* Player= PlayerService->GetCharacterProfile();

	if (Player == nullptr)
	{
		// TODO: Handle this on UI
		ErrorMessage = "Could not login to server";
	}

	Ctrl->PlayerInfo = Player;
	
	return Ctrl;
}

AActor* AOpenMMORPGGameMode::FindPlayerStart_Implementation(AController* Player, const FString& IncomingName)
{
	auto pawn = Cast<AOpenMMORPGCharacter>(Player->GetPawn());
	if (pawn)
	{
		auto inst = Cast<UOpenMMORPG_GameInstance>(pawn->GetGameInstance());
		GLog->Log("Pawn found");
		
		if (inst)
		{
			GLog->Log(inst->CurrentCharacterId);
			GLog->Log("inst found");
		}
	}
	GLog->Log("Hello from implementation");

	return GetWorld()->SpawnActor<APlayerStart>(FVector(42000, 36590, 300), FRotator());
	
}


//void AOpenMMORPGGameMode::PostLogin(APlayerController* NewPlayer)
//{
	/*FVector CurLocation;
	FRotator Rotator;
	FActorSpawnParameters SpawnInfo;


	if (AActor::GetWorld() != nullptr)
	{
		auto inst = Cast<UOpenMMORPG_GameInstance>(GetGameInstance());
		if (inst != nullptr)
		{
			auto loc = inst->Player.location();
			CurLocation.X = loc.x();
			CurLocation.Y = loc.y();
			CurLocation.Z = loc.z();

			auto rot = inst->Player.rotation();
			Rotator.Yaw = rot.yaw();
		}
		
	}

	const auto Player = GetWorld()->SpawnActor<AOpenMMORPGCharacter>(AOpenMMORPGCharacter::StaticClass(), CurLocation, Rotator, SpawnInfo);
	NewPlayer->Possess(Player);*/
//}
