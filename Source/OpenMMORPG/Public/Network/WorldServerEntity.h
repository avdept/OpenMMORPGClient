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
	static TArray<UWorldServerEntity*> FromJSON(TSharedPtr<FJsonObject>);
	static void GenerateStructsFromJson(TArray<UWorldServerEntity*> &ServerList, TSharedPtr<FJsonObject> JsonObject);
	static UWorldServerEntity* InitNewFWorldServerEntity(FString ServerName,
		FString GameServerUrl,
		FString GameServerPort,
		FString WorldServerUrl,
		FString WorldServerUdpPort,
		FString WorldServerGrpcPort,
		int32 Id,
		uint8 CharacterCount,
		uint8 ServerLoad,
		bool bAllowedToJoin);
	
	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly)
	FString ServerName;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FString GameServerUrl;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FString GameServerPort;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FString WorldServerUrl;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FString WorldServerUdpPort;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FString WorldServerGrpcPort;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	uint8 CharacterCount;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	uint8 ServerLoad;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	int32 Id;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool bAllowedToJoin;
};