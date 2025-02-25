// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/APITest/GameStatsAPITestWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Player/DSLocalPlayerSubSystem.h"
#include "UI/HTTP/HTTPRequestTypes.h"
#include "UI/GameStats/GameStatsManager.h"


void UGameStatsAPITestWidget::NativeConstruct()
{
	Super::NativeConstruct();

	GameStatsManager = NewObject<UGameStatsManager>(this, GameStatsManagerClass);
	Button_TestGameStatsAPI->OnClicked.AddDynamic(this, &UGameStatsAPITestWidget::OnGameStatsButtonClicked);
}

void UGameStatsAPITestWidget::SetStatusMessage(const FString& Text, bool bShouldResetWigets)
{
	TextBlock_StatusMessage->SetText(FText::FromString(Text));

	if(bShouldResetWigets)
	{
		TextBlock_StatusMessage->SetIsEnabled(true);
	}
}

void UGameStatsAPITestWidget::OnGameStatsButtonClicked()
{
	check (GameStatsManager);
	const UDSLocalPlayerSubSystem* LocalPlayerSubSystem = GetOwningLocalPlayer()->GetSubsystem<UDSLocalPlayerSubSystem>();
	if (!IsValid(LocalPlayerSubSystem)) return;

	const FString Username = LocalPlayerSubSystem->Username;
	FDSRecordMatchStatsInput RecordMatchStatsInput;
	RecordMatchStatsInput.username = Username;
	RecordMatchStatsInput.matchStats.hits = 2;
	RecordMatchStatsInput.matchStats.highestStreak = 10;
	RecordMatchStatsInput.matchStats.matchWins = 1;
	RecordMatchStatsInput.matchStats.matchLosses = 1;
	RecordMatchStatsInput.matchStats.scoredElims = 45;
	RecordMatchStatsInput.matchStats.gotFirstBlood = 1;
	RecordMatchStatsInput.matchStats.showstopperElims = 2;
	RecordMatchStatsInput.matchStats.revengeElims = 9;
	
	GameStatsManager->RecordMatchStats(RecordMatchStatsInput);
}
