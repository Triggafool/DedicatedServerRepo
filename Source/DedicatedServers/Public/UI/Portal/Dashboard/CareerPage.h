// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CareerPage.generated.h"

class UTextBlock;
class UCareerAchievement;
class UScrollBox;
struct FDSRetrieveMatchStatsResponse;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UCareerPage : public UUserWidget
{
	GENERATED_BODY()

public:

	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCareerAchievement> CareerAchievementClass;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> ScrollBox_Achievements;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Username;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Wins;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Losses;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_StatusMessage;

	UFUNCTION()
	virtual void OnRetrieveMatchStats(const FDSRetrieveMatchStatsResponse& Response);


	UFUNCTION()
	void SetStatusMessage(const FString& Message, bool bShouldResetWidgets);

protected:

	virtual void NativeConstruct() override;

private:


};
