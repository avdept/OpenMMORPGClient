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

	#if WITH_SERVER_CODE

		UE_LOG(LogTemp, Display, TEXT("##################################      START Gamemode    #######################################"));
		UE_LOG(LogTemp, Display, TEXT("GM_SERVER_WORLD: BeginPlay"));


		UE_LOG(LogTemp, Display, TEXT("##################################        END Gamemode    #######################################"));
	#endif
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
	GLog->Log("######Server proceeds with login for client");
	auto Ctrl = Cast<AOpenMMORPGPlayerController>(Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage));
	GLog->Log("Controller set");
	if(!UGameplayStatics::ParseOption(Options, "Token").IsEmpty() && !UGameplayStatics::ParseOption(Options, "PlayerID").IsEmpty())
	{
		auto PlayerService = new UserService(UGameplayStatics::ParseOption(Options, "Token"), UGameplayStatics::ParseOption(Options, "PlayerID"));
		proto_messages::Player* Player = PlayerService->GetCharacterProfile();
		GLog->Log("Requested player info");
		GLog->Log(TCHAR_TO_UTF8(Player->name().c_str()));
		if (Player == nullptr)
		{
			// TODO: Handle this on UI
		GLog->Log("Could not login to server");
		ErrorMessage = "Could not login to server";
		}

		Ctrl->PlayerInfo = Player;

		Ctrl->Name = UTF8_TO_TCHAR(Player->name().c_str());
			
		return Ctrl;
	}
	

	GLog->Log("Returning default");
	return Ctrl;
}

AActor* AOpenMMORPGGameMode::FindPlayerStart_Implementation(AController* Player, const FString& IncomingName)
{
	auto Ctrl = Cast<AOpenMMORPGPlayerController>(Player);

	if (Ctrl->PlayerInfo)
	{
		return GetWorld()->SpawnActor<APlayerStart>(FVector(Ctrl->PlayerInfo->location().x(), Ctrl->PlayerInfo->location().y(), Ctrl->PlayerInfo->location().z()), FRotator());
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
