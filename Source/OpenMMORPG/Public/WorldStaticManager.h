// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OpenMMORPG_GameInstance.h"

/**
 * 
 */
class OPENMMORPG_API WorldStaticManager
{
public:
	static UOpenMMORPG_GameInstance* GameInstance;
	static int CurrentCharacterId;
	static int CurrentUserId;
	WorldStaticManager();
	~WorldStaticManager();
};
