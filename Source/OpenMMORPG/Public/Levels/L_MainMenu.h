// Created by Alex Sinelnikov - sinelniko@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "Network/WorldServerEntity.h"
#include "OpenMMORPG/Public/UI/UI_CharacterItem.h"
#include "OpenMMORPG/Network/HTTPRequestManager.h"
#include "OpenMMORPG/Network/Proto/Player.pb.h"

#include "L_MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class OPENMMORPG_API AL_MainMenu : public ALevelScriptActor
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly)
	TArray<UWorldServerEntity*> ServerList;

	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly)
	TArray<UUI_CharacterItem*> CharactersList; 

	UFUNCTION(BlueprintCallable)
	void FetchServerList();

	UFUNCTION(BlueprintCallable)
	void FetchCharactersList(int ServerID);

	UFUNCTION(BlueprintCallable)
    void OnServerListFetched(bool bSuccess, UHTTPRequestManager* RequestManager, ERequestResult status);

	UFUNCTION(BlueprintCallable)
	void OnCharactersListFetched(bool bSuccess, UHTTPRequestManager* RequestManager, ERequestResult status);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void ShowMenu();

	void HandleServerJoin(const proto_messages::Player& Player, const UWorldServerEntity* Server);

	void HandleServerLoginError(const proto_messages::Player& Player);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnJoinServerSuccess();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnJoinServerFailed(const FString &Message, bool NeedsImmediateExit);
	
	UFUNCTION(BlueprintCallable)
	bool JoinServer(UWorldServerEntity *Server, int CharacterID);
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void ShowCharactersListMenu();
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void ShowNewCharacterMenu();
	
};
