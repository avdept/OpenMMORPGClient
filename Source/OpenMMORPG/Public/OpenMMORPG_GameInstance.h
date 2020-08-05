// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OpenMMORPG_GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class OPENMMORPG_API UOpenMMORPG_GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	//virtual void Shutdown() override;
};
