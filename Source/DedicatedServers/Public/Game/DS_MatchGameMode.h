// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "DS_GameModeBase.h"
#include "Types/DSTypes.h"
#include "DS_MatchGameMode.generated.h"

class AScoreboardController;
class UGameStatsManager;
class ADSGameState;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API ADS_MatchGameMode : public ADS_GameModeBase
{
	GENERATED_BODY()

public:
	ADS_MatchGameMode();
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameStatsManager> GameStatsManagerClass;

	virtual void PostLogin(APlayerController* NewPlayer) override;
	
	UPROPERTY()
	EMatchStatus MatchStatus;

	virtual void Logout(AController* Exiting) override;
	virtual void InitSeamlessTravelPlayer(AController* NewController) override;
	
protected:
	virtual void BeginPlay() override;
	virtual void OnCountdownTimerFinished(ECountdownTimerType Type) override;
	void SetClientInputEnabled(bool bEnabled);
	void EndMatchForPlayerStates();
	virtual void OnMatchEnded();
	void UpdateLeaderboard(const TArray<FString>& LeaderboardNames);
	
	UFUNCTION()
	void OnLeaderboardUpdated();
	
	UPROPERTY(EditDefaultsOnly)
	FCountdownTimerHandle PreMatchTimer;

	UPROPERTY(EditDefaultsOnly)
	FCountdownTimerHandle MatchTimer;

	UPROPERTY(EditDefaultsOnly)
	FCountdownTimerHandle PostMatchTimer;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> LobbyMap;

private:

	UPROPERTY()
	TObjectPtr<UGameStatsManager> GameStatsManager;

	void SetUsername(APlayerController* PlayerController);
};
