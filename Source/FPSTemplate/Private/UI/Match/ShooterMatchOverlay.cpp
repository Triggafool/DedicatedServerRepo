// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Match/ShooterMatchOverlay.h"
#include "Game/MatchGameState.h"
#include "UI/Match/Scoreboard.h"

void UShooterMatchOverlay::OnScoreboardOpen(bool bOpen)
{
	Super::OnScoreboardOpen(bOpen);
	check (ScoreboardClass)
	
}

void UShooterMatchOverlay::RefreshMatchStats()
{
	Super::RefreshMatchStats();
	
}
