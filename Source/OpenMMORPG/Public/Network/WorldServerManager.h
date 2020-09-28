// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "WorldServerEntity.h"
#include "UObject/NoExportTypes.h"
#include "WorldServerManager.generated.h"

/**
 * 
 */
class UWorldServerEntity;
UCLASS()
class UWorldServerManager : public UObject
{
 GENERATED_BODY()
public:
 
 UFUNCTION(BlueprintCallable)
 static TArray<UWorldServerEntity*> GetListOfServers();
 static TArray<UWorldServerEntity*> ServerList;
 
};
