// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Match/MatchOverlay.h"
#include "Components/ScrollBox.h"
#include "Game/DSGameState.h"
#include "Scoreboard/ScoreboardController.h"
#include "Player/DSPlayerController.h"
#include "Player/DS_MatchPlayerState.h"
#include "UI/Match/Scoreboard.h"

void UMatchOverlay::NativeConstruct()
{
	Super::NativeConstruct();

	ADSPlayerController* PlayerController = Cast<ADSPlayerController>(GetOwningPlayer()->GetLocalPlayer()->PlayerController);
	if (IsValid(PlayerController))
	{
		PlayerController->OnScoreboardMenuOpen.AddDynamic(this, &UMatchOverlay::OnScoreboardOpen);
	}
	ADSGameState* DSGameState = GetWorld()->GetGameState<ADSGameState>();
	if (!IsValid(DSGameState)) return;

	if (IsValid(DSGameState->ScoreboardController))
	{
		OnScoreboardControllerInitiallized(DSGameState->ScoreboardController);
	}
	else
	{
		DSGameState->OnScoreboardControllerInitialized.AddDynamic(this, &UMatchOverlay::OnScoreboardControllerInitiallized);
	}
}

void UMatchOverlay::OnScoreboardControllerInitiallized(AScoreboardController* ScoreboardController)
{
	if (!IsValid(ScoreboardController)) return;

}

void UMatchOverlay::UpdateScoreboardPlayerInfo(AScoreboardController* ScoreboardController)
{
	Scoreboard->ScrollBox_Scoreboard->ClearChildren();
	Scoreboard->PopulateScoreboard(ScoreboardController->GetScoreboardPlayers());
}

void UMatchOverlay::GetPlayerStatesAndAddToArray(AScoreboardController* ScoreboardController)
{
	ADSGameState* DSGameState = GetWorld()->GetGameState<ADSGameState>();

	if (IsValid(DSGameState))
	{
		for (auto& Player : DSGameState->PlayerArray)
		{
			ADS_MatchPlayerState* DSPlayerState = Cast<ADS_MatchPlayerState>(Player);
			if (!IsValid(DSPlayerState)) return;

			UE_LOG(LogTemp, Warning, TEXT("Valid Player State for Adding to Scoreboard ARraY)"));
			ScoreboardController->AddPlayerInfo(DSPlayerState->CurrentScoreboardStats);
		}
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
	ADSPlayerController* PlayerController = Cast<ADSPlayerController>(GetOwningPlayer()->GetLocalPlayer()->PlayerController);
	if (IsValid(PlayerController))
	{
		if(ADS_MatchPlayerState* DSPlayerState = PlayerController->GetPlayerState<ADS_MatchPlayerState>(); IsValid(DSPlayerState))
		{
			//DSPlayerState->OnScoreboardStatsChangedDelegate.AddDynamic(this, &UMatchOverlay::RefreshMatchStats);
		}
	}

	if (bOpen)
	{
		ADSGameState* DSGameState = GetWorld()->GetGameState<ADSGameState>();
		if (!IsValid(DSGameState)) return;
			Scoreboard = CreateWidget<UScoreboard>(this, ScoreboardClass);
			if (IsValid(Scoreboard))
			{
				UpdateScoreboardPlayerInfo(DSGameState->ScoreboardController);

				if (!DSGameState->ScoreboardController->OnScoreboardPlayerRemoved.IsBound() && !DSGameState->ScoreboardController->OnScoreboardPlayerAdded.IsBound())
				{
					DSGameState->ScoreboardController->OnScoreboardPlayerAdded.AddDynamic(Scoreboard, &UScoreboard::CreateAndAddPlayerLabel);
					DSGameState->ScoreboardController->OnScoreboardPlayerRemoved.AddDynamic(Scoreboard, &UScoreboard::OnPlayerRemoved);
					DSGameState->ScoreboardController->OnScoreboardStatModify.AddDynamic(Scoreboard,  &UScoreboard::RefreshScoreboard);
				}
				
				Scoreboard->AddToViewport();
			}
			
			Scoreboard->PopulateScoreboard(DSGameState->ScoreboardController->GetScoreboardPlayers());
	}
	else
	{
		if (IsValid(Scoreboard))
		{
			ADSGameState* DSGameState = GetWorld()->GetGameState<ADSGameState>();
			if (!IsValid(DSGameState)) return;
			if (DSGameState->ScoreboardController->OnScoreboardPlayerRemoved.IsBound() || DSGameState->ScoreboardController->OnScoreboardPlayerAdded.IsBound() || DSGameState->ScoreboardController->OnScoreboardStatModify.IsBound())
			{
				DSGameState->ScoreboardController->OnScoreboardPlayerAdded.RemoveAll(Scoreboard);
				DSGameState->ScoreboardController->OnScoreboardPlayerRemoved.RemoveAll(Scoreboard);
				DSGameState->ScoreboardController->OnScoreboardStatModify.RemoveAll(Scoreboard);
			}
			Scoreboard->RemoveFromParent();
		}
	}
	
}
