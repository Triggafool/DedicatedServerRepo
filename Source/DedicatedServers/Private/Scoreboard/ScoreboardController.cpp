// Fill out your copyright notice in the Description page of Project Settings.

#include "Scoreboard/ScoreboardController.h"
#include "Net/UnrealNetwork.h"
#include "Player/DS_MatchPlayerState.h"

AScoreboardController::AScoreboardController()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	bAlwaysRelevant = true;
	
}

TArray<FScoreboardInfo> AScoreboardController::GetScoreboardPlayers() const
{
	return ScoreboardInfoArray.Players;
}

bool AScoreboardController::PlayerExistsInPlayerArray(const FString& Username)
{
	if (ScoreboardInfoArray.FindPlayer(Username))
	{
		return true;
	}
	return false;
}

void AScoreboardController::SetUsernameInInfo(const FScoreboardInfo& ScoreboardInfo, const FString& Username)
{
	ScoreboardInfoArray.SetUsername(ScoreboardInfo, Username);
}

void AScoreboardController::ModifyKills(FScoreboardInfo& ScoreboardInfo , int32 Amount)
{
	ScoreboardInfoArray.ModifyKills(ScoreboardInfo, Amount);
}

void AScoreboardController::ModifyDeaths(FScoreboardInfo& ScoreboardInfo, int32 Amount)
{
	ScoreboardInfoArray.ModifyDeaths(ScoreboardInfo, Amount);
}

void AScoreboardController::ModifyHeadShots(FScoreboardInfo& ScoreboardInfo, int32 Amount)
{
	ScoreboardInfoArray.ModifyHeadShots(ScoreboardInfo, Amount);
}

void AScoreboardController::AddPlayerInfo(const FScoreboardInfo& ScoreboardInfo)
{
	ScoreboardInfoArray.AddPlayer(ScoreboardInfo);
	OnScoreboardPlayerAdded.Broadcast(ScoreboardInfo);
}

void AScoreboardController::RemovePlayerInfo(const FString& Username)
{
	ScoreboardInfoArray.RemovePlayer(Username);
	OnScoreboardPlayerRemoved.Broadcast(Username);
}

void AScoreboardController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AScoreboardController, ScoreboardInfoArray);
	
}

void AScoreboardController::OnRep_ScoreboardInfoChange()
{
	FScoreboardInfoDelta Delta = ComputeFScoreboardInfoDelta(LastScoreboardInfoArray.Players, ScoreboardInfoArray.Players);

	for (const auto& ScoreboardInfo : Delta.ScoreboardInfoPlayersAdded)
	{
		OnScoreboardPlayerAdded.Broadcast(ScoreboardInfo);
	}

	for (const auto& ScoreboardInfo : Delta.ScoreboardInfoPlayersRemoved)
	{
		OnScoreboardPlayerRemoved.Broadcast(ScoreboardInfo);
	}
	
	OnScoreboardStatModify.Broadcast(ScoreboardInfoArray);
	
	LastScoreboardInfoArray = ScoreboardInfoArray;
}

FScoreboardInfoDelta AScoreboardController::ComputeFScoreboardInfoDelta(const TArray<FScoreboardInfo>& OldArray, const TArray<FScoreboardInfo>& NewArray)
{
	FScoreboardInfoDelta Delta;
	
	TMap<FString, const FScoreboardInfo*> OldMap;
	TMap<FString, const FScoreboardInfo*> NewMap;

	for (const auto& ScoreboardInfo : OldArray)
	{
		OldMap.Add(ScoreboardInfo.Username, &ScoreboardInfo);
	}

	for (const auto& ScoreboardInfo : NewArray)
	{
		NewMap.Add(ScoreboardInfo.Username, &ScoreboardInfo);
	}

	
	for (const auto& OldScoreboardInfo : OldArray)
	{
		if (!NewMap.Contains(OldScoreboardInfo.Username))
		{
			Delta.ScoreboardInfoPlayersRemoved.Add(OldScoreboardInfo.Username);
		}
	}

	for (const auto& NewScoreboardInfo : NewArray)
	{
		if (!OldMap.Contains(NewScoreboardInfo.Username))
		{
			Delta.ScoreboardInfoPlayersAdded.Add(NewScoreboardInfo.Username);
		}
	}
	
	return Delta;
}

