// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameStatsAPITestWidget.generated.h"

/**
 * 
 */

class UGameStatsManager;
class UTextBlock;
class UButton;

UCLASS()
class DEDICATEDSERVERS_API UGameStatsAPITestWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	virtual void NativeConstruct() override;

	UPROPERTY()
	TObjectPtr<UGameStatsManager> GameStatsManager;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameStatsManager> GameStatsManagerClass;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_TestGameStatsAPI;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_StatusMessage;

	UFUNCTION()
	void SetStatusMessage(const FString& Text, bool bShouldResetWigets);

private:

	UFUNCTION()
	void OnGameStatsButtonClicked();
};
