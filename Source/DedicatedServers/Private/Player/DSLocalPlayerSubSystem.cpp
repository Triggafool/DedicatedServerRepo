// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DSLocalPlayerSubSystem.h"
#include "UI/Portal/Interfaces/PortalManagement.h"

void UDSLocalPlayerSubSystem::InitializeTokens(const FDSAuthenticationResult& AuthResult, TScriptInterface<IPortalManagement> PortalManagement)
{
	AuthenticationResult = AuthResult;
	PortalManagementInterface = PortalManagement;

	SetRefreshTokenTimer();
	
}

void UDSLocalPlayerSubSystem::SetRefreshTokenTimer()
{
	UWorld* World = GetWorld();
	if (IsValid(World) && IsValid(PortalManagementInterface.GetObject()))
	{
		FTimerDelegate RefreshDelegate;
		RefreshDelegate.BindLambda(
			[this]()
			{
				PortalManagementInterface->RefreshTokens(AuthenticationResult.RefreshToken);
			});
		World->GetTimerManager().SetTimer(RefreshTimer, RefreshDelegate, TokenRefreshInterval, false);
	}
}

void UDSLocalPlayerSubSystem::UpdateTokens(const FString& AccessToken, const FString& IdToken)
{
	AuthenticationResult.AccessToken = AccessToken;
	AuthenticationResult.IdToken = IdToken;
	AuthenticationResult.Dump();
	SetRefreshTokenTimer();
}
