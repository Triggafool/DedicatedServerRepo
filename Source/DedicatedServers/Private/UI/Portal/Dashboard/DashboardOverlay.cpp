// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Portal/Dashboard/DashboardOverlay.h"
#include "UI/Portal/Dashboard/GamePage.h"
#include "UI/Portal/Dashboard/CareerPage.h"
#include "UI/Portal/Dashboard/LeaderboardPage.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "UI/GameStats/GameStatsManager.h"

void UDashboardOverlay::NativeConstruct()
{
	Super::NativeConstruct();

	check(GamePage);
	check(CareerPage);
	check(LeaderboardPage);
	check(GameStatsManagerClass)

	GameStatsManager = NewObject<UGameStatsManager>(this, GameStatsManagerClass);

	GameStatsManager->OnRetrieveMatchStatsResponse.AddDynamic(CareerPage, &UCareerPage::OnRetrieveMatchStats);
	GameStatsManager->RetrieveGameStatsStatusMessage.AddDynamic(CareerPage, &UCareerPage::SetStatusMessage);

	Button_GamePage->OnClicked.AddDynamic(this, &UDashboardOverlay::ShowGamePage);
	Button_CareerPage->OnClicked.AddDynamic(this, &UDashboardOverlay::ShowCareerPage);
	Button_LeaderboardPage->OnClicked.AddDynamic(this, &UDashboardOverlay::ShowLeaderboardPage);

	ShowGamePage();
	
}

void UDashboardOverlay::ShowGamePage()
{
	WidgetSwitcher->SetActiveWidget(GamePage);
	EnableButtons();
	Button_GamePage->SetIsEnabled(false);
}
void UDashboardOverlay::ShowCareerPage()
{
	WidgetSwitcher->SetActiveWidget(CareerPage);
	EnableButtons();
	Button_CareerPage->SetIsEnabled(false);
	GameStatsManager->RetrieveMatchStats();
}

void UDashboardOverlay::ShowLeaderboardPage()
{
	WidgetSwitcher->SetActiveWidget(LeaderboardPage);
	EnableButtons();
	Button_LeaderboardPage->SetIsEnabled(false);
}

void UDashboardOverlay::EnableButtons()
{
	Button_GamePage->SetIsEnabled(true);
	Button_CareerPage->SetIsEnabled(true);
	Button_LeaderboardPage->SetIsEnabled(true);
}