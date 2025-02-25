// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Career/ShooterCareerPage.h"

#include "Components/ScrollBox.h"
#include "Data/SpecialElimData.h"
#include "ShooterTypes/ShooterTypes.h"
#include "UI/Portal/Dashboard/CareerAchievement.h"
#include "UI/HTTP/HTTPRequestTypes.h"

void UShooterCareerPage::OnRetrieveMatchStats(const FDSRetrieveMatchStatsResponse& Response)
{
	Super::OnRetrieveMatchStats(Response);

	ScrollBox_Achievements->ClearChildren();

	TMap<ESpecialElimType, int32> AchievementData;
	
	if (Response.hits > 0) AchievementData.Emplace(ESpecialElimType::Hits, Response.hits);
	if (Response.misses > 0) AchievementData.Emplace(ESpecialElimType::Misses, Response.misses);
	if (Response.scoredElims > 0) AchievementData.Emplace(ESpecialElimType::ScoredElims, Response.scoredElims);
	if (Response.defeats > 0) AchievementData.Emplace(ESpecialElimType::Defeats, Response.defeats);
	if (Response.highestStreak > 0) AchievementData.Emplace(ESpecialElimType::Streak, Response.highestStreak);
	if (Response.dethroneElims > 0) AchievementData.Emplace(ESpecialElimType::Dethrone, Response.dethroneElims);
	if (Response.gotFirstBlood > 0) AchievementData.Emplace(ESpecialElimType::FirstBlood, Response.gotFirstBlood);
	if (Response.revengeElims > 0) AchievementData.Emplace(ESpecialElimType::Revenge, Response.revengeElims);
	if (Response.showstopperElims > 0) AchievementData.Emplace(ESpecialElimType::Showstopper, Response.showstopperElims);
	if (Response.headShotElims > 0) AchievementData.Emplace(ESpecialElimType::Headshot, Response.headShotElims);

	check (SpecialElimData)

	for (const TPair<ESpecialElimType, int32> Pair : AchievementData)
	{
		const FString& CareerAchievementName = SpecialElimData->SpecialElimInfo.FindChecked(Pair.Key).CareerPageAchievementName;
		UTexture2D* Icon = SpecialElimData->SpecialElimInfo.FindChecked(Pair.Key).ElimIcon;

		UCareerAchievement* CareerAchievement = CreateWidget<UCareerAchievement>(this, CareerAchievementMakerClass);

		if (IsValid(CareerAchievement))
		{
			CareerAchievement->SetAchievementText(CareerAchievementName, Pair.Value);
			if (Icon)
			{
				CareerAchievement->SetAchievementIcon(Icon);
			}

			ScrollBox_Achievements->AddChild(CareerAchievement);
		}
	}
}
