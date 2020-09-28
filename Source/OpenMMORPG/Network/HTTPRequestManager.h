// Created by Alex Sinelnikov - sinelniko@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "Http.h"
#include "Json.h"
#include "Interfaces/IHttpRequest.h"
#include "UObject/NoExportTypes.h"
#include "HTTPRequestManager.generated.h"

/**
 * 
 */
UENUM(BlueprintType, Category = "JSON")
enum class ERequestResult : uint8
{
	Success = 0,
    HttpFailed,
    JSONParsingFailed
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnGetResult, const bool, bSuccess, class UHTTPRequestManager*, JSON, const ERequestResult, Status);


UCLASS()
class OPENMMORPG_API UHTTPRequestManager : public UObject
{
	GENERATED_BODY()
public:

	TSharedPtr<FJsonObject> Data;

	UPROPERTY(BlueprintAssignable, Category = Callback)
	FOnGetResult Request_OnFinished;
	
	static TSharedPtr<UHTTPRequestManager> Instance;
	
	UHTTPRequestManager();

    bool FromString(const FString& dataString);
	
    static UHTTPRequestManager* Create();
	UHTTPRequestManager* GetRequest(const FString& url);
	TArray<UHTTPRequestManager*> GetObjectArray(const FString& key, bool& success);

private:
	static FString CreateUrl(FString Url);
	void OnReady(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
};
