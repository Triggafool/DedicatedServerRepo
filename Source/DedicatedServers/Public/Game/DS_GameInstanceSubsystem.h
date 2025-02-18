// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameLiftServerSDK.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DS_GameInstanceSubsystem.generated.h"

struct FServerParameters;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UDS_GameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	UDS_GameInstanceSubsystem();
	
	UPROPERTY(BlueprintReadOnly)
	bool bGameLiftInitialized;

	
	void InitGameLift(const FServerParameters& ServerParameters);

	
private:
	UPROPERTY()
	TObjectPtr<UDS_GameInstanceSubsystem> DSGameInstanceSubsystem;
	// Process Parameters needs to remain in scope for the lifetime of the app
	FProcessParameters ProcessParameters;

	void ParseCommandLinePoint(int32& OutPort);
	
};
