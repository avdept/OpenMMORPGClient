// Created by Alex Sinelnikov - sinelniko@gmail.com
// DOC:
// This class is a wrapper for characters json response. It takes care of parsing and creating new objects for each character

#include "OpenMMORPG/Public/UI/UI_CharacterItem.h"

UUI_CharacterItem::UUI_CharacterItem()
{
}

UUI_CharacterItem* UUI_CharacterItem::InitNewCharacterItem(FString Name, int32 ID)
{
    UUI_CharacterItem* Entity =  NewObject<UUI_CharacterItem>();
    Entity->Name = Name;
    Entity->ID = ID;
    return Entity;
}

TArray<UUI_CharacterItem*> UUI_CharacterItem::FromJSON(TSharedPtr<FJsonObject> JsonObject)
{
    TArray<UUI_CharacterItem*> CharacterList;
    GenerateStructsFromJson(CharacterList, JsonObject);
    return CharacterList;
}

void UUI_CharacterItem::GenerateStructsFromJson(TArray<UUI_CharacterItem*>& CharacterList,
    TSharedPtr<FJsonObject> JsonObject)
{
    TArray<TSharedPtr<FJsonValue>> objArray = JsonObject->GetArrayField(TEXT("characters"));

    for (int32 i = 0; i < objArray.Num(); i++)
    {
        const TSharedPtr<FJsonObject> json = objArray[i]->AsObject();

        const FString Name = json->GetStringField(TEXT("name"));
        const int32 ID = json->GetNumberField(TEXT("id"));
		
        UUI_CharacterItem* CharacterEntity = InitNewCharacterItem(
            Name, ID
        );

        CharacterList.Push(CharacterEntity);
    }
}
