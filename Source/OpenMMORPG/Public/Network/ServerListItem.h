// Created by Alex Sinelnikov - sinelniko@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ServerListItem.generated.h"

/**
 * 
 */
UCLASS()
class OPENMMORPG_API UServerListItem : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	int32 CharacterCount;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	int32 Id;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FString Name;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FString GameServerPort;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FString GameServerURL;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FString WorldServerGrpcPort;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FString WorldServerUdpPort;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FString WorldServerURL;

	static UServerListItem* CreateServerListItem(
		int32 CharacterCount, int32 Id, FString Name, FString GameServerPort, FString GameServerURL, FString WorldServerGrpcPort, FString WorldServerUdpPort, FString WorldServerUrl
		);
};
