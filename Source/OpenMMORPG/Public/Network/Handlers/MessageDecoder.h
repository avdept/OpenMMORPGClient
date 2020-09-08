// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OpenMMORPG/Network/Proto/MessageModels.pb.h"
#include "UObject/NoExportTypes.h"
#include "MessageDecoder.generated.h"

/**
 * 
 */
UCLASS()
class OPENMMORPG_API UMessageDecoder : public UObject
{
	GENERATED_BODY()
public:
	void SendProtoToDecoder(Wrapper * wrapper);
	
};
