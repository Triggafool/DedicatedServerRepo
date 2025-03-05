// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/DS_PlayerCharacter.h"
#include "GameFramework/PlayerState.h"
#include "Scoreboard/ScoreboardInfo.h"
#include "UI/HTTP/HTTPRequestTypes.h"
#include "DS_MatchPlayerState.generated.h"

class AScoreboardController;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnScoreboardStatsChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDisplayNameReplicated, const FString&, DisplayName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScoreboardControllerStatsInitialized, APlayerState*, PlayerState);

struct FDSRecordMatchStatsInput;
class UGameStatsManager;
/**
 *  Class has functionality for sending HTTP requests.
 */
UCLASS()
class DEDICATEDSERVERS_API ADS_MatchPlayerState : public APlayerState
{
	GENERATED_BODY()

public:

	void AddPlayerToScoreboard(AController* NewPlayer);

	UFUNCTION(Client, Reliable)
	void Client_GetUsernameFromSubsystem(APlayerController* PlayerController);

	UFUNCTION(Server, Reliable)
	void Server_SetDisplayUsername(const FString& InDisplayUsername);
	
	bool bAddedToScoreboard = false;

	UPROPERTY(ReplicatedUsing=OnRep_DisplayUsername, VisibleAnywhere)
	FString DisplayUsername{};
	
	ADS_MatchPlayerState();
	
	FScoreboardInfo CurrentScoreboardStats{};
	
	void SetCurrentScoreboardStatsController(AScoreboardController* InScoreboardController);

	UPROPERTY()
	FOnScoreboardControllerStatsInitialized OnScoreboardControllerStatsInitializedDelegate;
	
	UPROPERTY(ReplicatedUsing = OnRep_ScoreboardController)
	TObjectPtr<AScoreboardController> ScoreboardController;
	
	UPROPERTY()
	FOnScoreboardStatsChanged OnScoreboardStatsChangedDelegate;


	UPROPERTY()
	FOnDisplayNameReplicated OnDisplayNameReplicated;
	
	UPROPERTY()
	TObjectPtr<UGameStatsManager> GameStatsManager;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameStatsManager> GameStatsManagerClass;
	
	virtual void OnMatchEnded(const FString& Username);

	UFUNCTION()
	virtual void OnScoreboardOpen(bool bOpen);

	void SetUsernameInStats(AController* PlayerController);

	void OnUserAddedToGame();

	UFUNCTION()
	void OnRep_ScoreboardController();

	UFUNCTION()
	void OnRep_DisplayUsername();

	FString GetDisplayName();
protected:

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual void BeginPlay() override;

	void RecordMatchStats(const FDSRecordMatchStatsInput& RecordMatchStatsInput) const;

};
























