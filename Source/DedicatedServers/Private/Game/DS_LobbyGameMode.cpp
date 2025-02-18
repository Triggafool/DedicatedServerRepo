// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/DS_GameInstanceSubsystem.h"
#include "Game/DS_LobbyGameMode.h"
#include "DedicatedServers/DedicatedServers.h"

void ADS_LobbyGameMode::BeginPlay()
{
	Super::BeginPlay();

	InitGameLift();
	
}

void ADS_LobbyGameMode::InitGameLift()
{
	if (UGameInstance* GameInstance = GetGameInstance(); IsValid(GameInstance))
	{
		if (DSGameInstanceSubsystem = GameInstance->GetSubsystem<UDS_GameInstanceSubsystem>(); IsValid(DSGameInstanceSubsystem))
		{
			FServerParameters ServerParameters;
			SetServerParameters(ServerParameters);
			DSGameInstanceSubsystem->InitGameLift(ServerParameters);
		}
	}
}

void ADS_LobbyGameMode::SetServerParameters(FServerParameters& OutServerParameters)
{
	//AuthToken returned from the "aws gamelift get-compute-auth-token" API. Note this will expire and require a new call to the API after 15 minutes.
 	if (FParse::Value(FCommandLine::Get(), TEXT("-authtoken="), OutServerParameters.m_authToken))
 	{
 		UE_LOG(LogDedicatedServers, Log, TEXT("AUTH_TOKEN: %s"), *OutServerParameters.m_authToken)
 	}
 
 	if (FParse::Value(FCommandLine::Get(), TEXT("-awsregion="), OutServerParameters.m_awsRegion))
 	{
 		UE_LOG(LogDedicatedServers, Log, TEXT("AWS_REGION: %s"), *OutServerParameters.m_awsRegion)
 	}
 	
 	if (FParse::Value(FCommandLine::Get(), TEXT("-accesskey="), OutServerParameters.m_accessKey))
 	{
 		UE_LOG(LogDedicatedServers, Log, TEXT("ACCESS_KEY: %s"), *OutServerParameters.m_accessKey)
 	}
 	if (FParse::Value(FCommandLine::Get(), TEXT("-secretkey="), OutServerParameters.m_secretKey))
 	{
 		UE_LOG(LogDedicatedServers, Log, TEXT("SECRET_KEY: % s"), *OutServerParameters.m_secretKey)
 	}
 	if (FParse::Value(FCommandLine::Get(), TEXT("-sessiontoken="), OutServerParameters.m_sessionToken))
 	{
 		UE_LOG(LogDedicatedServers, Log, TEXT("SESSION_TOKEN: %s"), *OutServerParameters.m_sessionToken)
 	}
 
 	//The Host/compute-name of the GameLift Anywhere instance.
 	if (FParse::Value(FCommandLine::Get(), TEXT("-hostid="), OutServerParameters.m_hostId))
 	{
 		UE_LOG(LogDedicatedServers, Log, TEXT("HOST_ID: %s"), *OutServerParameters.m_hostId)
 	}
 
 	//The Anywhere Fleet ID.
 	if (FParse::Value(FCommandLine::Get(), TEXT("-fleetid="), OutServerParameters.m_fleetId))
 	{
 		UE_LOG(LogDedicatedServers, Log, TEXT("FLEET_ID: %s"), *OutServerParameters.m_fleetId)
 	}
 
 	//The WebSocket URL (GameLiftServiceSdkEndpoint).
 	if (FParse::Value(FCommandLine::Get(), TEXT("-websocketurl="), OutServerParameters.m_webSocketUrl))
 	{
 		UE_LOG(LogDedicatedServers, Log, TEXT("WEBSOCKET_URL: %s"), *OutServerParameters.m_webSocketUrl)
 	}
 
 	
 	FString glProcessId = "";
 	if (FParse::Value(FCommandLine::Get(), TEXT("-processId="), glProcessId))
 	{
 		OutServerParameters.m_processId = TCHAR_TO_UTF8(*glProcessId);
 	}
 	else
 	{
 		// If no ProcessId is passed as a command line argument, generate a randomized unique string.
 		FString TimeString = FString::FromInt(std::time(nullptr));
 		FString ProcessId = "ProcessId_" + TimeString;
 		OutServerParameters.m_processId = TCHAR_TO_UTF8(*ProcessId);
 	}
 	//The PID of the running process
 	UE_LOG(LogDedicatedServers, Log, TEXT("PID: %s"), *OutServerParameters.m_processId);
}
