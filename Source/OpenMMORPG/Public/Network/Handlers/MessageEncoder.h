// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OpenMMORPG_GameInstance.h"
#include "google/protobuf/message.h"
#include "Network/NetworkSocketManager.h"

#include "UObject/NoExportTypes.h"
#include "MessageEncoder.generated.h"

/**
 * 
 */
UCLASS()
class OPENMMORPG_API UMessageEncoder : public UObject
{
	GENERATED_BODY()

public:
	static const std::string UtilityMessageName;
	static bool Send(google::protobuf::Message * Message, bool bCrypt, bool bTCP);
};
