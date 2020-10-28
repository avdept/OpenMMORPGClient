// Created by Alex Sinelnikov - sinelniko@gmail.com


#include "OpenMMORPG/Public/Levels/L_MainMenu.h"
#include "OpenMMORPG/Network/HTTPRequestManager.h"

void AL_MainMenu::FetchServerList()
{
    UHTTPRequestManager* HttpClientInstance = UHTTPRequestManager::Create();
    HttpClientInstance->Request_OnFinished.AddDynamic(this, &AL_MainMenu::OnServerListFetched);
    HttpClientInstance->GetRequest(FString("http://10.0.1.7:4000/api/servers"));
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
    HttpClientInstance->GetRequest(FString::Printf(TEXT("http://10.0.1.7:4000/api/servers/%d/characters"), ServerId));
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
