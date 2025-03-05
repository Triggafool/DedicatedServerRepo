// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Match/PlayerScoreCard.h"
#include "Components/TextBlock.h"
#include "Scoreboard/ScoreboardInfo.h"

void UPlayerScoreCard::SetTextBlocks(const FScoreboardInfo& Info)
{
	TextBlock_Username->SetText(FText::FromString(Info.Username));
	TextBlock_Elims->SetText(FText::AsNumber(Info.Kills));
	TextBlock_Deaths->SetText(FText::AsNumber(Info.Deaths));
	TextBlock_HeadShots->SetText(FText::AsNumber(Info.HeadShots));
}

FString UPlayerScoreCard::GetUsername() const
{
	return TextBlock_Username->GetText().ToString();
}
