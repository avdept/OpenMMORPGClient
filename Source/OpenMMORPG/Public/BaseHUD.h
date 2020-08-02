// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BaseHUD.generated.h"

/**
 * 
 */
UCLASS()
class OPENMMORPG_API ABaseHUD : public AHUD
{
	GENERATED_BODY()

	public:
	FString GetSHA256_s(const void * data, size_t data_len);

	virtual void DrawHUD() override;
	//virtual void BeginPlay() override;
};
