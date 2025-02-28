// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Match/MatchOverlay.h"
#include "Player/DSPlayerController.h"
#include "Player/DS_MatchPlayerState.h"
#include "UI/Match/Scoreboard.h"

void UMatchOverlay::NativeConstruct()
{
	Super::NativeConstruct();
	
	// we need to bind to when the button is clicked from the player controller.
	ADSPlayerController* PlayerController = Cast<ADSPlayerController>(GetOwningPlayer()->GetLocalPlayer()->PlayerController);
	if (IsValid(PlayerController))
	{
		PlayerController->OnScoreboardMenuOpen.AddDynamic(this, &UMatchOverlay::OnScoreboardOpen);
	}
	
}

TArray<FDSScoreboardMatchStats> UMatchOverlay::RecieveMatchStats()
{
	return TArray<FDSScoreboardMatchStats>{};
}

void UMatchOverlay::RefreshMatchStats()
{
	
}

void UMatchOverlay::OnScoreboardOpen(bool bOpen)
{
	// wont need this in super call.
	ADSPlayerController* PlayerController = Cast<ADSPlayerController>(GetOwningPlayer()->GetLocalPlayer()->PlayerController);
	if (IsValid(PlayerController))
	{
		if(ADS_MatchPlayerState* DSPlayerState = PlayerController->GetPlayerState<ADS_MatchPlayerState>(); IsValid(DSPlayerState))
		{
			DSPlayerState->OnScoreboardStatsChangedDelegate.AddDynamic(this, &UMatchOverlay::RefreshMatchStats);
		}
	}
}
