// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "UI/HTTP/HTTPRequestTypes.h"
#include "DS_MatchPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnScoreboardStatsChanged);

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

	UPROPERTY()
	FOnScoreboardStatsChanged OnScoreboardStatsChangedDelegate;
	
	UPROPERTY(ReplicatedUsing  = OnRep_MatchPlayerState)
	FDSScoreboardMatchStats CurrentMatchStats{};
	
	UPROPERTY()
	TObjectPtr<UGameStatsManager> GameStatsManager;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameStatsManager> GameStatsManagerClass;
	
	virtual void OnMatchEnded(const FString& Username);

	UFUNCTION()
	virtual void OnScoreboardOpen(bool bOpen);

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

protected:

	UFUNCTION()
	void OnRep_MatchPlayerState();
	
	virtual void BeginPlay() override;

	void RecordMatchStats(const FDSRecordMatchStatsInput& RecordMatchStatsInput) const;


private:
	



	
};























