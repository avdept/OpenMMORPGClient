// Created by Alex Sinelnikov - sinelniko@gmail.com


#include "OpenMMORPG/Public/Levels/L_MainMenu.h"


#include "WorldStaticManager.h"
#include "Kismet/GameplayStatics.h"
#include "Network/WorldServerManager.h"
#include "OpenMMORPG/Network/HTTPRequestManager.h"
#include "OpenMMORPG/Network/NetworkConfig.h"
#include "OpenMMORPG/Network/SocketObject.h"
#include "OpenMMORPG/Network/Proto/Server.grpc.pb.h"
#include "OpenMMORPG/Network/Proto/Server.pb.h"

void AL_MainMenu::FetchServerList()
{
    UHTTPRequestManager* HttpClientInstance = UHTTPRequestManager::Create();
    HttpClientInstance->Request_OnFinished.AddDynamic(this, &AL_MainMenu::OnServerListFetched);
    HttpClientInstance->GetRequest(NetworkConfig::ServersListUrl);
}

void AL_MainMenu::OnServerListFetched(bool bSuccess, UHTTPRequestManager* RequestManager, ERequestResult status)
{
    if (bSuccess)
    {
        ServerList = UWorldServerEntity::FromJSON(RequestManager->Data);    
    } else
    {
        ServerList = {};
    }

    ShowMenu();
    
}

void AL_MainMenu::FetchCharactersList(int const ServerId)
{
    UHTTPRequestManager* HttpClientInstance = UHTTPRequestManager::Create();
    HttpClientInstance->Request_OnFinished.AddDynamic(this, &AL_MainMenu::OnCharactersListFetched);
    HttpClientInstance->GetRequest( NetworkConfig::CharacterServerUrl(ServerId));
}


void AL_MainMenu::OnCharactersListFetched(bool bSuccess, UHTTPRequestManager* RequestManager, ERequestResult status)
{
    if (bSuccess)
    {
        CharactersList = UUI_CharacterItem::FromJSON(RequestManager->Data);
    } else
    {
        CharactersList = {};   
    }

    ShowCharactersListMenu();
}


void AL_MainMenu::HandleServerJoin(const proto_messages::Player& Player, const UWorldServerEntity* Server)
{
    WorldStaticManager::CurrentCharacterId = Player.id();
    
    const FString LevelName = Server->GameServerUrl + ":" + Server->GameServerPort;
    GLog->Log(LevelName);
    const FString Options = "PlayerID=" + FString::FromInt(Player.id()) + "?Token=" + WorldStaticManager::GameInstance->AuthToken;
    UGameplayStatics::OpenLevel(GetWorld(), FName(*LevelName), true, Options);
    auto inst = Cast<UOpenMMORPG_GameInstance>(GetGameInstance());
    inst->Player = Player;
    OnJoinServerSuccess();
    
}


void AL_MainMenu::HandleServerLoginError(const proto_messages::Player& Player)
{
    GLog->Log("Error while processing server login");
    if (Player.has_error())
    {
        OnJoinServerFailed(FString(Player.error().errormessage().c_str()), Player.error().shouldquitimmediately());
    } else
    {
        OnJoinServerFailed(FString(TEXT("Could not connect to server")), true);
    }
    
}



bool AL_MainMenu::JoinServer(UWorldServerEntity* Server, int CharacterID)
{
    
    proto_messages::Player Player;
    bool const bStatus = USocketObject::ConnectToWorldServer(Server, CharacterID, &Player);
    if (bStatus)
    {
        if(Player.requestresult())
        {
            HandleServerJoin(Player, Server);
        } else
        {
            HandleServerLoginError(Player);
        }
    } else
    {
        OnJoinServerFailed(FString(TEXT("Could not connect to server")), true);
        GLog->Log("Failed to join server");
    }
  
    return true;
}
