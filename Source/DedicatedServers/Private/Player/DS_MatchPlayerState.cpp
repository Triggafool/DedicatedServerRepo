// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DS_MatchPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "Player/DSLocalPlayerSubSystem.h"
#include "Player/DSPlayerController.h"
#include "UI/GameStats/GameStatsManager.h"

void ADS_MatchPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(ADS_MatchPlayerState, CurrentMatchStats, COND_None, REPNOTIFY_Always);
}

void ADS_MatchPlayerState::OnRep_MatchPlayerState()
{
	OnScoreboardStatsChangedDelegate.Broadcast();
}

void ADS_MatchPlayerState::OnMatchEnded(const FString& Username)
{
	
}

void ADS_MatchPlayerState::OnScoreboardOpen(bool bOpen)
{
	
}

void ADS_MatchPlayerState::BeginPlay()
{
	Super::BeginPlay();

	GameStatsManager = NewObject<UGameStatsManager>(this, GameStatsManagerClass);

	ADSPlayerController* DSPlayerController = Cast<ADSPlayerController>(GetPlayerController());
	if (IsValid(DSPlayerController))
	{
		DSPlayerController->OnScoreboardMenuOpen.AddDynamic(this, &ADS_MatchPlayerState::OnScoreboardOpen);

		if (DSPlayerController->Username.IsEmpty())
		{
			UE_LOG(LogTemp, Warning, TEXT("Username empty in player controller"));
		}
		else
		{
			CurrentMatchStats.Username = DSPlayerController->Username;
		}
		
	}

}

void ADS_MatchPlayerState::RecordMatchStats(const FDSRecordMatchStatsInput& RecordMatchStatsInput) const
{
	check(IsValid(GameStatsManager))
	GameStatsManager->RecordMatchStats(RecordMatchStatsInput);

	
}
