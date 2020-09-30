// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenMMORPG/Public/Network/WorldServerManager.h"
#include "OpenMMORPG/Public/Network/WorldServerEntity.h"


TArray<UWorldServerEntity*> UWorldServerManager::ServerList  = {};

TArray<UWorldServerEntity*> UWorldServerManager::GetListOfServers()
{
    return ServerList;
}
