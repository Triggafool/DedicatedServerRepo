// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Match/Scoreboard.h"
#include "Components/ScrollBox.h"
#include "Player/DS_MatchPlayerState.h"
#include "UI/Match/PlayerScoreCard.h"

void UScoreboard::PopulateScoreboard(TArray<TObjectPtr<APlayerState>>& Players)
{
	check(PlayerScoreCardClass)
	
	ScrollBox_Scoreboard->ClearChildren();

	for (TObjectPtr<APlayerState>& MatchPlayer : Players)
	{
		if (const ADS_MatchPlayerState* DSMatchPlayer = Cast<ADS_MatchPlayerState>(MatchPlayer); IsValid(DSMatchPlayer))
		{
			UPlayerScoreCard* PlayerScoreCard = CreateWidget<UPlayerScoreCard>(this, PlayerScoreCardClass);
			PlayerScoreCard->SetTextBlocks(DSMatchPlayer->CurrentMatchStats);
			ScrollBox_Scoreboard->AddChild(PlayerScoreCard);
		}
	}
	
}
