// Fill out your copyright notice in the Description page of Project Settings.



#include "WorldStaticManager.h"

UOpenMMORPG_GameInstance* WorldStaticManager::GameInstance = nullptr;
int WorldStaticManager::CurrentCharacterId = 0;
int WorldStaticManager::CurrentUserId = 0;

WorldStaticManager::WorldStaticManager()
{
}

WorldStaticManager::~WorldStaticManager()
{
}
