// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "NetworkInterface.generated.h"

/**
 * 
 */
UCLASS()
class OPENMMORPG_API UNetworkInterface : public UObject
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable)
	static void SendUDPMessage();
	
	UFUNCTION(BlueprintCallable)
	static void SendGRCPMessage();

	UFUNCTION(BlueprintCallable)
	static void SendTCPMessage();
	
};
