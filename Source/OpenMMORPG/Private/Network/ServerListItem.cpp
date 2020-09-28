// Created by Alex Sinelnikov - sinelniko@gmail.com


#include "OpenMMORPG/Public/Network/ServerListItem.h"

UServerListItem* UServerListItem::CreateServerListItem(int32 CharacterCount, int32 Id, FString Name,
    FString GameServerPort, FString GameServerURL, FString WorldServerGrpcPort, FString WorldServerUdpPort,
    FString WorldServerUrl)
{
    UServerListItem *ListItem = NewObject<UServerListItem>();
    ListItem->Id = Id;
    ListItem->CharacterCount = CharacterCount;
    ListItem->Name = Name;
    ListItem->GameServerPort = GameServerPort;
    ListItem->GameServerURL = GameServerURL;
    ListItem->WorldServerGrpcPort = WorldServerGrpcPort;
    ListItem->WorldServerUdpPort = WorldServerUdpPort;
    ListItem->WorldServerURL = WorldServerUrl;
    return ListItem;
}
