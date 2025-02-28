// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Match/PlayerScoreCard.h"

#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HTTP/HTTPRequestTypes.h"


void UPlayerScoreCard::SetTextBlocks(const FDSScoreboardMatchStats& Stats)
{

	float TotalShots = Stats.matchStats.hits + Stats.matchStats.misses;
	float Accurracy = (Stats.matchStats.hits / TotalShots) * 100.f;
		
	TextBlock_Username->SetText(FText::FromString(Stats.Username));
	TextBlock_Elims->SetText(FText::AsNumber(Stats.matchStats.scoredElims));
	TextBlock_Deaths->SetText(FText::AsNumber(Stats.matchStats.defeats));
	TextBlock_Accuracy->SetText(FText::AsNumber(Accurracy));
}
