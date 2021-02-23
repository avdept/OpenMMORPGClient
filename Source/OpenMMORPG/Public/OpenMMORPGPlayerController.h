// Created by Alex Sinelnikov - sinelniko@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "OpenMMORPG/Network/Proto/Player.grpc.pb.h"

#include "OpenMMORPGPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class OPENMMORPG_API AOpenMMORPGPlayerController : public APlayerController
{
	GENERATED_BODY()

public:	
	proto_messages::Player* PlayerInfo;
	
};
