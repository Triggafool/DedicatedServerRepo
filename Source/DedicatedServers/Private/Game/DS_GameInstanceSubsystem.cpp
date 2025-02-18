// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/DS_GameInstanceSubsystem.h"
#include "DedicatedServers/DedicatedServers.h"

UDS_GameInstanceSubsystem::UDS_GameInstanceSubsystem()
{
	bGameLiftInitialized = false;
}

void UDS_GameInstanceSubsystem::InitGameLift(const FServerParameters& ServerParameters)
{
	if (bGameLiftInitialized) return;
	
#if WITH_GAMELIFT
	UE_LOG(LogDedicatedServers, Log, TEXT("Initializing the GameLift Server"));
	
	FGameLiftServerSDKModule* GameLiftSdkModule = &FModuleManager::LoadModuleChecked<FGameLiftServerSDKModule>(FName("GameLiftServerSDK"));

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
        UE_LOG(LogDedicatedServers, Log, TEXT("GameSession Initializing: %s"), *GameSessionId);
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
        UE_LOG(LogDedicatedServers, Log, TEXT("Game Server Process is terminating"));
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
        UE_LOG(LogDedicatedServers, Log, TEXT("Performing Health Check"));
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
    UE_LOG(LogDedicatedServers, Log, TEXT("Calling Process Ready"));
    GameLiftSdkModule->ProcessReady(ProcessParameters);
	
#endif

	bGameLiftInitialized = true;
	
}

void UDS_GameInstanceSubsystem::ParseCommandLinePoint(int32& OutPort)
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
			if (Key.Equals(TEXT("port"), ESearchCase::IgnoreCase) && Value.IsNumeric())
			{
				LexFromString(OutPort, *Value);
				return;
			}
		}
	}
}
