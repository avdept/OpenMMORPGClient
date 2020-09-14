// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenMMORPG/Public/Network/WorldServerEntity.h"


UWorldServerEntity::UWorldServerEntity()
{
}

UWorldServerEntity* UWorldServerEntity::InitNewFWorldServerEntity(FString ServerName, FString ServerURL, uint8 CharacterCount, uint8 ServerLoad,
                                                                  bool bAllowedToJoin)
{
    UWorldServerEntity* Entity =  NewObject<UWorldServerEntity>();
    Entity->ServerName = ServerName;
    Entity->ServerURL = ServerURL;
    Entity->CharacterCount = CharacterCount;
    Entity->ServerLoad = ServerLoad;
    Entity->bAllowedToJoin = bAllowedToJoin;
    return Entity;
}
