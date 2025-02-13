// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "UI/HTTP/HTTPRequestTypes.h"
#include "DSLocalPlayerSubSystem.generated.h"

class IPortalManagement;
class UPortalManager;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UDSLocalPlayerSubSystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()

public:
	
	void InitializeTokens(const FDSAuthenticationResult& AuthResult, TScriptInterface<IPortalManagement> PortalManagement);
	void SetRefreshTokenTimer();
	void UpdateTokens(const FString& AccessToken, const FString& IdToken);

private:
	
	UPROPERTY()
	FDSAuthenticationResult AuthenticationResult;

	UPROPERTY()
	TScriptInterface<IPortalManagement> PortalManagementInterface;
	
	// 75% of an hour (the expiration time for AccessToken and IdToken)
	float TokenRefreshInterval = 2700.f;
	
	FTimerHandle RefreshTimer;
};
