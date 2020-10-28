// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenMMORPG/Public/Network/WorldServerEntity.h"

#include "Network/WorldServerManager.h"


UWorldServerEntity::UWorldServerEntity()
{
}

TArray<UWorldServerEntity*> UWorldServerEntity::FromJSON(TSharedPtr<FJsonObject> JsonObject)
{
    TArray<UWorldServerEntity*> ServersList;
    GenerateStructsFromJson(ServersList, JsonObject);
    return ServersList;
}

void UWorldServerEntity::GenerateStructsFromJson(
    TArray<UWorldServerEntity*> &ServerList, 
    TSharedPtr<FJsonObject> JsonObject
    )
{
    TArray<TSharedPtr<FJsonValue>> objArray = JsonObject->GetArrayField(TEXT("servers"));

    for (int32 i = 0; i < objArray.Num(); i++)
    {
        TSharedPtr<FJsonValue> value = objArray[i];
        TSharedPtr<FJsonObject> json = value->AsObject();

        const FString ServerName = json->GetStringField(TEXT("name"));
        const FString GameServerUrl = json->GetStringField(TEXT("game_server_url"));
        const FString GameServerPort = json->GetStringField(TEXT("game_server_port"));
        const FString WorldServerUrl = json->GetStringField(TEXT("world_server_url"));
        const FString WorldServerUdpPort = json->GetStringField(TEXT("world_server_udp_port"));
        const FString WorldServerGrpcPort = json->GetStringField(TEXT("world_server_udp_port"));
        const int32 Id = json->GetNumberField(TEXT("id"));
        const int32 CharacterCount = json->GetNumberField(TEXT("character_count"));
        
        //bool     bAllowedToJoin = json->GetBoolField(TEXT("allowed_to_join"));
		
        UWorldServerEntity* ServerEntity = InitNewFWorldServerEntity(
            ServerName,
            GameServerUrl,
            GameServerPort,
            WorldServerUrl,
            WorldServerUdpPort,
            WorldServerGrpcPort,
            Id,
            CharacterCount,
            1,
            true
        );

        ServerList.Push(ServerEntity);
    }
}

UWorldServerEntity* UWorldServerEntity::InitNewFWorldServerEntity(FString ServerName, FString GameServerUrl,
                                                                  FString GameServerPort, FString WorldServerUrl, FString WorldServerUdpPort, FString WorldServerGrpcPort, int32 Id,
                                                                  uint8 CharacterCount, uint8 ServerLoad, bool bAllowedToJoin)
{
    UWorldServerEntity* Entity =  NewObject<UWorldServerEntity>();
    Entity->Id = Id;
    Entity->ServerName = ServerName;
    Entity->WorldServerUrl = WorldServerUrl;
    Entity->CharacterCount = CharacterCount;
    Entity->ServerLoad = ServerLoad;
    Entity->bAllowedToJoin = bAllowedToJoin;
    Entity->GameServerUrl = GameServerUrl;
    Entity->GameServerPort = GameServerPort;
    Entity->GameServerUrl = GameServerUrl;
    Entity->WorldServerGrpcPort = WorldServerGrpcPort;
    Entity->WorldServerUdpPort = WorldServerUdpPort;
    return Entity;
}

