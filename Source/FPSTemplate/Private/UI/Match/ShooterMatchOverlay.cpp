// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Match/ShooterMatchOverlay.h"
#include "Game/MatchGameState.h"
#include "UI/Match/Scoreboard.h"

void UShooterMatchOverlay::OnScoreboardOpen(bool bOpen)
{
	Super::OnScoreboardOpen(bOpen);
	check (ScoreboardClass)
	
	if (bOpen)
	{
		AMatchGameState* ShooterGameState = GetWorld()->GetGameState<AMatchGameState>();
		if (IsValid(ShooterGameState))
		{
			Scoreboard = CreateWidget<UScoreboard>(this, ScoreboardClass);
			if (IsValid(Scoreboard))
			{
				Scoreboard->AddToViewport();
			}
			Scoreboard->PopulateScoreboard(ShooterGameState->PlayerArray);
		}
		
	}
	else
	{
		if (IsValid(Scoreboard))
		{
			Scoreboard->RemoveFromParent();
		}
	}
}

void UShooterMatchOverlay::RefreshMatchStats()
{
	Super::RefreshMatchStats();

	if (IsValid(Scoreboard))
	{
		AMatchGameState* ShooterGameState = GetWorld()->GetGameState<AMatchGameState>();
		if (IsValid(ShooterGameState))
		{
			Scoreboard->PopulateScoreboard(ShooterGameState->PlayerArray);
		}
	}
	
}
