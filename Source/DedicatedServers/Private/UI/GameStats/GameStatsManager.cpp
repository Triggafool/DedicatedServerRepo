// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameStats/GameStatsManager.h"
#include "HttpModule.h"
#include "JsonObjectConverter.h"
#include "Data/API/APIData.h"
#include "Player/DSLocalPlayerSubSystem.h"
#include "DedicatedServers/DedicatedServers.h"
#include "Interfaces/IHttpResponse.h"
#include "UI/HTTP/HTTPRequestTypes.h"


void UGameStatsManager::RecordMatchStats(const FDSRecordMatchStatsInput& RecordMatchStatsInput)
{
	/* if we want a json object
	TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();
	FJsonObjectConverter::UStructToJsonObject(FDSRecordMatchStatsInput::StaticStruct(), &RecordMatchStatsInput, JsonObject.ToSharedRef());
	*/
	
	FString JsonString;
	FJsonObjectConverter::UStructToJsonObjectString(FDSRecordMatchStatsInput::StaticStruct(), &RecordMatchStatsInput, JsonString);

	GEngine->AddOnScreenDebugMessage(-1, 600.f, FColor::Red, JsonString);

	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UGameStatsManager::RecordMatchStats_Response);
	const FString APIURL = APIData->GetAPIEndPoint(DedicatedServersTags::GameStatsAPI::RecordMatchStats);

	Request->SetURL(APIURL);
	Request->SetVerb("POST");
	Request->SetHeader("Content-Type", "application/json");
	Request->SetContentAsString(JsonString);

	Request->ProcessRequest();
	
}

void UGameStatsManager::RecordMatchStats_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		UE_LOG(LogDedicatedServers, Warning, TEXT("Failed to Record Game Stats"));
		return;
	}
	
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	if (FJsonSerializer::Deserialize(JsonReader, JsonObject)) 
	{
		ContainsErrors(JsonObject);
	}
	
}

void UGameStatsManager::RetrieveMatchStats()
{
	RetrieveGameStatsStatusMessage.Broadcast(TEXT("Retrieving Match Stats..."), false);
	UDSLocalPlayerSubSystem* LocalPlayerSubSystem = GetDSLocalPlayerSubsytem();
	if (!IsValid(LocalPlayerSubSystem)) return;

	check (APIData);
	const FString AccessToken = LocalPlayerSubSystem->GetAuthResults().AccessToken;
	

	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UGameStatsManager::RetrieveMatchStats_Response);
	const FString APIURL = APIData->GetAPIEndPoint(DedicatedServersTags::GameStatsAPI::RetrieveMatchStats);

	Request->SetURL(APIURL);
	Request->SetVerb("POST");
	Request->SetHeader("Content-Type", "application/json");
	
		const TMap<FString, FString> Params = {
			{TEXT("accessToken"), AccessToken },
		};
	
	Request->SetContentAsString(SerializeJsonContent(Params));
	Request->ProcessRequest();
	
}
void UGameStatsManager::RetrieveMatchStats_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	FDSRetrieveMatchStatsResponse RetrieveMatchStatsResponse;

	if (!bWasSuccessful)
	{
		OnRetrieveMatchStatsResponse.Broadcast(RetrieveMatchStatsResponse);
		RetrieveGameStatsStatusMessage.Broadcast(HTTPStatusMessages::SomethingWentWrong, false);
		return;
	}
	
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	if (FJsonSerializer::Deserialize(JsonReader, JsonObject)) 
	{
		ContainsErrors(JsonObject);
		RetrieveGameStatsStatusMessage.Broadcast(HTTPStatusMessages::SomethingWentWrong, false);
	}
	
	FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), &RetrieveMatchStatsResponse);
	RetrieveMatchStatsResponse.Dump();
	OnRetrieveMatchStatsResponse.Broadcast(RetrieveMatchStatsResponse);
	RetrieveGameStatsStatusMessage.Broadcast(TEXT(""), false);
	
}

void UGameStatsManager::UpdateLeaderboard(const TArray<FString>& WinnerUsernames)
{
	check (APIData);

	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UGameStatsManager::UpdateLeaderboard_Response);
	const FString APIURL = APIData->GetAPIEndPoint(DedicatedServersTags::GameStatsAPI::UpdateLeaderboard);

	Request->SetURL(APIURL);
	Request->SetVerb("POST");
	Request->SetHeader("Content-Type", "application/json");

	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

	TArray<TSharedPtr<FJsonValue>> PlayerIdJsonArray;

	for (const FString& Username : WinnerUsernames )
	{
		PlayerIdJsonArray.Add(MakeShareable(new FJsonValueString(Username)));
		
	}
	JsonObject->SetArrayField("playerIds", PlayerIdJsonArray);

	FString Content;
	const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Content);

	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	Request->SetContentAsString(Content);
	
	Request->ProcessRequest();
}

void UGameStatsManager::UpdateLeaderboard_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		return;
	}
	
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
	{
		if (ContainsErrors(JsonObject))
		{
			return;
		}
	}

	OnUpdateLeaderSucceeded.Broadcast();
}

void UGameStatsManager::RetrieveLeaderboard()
{
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UGameStatsManager::RetrieveLeaderboard_Response);
	const FString APIURL = APIData->GetAPIEndPoint(DedicatedServersTags::GameStatsAPI::RetrieveLeaderboard);

	Request->SetURL(APIURL);
	Request->SetVerb("GET");
	Request->SetHeader("Content-Type", "application/json");
	
	Request->ProcessRequest();
}

void UGameStatsManager::RetrieveLeaderboard_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		UE_LOG(LogDedicatedServers, Error, TEXT("Failed to retrieve Leaderboard."))
		return;
	}
	
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

	TArray<FDSLeaderboardItem> LeaderboardItems;
	
	if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
	{
		if (ContainsErrors(JsonObject))
		{
			return;
		}

		const TArray<TSharedPtr<FJsonValue>>* LeaderboardJsonArray;

		if (JsonObject->TryGetArrayField(TEXT("Leaderboard"), LeaderboardJsonArray))
		{
			for (const TSharedPtr<FJsonValue>& ItemValue : *LeaderboardJsonArray)
			{
				TSharedPtr<FJsonObject> ItemObject = ItemValue->AsObject();
				if (ItemObject.IsValid())
				{
					FDSLeaderboardItem Item;
					if (FJsonObjectConverter::JsonObjectToUStruct(ItemObject.ToSharedRef(), &Item))
					{
						LeaderboardItems.Add(Item);
					} else
					{
						UE_LOG(LogDedicatedServers, Error, TEXT("Failed to parse leaderboard item."));
					}
				}
			}
		}
	}

	OnRetrieveLeaderboardDelegate.Broadcast(LeaderboardItems);
}

