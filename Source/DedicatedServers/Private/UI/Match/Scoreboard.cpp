// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Match/Scoreboard.h"
#include "Components/ScrollBox.h"
#include "Player/DS_MatchPlayerState.h"
#include "UI/Match/PlayerScoreCard.h"

void UScoreboard::PopulateScoreboard(const TArray<FScoreboardInfo>& ScoreboardPlayers)
{
	check(PlayerScoreCardClass)
	
	ScrollBox_Scoreboard->ClearChildren();
	
	for (const FScoreboardInfo& Player : ScoreboardPlayers)
	{
		// Perform a sort function for the highest kills to least kills so that leaders are on top.
		// Make an array from the scoreboard players, that is sorted.. and use this array instead here for the loop.
		// then proceed to make a label for each.
		CreateAndAddPlayerLabel(Player);
	}
	
}

void UScoreboard::CreateAndAddPlayerLabel(const FScoreboardInfo& PlayerInfo)
{

	//if (FindPlayerScoreCard(PlayerInfo.Username)) return;
	
	UPlayerScoreCard* PlayerScoreCard = CreateWidget<UPlayerScoreCard>(this, PlayerScoreCardClass);
	if (!IsValid(PlayerScoreCard)) return;
	PlayerScoreCard->SetTextBlocks(PlayerInfo);
	ScrollBox_Scoreboard->AddChild(PlayerScoreCard);
}

void UScoreboard::OnPlayerRemoved(const FScoreboardInfo& PlayerInfo)
{
	if (UPlayerScoreCard* PlayerScoreCard = FindPlayerScoreCard(PlayerInfo.TestingName))
	{
		ScrollBox_Scoreboard->RemoveChild(PlayerScoreCard);
	}
}

void UScoreboard::RefreshScoreboard(const FScoreboardInfoArray& PlayerScoreboardInfo)
{
	PopulateScoreboard(PlayerScoreboardInfo.Players);
}	

void UScoreboard::NativeOnInitialized()
{
	Super::NativeOnInitialized();

}

UPlayerScoreCard* UScoreboard::FindPlayerScoreCard(const FString& Username)
{
	for (UWidget* Child : ScrollBox_Scoreboard->GetAllChildren())
	{
		UPlayerScoreCard* PlayerScoreCard = Cast<UPlayerScoreCard>(Child);
		if (IsValid(PlayerScoreCard) && PlayerScoreCard->GetUsername() == Username)
		{
			return PlayerScoreCard;
		}
	}

	return nullptr;
}
