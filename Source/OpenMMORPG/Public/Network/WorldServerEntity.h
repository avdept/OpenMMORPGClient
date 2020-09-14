// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WorldServerEntity.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class UWorldServerEntity : public UObject
{
	GENERATED_BODY()
public:

	UWorldServerEntity();
	static UWorldServerEntity* InitNewFWorldServerEntity(FString ServerName, FString ServerURL, uint8 CharacterCount,uint8 ServerLoad, bool bAllowedToJoin);
	
	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly)
	FString ServerName;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FString ServerURL;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	uint8 CharacterCount;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	uint8 ServerLoad;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool bAllowedToJoin;
};