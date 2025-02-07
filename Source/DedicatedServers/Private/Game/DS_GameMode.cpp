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
 		//AuthToken returned from the "aws gamelift get-compute-auth-token" API. Note this will expire and require a new call to the API after 15 minutes.
 	if (FParse::Value(FCommandLine::Get(), TEXT("-authtoken="), OutServerParameters.m_authToken))
 	{
 		UE_LOG(LogDS_ShooterGameMode, Log, TEXT("AUTH_TOKEN: %s"), *OutServerParameters.m_authToken)
 	}
 
 	if (FParse::Value(FCommandLine::Get(), TEXT("-awsregion="), OutServerParameters.m_awsRegion))
 	{
 		UE_LOG(LogDS_ShooterGameMode, Log, TEXT("AWS_REGION: %s"), *OutServerParameters.m_awsRegion)
 	}
 	
 	if (FParse::Value(FCommandLine::Get(), TEXT("-accesskey="), OutServerParameters.m_accessKey))
 	{
 		UE_LOG(LogDS_ShooterGameMode, Log, TEXT("ACCESS_KEY: %s"), *OutServerParameters.m_accessKey)
 	}
 	if (FParse::Value(FCommandLine::Get(), TEXT("-secretkey="), OutServerParameters.m_secretKey))
 	{
 		UE_LOG(LogDS_ShooterGameMode, Log, TEXT("SECRET_KEY: % s"), *OutServerParameters.m_secretKey)
 	}
 	if (FParse::Value(FCommandLine::Get(), TEXT("-sessiontoken="), OutServerParameters.m_sessionToken))
 	{
 		UE_LOG(LogDS_ShooterGameMode, Log, TEXT("SESSION_TOKEN: %s"), *OutServerParameters.m_sessionToken)
 	}
 
 	//The Host/compute-name of the GameLift Anywhere instance.
 	if (FParse::Value(FCommandLine::Get(), TEXT("-hostid="), OutServerParameters.m_hostId))
 	{
 		UE_LOG(LogDS_ShooterGameMode, Log, TEXT("HOST_ID: %s"), *OutServerParameters.m_hostId)
 	}
 
 	//The Anywhere Fleet ID.
 	if (FParse::Value(FCommandLine::Get(), TEXT("-fleetid="), OutServerParameters.m_fleetId))
 	{
 		UE_LOG(LogDS_ShooterGameMode, Log, TEXT("FLEET_ID: %s"), *OutServerParameters.m_fleetId)
 	}
 
 	//The WebSocket URL (GameLiftServiceSdkEndpoint).
 	if (FParse::Value(FCommandLine::Get(), TEXT("-websocketurl="), OutServerParameters.m_webSocketUrl))
 	{
 		UE_LOG(LogDS_ShooterGameMode, Log, TEXT("WEBSOCKET_URL: %s"), *OutServerParameters.m_webSocketUrl)
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
 	UE_LOG(LogDS_ShooterGameMode, Log, TEXT("PID: %s"), *OutServerParameters.m_processId);
 
 }

void ADS_GameMode::ParseCommandLinePoint(int32& OutPort)
{
	TArray<FString> Tokens;
	TArray<FString> CommandLineSwitches;
	FCommandLine::Parse(FCommandLine::Get(), Tokens, CommandLineSwitches);

	for (FString Switch : CommandLineSwitches)
	{
		FString Key;
		FString Value;

		if (Switch.Split("=", &Key, &Value))
		{
			if (Key.Equals(TEXT("port"), ESearchCase::IgnoreCase))
			{
				OutPort = FCString::Atoi(*Value);
				return;
			}
		}
	}
}

void ADS_GameMode::InitGameLift()
{
		UE_LOG(LogDS_ShooterGameMode, Log, TEXT("Initializing the GameLift Server"));
	
	FGameLiftServerSDKModule* GameLiftSdkModule = &FModuleManager::LoadModuleChecked<FGameLiftServerSDKModule>(FName("GameLiftServerSDK"));

	//Define the server parameters for a GameLift Anywhere fleet. These are not needed for a GameLift managed EC2 fleet.
	FServerParameters ServerParameters;
	
	SetServerParameters(ServerParameters);

	//InitSDK establishes a local connection with GameLift's agent to enable further communication.
	//Use InitSDK(serverParameters) for a GameLift Anywhere fleet. 
	//Use InitSDK() for a GameLift managed EC2 fleet.
	GameLiftSdkModule->InitSDK(ServerParameters);

	//Implement callback function onGameSession
    //GameLift sends a game session activation request to the game server
    //and passes a game session object with game properties and other settings.
    //Here is where a game server takes action based on the game session object.
    //When the game server is ready to receive incoming player connections, 
    //it invokes the server SDK call ActivateGameSession().
    auto OnGameSession = [=](Aws::GameLift::Server::Model::GameSession gameSession)
    {
        FString GameSessionId = FString(gameSession.GetGameSessionId());
        UE_LOG(LogDS_ShooterGameMode, Log, TEXT("GameSession Initializing: %s"), *GameSessionId);
        GameLiftSdkModule->ActivateGameSession();
    };

    ProcessParameters.OnStartGameSession.BindLambda(OnGameSession);

    //Implement callback function OnProcessTerminate
    //GameLift invokes this callback before shutting down the instance hosting this game server.
    //It gives the game server a chance to save its state, communicate with services, etc., 
    //and initiate shut down. When the game server is ready to shut down, it invokes the 
    //server SDK call ProcessEnding() to tell GameLift it is shutting down.
    auto OnProcessTerminate = [=]()
    {
        UE_LOG(LogDS_ShooterGameMode, Log, TEXT("Game Server Process is terminating"));
        GameLiftSdkModule->ProcessEnding();
    };

    ProcessParameters.OnTerminate.BindLambda(OnProcessTerminate);

    //Implement callback function OnHealthCheck
    //GameLift invokes this callback approximately every 60 seconds.
    //A game server might want to check the health of dependencies, etc.
    //Then it returns health status true if healthy, false otherwise.
    //The game server must respond within 60 seconds, or GameLift records 'false'.
    //In this example, the game server always reports healthy.
    auto OnHealthCheck = []()
    {
        UE_LOG(LogDS_ShooterGameMode, Log, TEXT("Performing Health Check"));
        return true;
    };

    ProcessParameters.OnHealthCheck.BindLambda(OnHealthCheck);

    //The game server gets ready to report that it is ready to host game sessions
    //and that it will listen on port 7777 for incoming player connections.

	int32 Port = FURL::UrlConfig.DefaultPort;

	ParseCommandLinePoint(Port);
	
	ProcessParameters.port = Port;

    //Here, the game server tells GameLift where to find game session log files.
    //At the end of a game session, GameLift uploads everything in the specified 
    //location and stores it in the cloud for access later.
    TArray<FString> LogFiles;
    LogFiles.Add(TEXT("DedicatedServer/Saved/Logs/FPSTemplate.log"));
    ProcessParameters.logParameters = LogFiles;

    //The game server calls ProcessReady() to tell GameLift it's ready to host game sessions.
    UE_LOG(LogDS_ShooterGameMode, Log, TEXT("Calling Process Ready"));
    GameLiftSdkModule->ProcessReady(ProcessParameters);
}


