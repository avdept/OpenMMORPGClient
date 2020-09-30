// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Network/ServerListItem.h"
#include "OpenMMORPG/Network/HTTPRequestManager.h"


#include "OpenMMORPG_GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class OPENMMORPG_API UOpenMMORPG_GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly)
	FString AuthToken;
	virtual void Init() override;
	virtual void Shutdown() override;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TArray<UServerListItem*> ServerListItemItems;

	UFUNCTION(BlueprintCallable)
	void OnServerListFetched(bool bSuccess, UHTTPRequestManager* RequestManager, ERequestResult status);
	
	void SetCommandLineArgs();
};
