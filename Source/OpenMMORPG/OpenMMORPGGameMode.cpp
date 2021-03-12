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
	DefaultPawnClass = PlayerPawnBPClass.Class;
	PlayerControllerClass = AOpenMMORPGPlayerController::StaticClass();
}


void AOpenMMORPGGameMode::BeginPlay()
{
	Super::BeginPlay();

	#if WITH_SERVER_CODE

		UE_LOG(LogTemp, Display, TEXT("##################################      START Gamemode    #######################################"));
		UE_LOG(LogTemp, Display, TEXT("GM_SERVER_WORLD: BeginPlay"));

		UE_LOG(LogTemp, Display, TEXT("##################################        END Gamemode    #######################################"));
	#endif
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

}

APlayerController* AOpenMMORPGGameMode::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal,
	const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	GLog->Log("######            Server proceeds with login for client            #######");
	
	auto Ctrl = Cast<AOpenMMORPGPlayerController>(Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage));
	if(!UGameplayStatics::ParseOption(Options, "Token").IsEmpty() && !UGameplayStatics::ParseOption(Options, "PlayerID").IsEmpty())
	{
		auto PlayerService = new UserService(UGameplayStatics::ParseOption(Options, "Token"), UGameplayStatics::ParseOption(Options, "PlayerID"));
		proto_messages::Player* Player = PlayerService->GetCharacterProfile();
		if (Player == nullptr)
		{
			// TODO: Handle this on UI
			GLog->Log("Could not login to server");
			ErrorMessage = "Could not login to server because no player info received";
		} else {
			Ctrl->PlayerInfo = Player;
			Ctrl->AuthToken = UGameplayStatics::ParseOption(Options, "Token");
			Ctrl->PlayerID = UGameplayStatics::ParseOption(Options, "PlayerID");	
		}
		return Ctrl;
	}
	
	return Ctrl;
}

AActor* AOpenMMORPGGameMode::FindPlayerStart_Implementation(AController* Player, const FString& IncomingName)
{
	auto Ctrl = Cast<AOpenMMORPGPlayerController>(Player);


	if (Ctrl && Ctrl->PlayerInfo)
	{
		return GetWorld()->SpawnActor<APlayerStart>(FVector(Ctrl->PlayerInfo->location().x(), Ctrl->PlayerInfo->location().y(), Ctrl->PlayerInfo->location().z()), FRotator(0, Ctrl->PlayerInfo->rotation().yaw(), 0));
	}

	// We dont have PlayerInfo on initial start in menu, hence we want to return 0,0,0
	return GetWorld()->SpawnActor<APlayerStart>(FVector(0, 0, 0), FRotator());
	
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
