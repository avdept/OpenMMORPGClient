// Created by Alex Sinelnikov - sinelniko@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "L_BaseLevel.generated.h"

/**
 * 
 */
UCLASS()
class OPENMMORPG_API AL_BaseLevel : public ALevelScriptActor
{
	GENERATED_BODY()

	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable)
	void SpawnPlayer();
};
