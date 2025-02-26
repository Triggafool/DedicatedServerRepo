// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Portal/Dashboard/LeaderboardPage.h"

#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "UI/HTTP/HTTPRequestTypes.h"
#include "UI/Portal/Dashboard/LeaderboardCard.h"

void ULeaderboardPage::PopulateLeaderboard(TArray<FDSLeaderboardItem>& Leaderboard)
{

	ScrollBox_Leaderboard->ClearChildren();

	CalculateLeaderboardPlaces(Leaderboard);
	
	for (const FDSLeaderboardItem& Item : Leaderboard)
	{

		check (LeaderboardCardClass)
		
		ULeaderboardCard* LeaderboardCard = CreateWidget<ULeaderboardCard>(this, LeaderboardCardClass);

		if (IsValid(LeaderboardCard))
		{
			LeaderboardCard->SetPlayerInfo(Item.username, Item.matchWins, Item.place);
			ScrollBox_Leaderboard->AddChild(LeaderboardCard);
		}
	}
}

void ULeaderboardPage::SetStatusMessage(const FString& Message, bool bShouldResetWidgets)
{
	TextBlock_StatusMessage->SetText(FText::FromString(Message));
}

void ULeaderboardPage::CalculateLeaderboardPlaces(TArray<FDSLeaderboardItem>& OutLeaderboard)
{

	OutLeaderboard.Sort([](const FDSLeaderboardItem& A, const FDSLeaderboardItem& B)
	{
		return A.matchWins > B.matchWins;
	});

	// Assign places base on wins, account for ties.

	int32 CurrentRank = 1;

	for (int32 i = 0; i < OutLeaderboard.Num(); ++i)
	{
		if (i > 0 && OutLeaderboard[i].matchWins == OutLeaderboard[i - 1].matchWins)
		{
			// if Wins are the same as the previous item, they sahre the  same place
			OutLeaderboard[i].place = OutLeaderboard[i - 1].place;
		}
		else
		{
			// if wins are different, assign the current rank as the place.
			OutLeaderboard[i].place = CurrentRank;
		}
		// Increment rank for next rank
		CurrentRank++;
	}
	
}
