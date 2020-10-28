// Created by Alex Sinelnikov - sinelniko@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "Network/WorldServerEntity.h"
#include "OpenMMORPG/Public/UI/UI_CharacterItem.h"
#include "OpenMMORPG/Network/HTTPRequestManager.h"


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
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void ShowCharactersListMenu();
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void ShowNewCharacterMenu();
	
};
