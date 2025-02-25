// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DashboardOverlay.generated.h"

class UGameStatsManager;
class UWidgetSwitcher;
class UButton;
class ULeaderboardPage;
class UGamePage;
class UCareerPage;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UDashboardOverlay : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameStatsManager> GameStatsManagerClass;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> WidgetSwitcher;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCareerPage> CareerPage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UGamePage> GamePage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<ULeaderboardPage> LeaderboardPage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_GamePage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_LeaderboardPage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_CareerPage;

protected:
	virtual void NativeConstruct() override;

private:

	UPROPERTY()
	TObjectPtr<UGameStatsManager> GameStatsManager;
	
	UFUNCTION()
	void ShowGamePage();

	UFUNCTION()
	void ShowLeaderboardPage();

	UFUNCTION()
	void ShowCareerPage();

	void EnableButtons();
	
};


























