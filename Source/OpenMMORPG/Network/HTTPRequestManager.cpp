// Created by Alex Sinelnikov - sinelniko@gmail.com


#include "HTTPRequestManager.h"


#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"

UHTTPRequestManager::UHTTPRequestManager()
{
    //Reset();
}

bool UHTTPRequestManager::FromString(const FString& dataString)
{
    TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(dataString);

    // Deserialize the JSON data
    const bool isDeserialized = FJsonSerializer::Deserialize(JsonReader, Data);

    if (!isDeserialized || !Data.IsValid()) {
        UE_LOG(LogJson, Error, TEXT("JSON data is invalid! Input:\n'%s'!"), *dataString);
        return false;
    }

    return true;
}

UHTTPRequestManager* UHTTPRequestManager::Create()
{
    return NewObject<UHTTPRequestManager>();
}

UHTTPRequestManager* UHTTPRequestManager::GetRequest(const FString &url) {
    

    
    TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
    HttpRequest->SetHeader("Authorization", "Bearer eyJhbGciOiJIUzUxMiIsInR5cCI6IkpXVCJ9.eyJhdWQiOiJvcGVubW1vcnBnX2xvZ2luc2VydmVyIiwiZXhwIjoxNjAzNjQ0OTY5LCJpYXQiOjE2MDEyMjU3NjksImlzcyI6Im9wZW5tbW9ycGdfbG9naW5zZXJ2ZXIiLCJqdGkiOiJlOThlY2EyNS1lYzMzLTQ4YzgtYWQ5NS0wYzBhY2MzNGJmYzkiLCJuYmYiOjE2MDEyMjU3NjgsInN1YiI6IjEiLCJ0eXAiOiJhY2Nlc3MifQ.NdB7dU71-yXdwx-hzxsKHRl0GumBwwUGodOHtefbxlwLtq_FfXL8o9bAaCM9nJgGdzrAhPzGcf2ju08pxjYMpQ");
    HttpRequest->SetVerb("GET");
    HttpRequest->SetURL(CreateUrl(url));
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &UHTTPRequestManager::OnReady);

    this->AddToRoot();

    HttpRequest->ProcessRequest();

    return this;
}

FString UHTTPRequestManager::CreateUrl(FString Url)
{

    if (!Url.StartsWith("http")) {
        return "http://" + Url;
    }

    return Url;
}

TArray<UHTTPRequestManager*> UHTTPRequestManager::GetObjectArray(const FString& key, bool& success) {
    TArray<UHTTPRequestManager*> objectArray;
    const TArray<TSharedPtr<FJsonValue>>* valueArray;
    success = Data->TryGetArrayField(*key, valueArray);
    if (success) {
        for (int32 i = 0; i < valueArray->Num(); i++) {
            UHTTPRequestManager* pageData = Create();
            pageData->Data = (*valueArray)[i]->AsObject();
            objectArray.Add(pageData);
        }
    }

    return objectArray;
}

void UHTTPRequestManager::OnReady(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    RemoveFromRoot();
    if (!bWasSuccessful) {
        UE_LOG(LogJson, Error, TEXT("Response was invalid! Please check the URL. Error: %i"), Response->GetResponseCode());
		
        // Broadcast the failed event
        Request_OnFinished.Broadcast(false, this, ERequestResult::HttpFailed);
        return;
    }

    // Process the string
    if (!FromString(Response->GetContentAsString())) {
        Request_OnFinished.Broadcast(false, this, ERequestResult::JSONParsingFailed);
    }

    // Broadcast the result event
    if (Request_OnFinished.IsBound())
    {
        GLog->Log("Broadcasting On Finished");
        Request_OnFinished.Broadcast(true, this, ERequestResult::Success);    
    }
    
    GLog->Log("Request done");
}
