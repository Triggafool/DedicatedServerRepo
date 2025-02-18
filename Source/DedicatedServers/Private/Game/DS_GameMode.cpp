// Fill out your copyright notice in the Description page of Project Settings.


#include "DedicatedServers/Public/Game/DS_GameMode.h"

DEFINE_LOG_CATEGORY(LogDS_ShooterGameMode);

void ADS_GameMode::BeginPlay()
{
	Super::BeginPlay();

	#if WITH_GAMELIFT
	InitGameLift();
	#endif
	
}

void ADS_GameMode::SetServerParameters(FServerParameters& OutServerParameters)
 {

 
 }

void ADS_GameMode::ParseCommandLinePoint(int32& OutPort)
{

}

void ADS_GameMode::InitGameLift()
{

	//

	
}


