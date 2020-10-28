// Created by Alex Sinelnikov - sinelniko@gmail.com
// DOC:
// This class used as a wrapper for Character Entity displayed in Character Selection Menu. This class can be expanded with any other information needed to display any character related data
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UI_CharacterItem.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class OPENMMORPG_API UUI_CharacterItem : public UObject
{
	GENERATED_BODY()

public:
    UUI_CharacterItem();

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
    FString Name;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
    int32 ID;

    static UUI_CharacterItem* InitNewCharacterItem(FString Name, int32 ID);

    static TArray<UUI_CharacterItem*> FromJSON(TSharedPtr<FJsonObject>);
    static void GenerateStructsFromJson(TArray<UUI_CharacterItem*> &ServerList, TSharedPtr<FJsonObject> JsonObject);
    
};
