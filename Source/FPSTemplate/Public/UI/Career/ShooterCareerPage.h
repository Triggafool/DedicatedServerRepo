// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Portal/Dashboard/CareerPage.h"
#include "ShooterCareerPage.generated.h"

class USpecialElimData;
class UCareerAchievement;
/**
 * 
 */
struct FDSRetrieveMatchStatsResponse;
UCLASS()
class FPSTEMPLATE_API UShooterCareerPage : public UCareerPage
{
	GENERATED_BODY()

public:

	virtual void OnRetrieveMatchStats(const FDSRetrieveMatchStatsResponse& Response) override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCareerAchievement> CareerAchievementMakerClass;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USpecialElimData> SpecialElimData;
};
