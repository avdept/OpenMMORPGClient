// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenMMORPG/Public/Network/WorldServerManager.h"

TArray<UWorldServerEntity*> UWorldServerManager::GetListOfServers()
{
    TArray<UWorldServerEntity*> Entities;
    UWorldServerEntity* Item1 = UWorldServerEntity::InitNewFWorldServerEntity(FString("Lucius"), FString("127.0.0.1:9001"), 2, 1, true);
    UWorldServerEntity* Item2 = UWorldServerEntity::InitNewFWorldServerEntity(FString("Gildaron"), FString("127.0.0.1:9002"), 0, 3, false);
    Entities.Add(Item1);
    Entities.Add(Item2);
    return Entities;
}
