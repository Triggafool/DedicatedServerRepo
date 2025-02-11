﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/IHttpRequest.h"

#include "UI/HTTP/HTTPRequestManager.h"
#include "PortalManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBroadcastJoinGameSessionMessage, const FString&, StatusMessage, bool, bShouldResetJoinGameButton);


/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UPortalManager : public UHTTPRequestManager
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FBroadcastJoinGameSessionMessage BroadcastJoinGameSessionMessage;
	
	void JoinGameSession();


private:
	FTimerHandle CreateSessionTimer;
	
	FString GetUniquePlayerId();
	void FindOrCreateGameSession_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void CreatePlayerSession_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void HandleGameSessionStatus(const FString& Status, const FString& GameSessionId );
	void TryCreatePlayerSession(const FString& PlayerId, const FString& GameSessionId);
};
