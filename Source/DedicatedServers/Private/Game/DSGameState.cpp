// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/DSGameState.h"
#include "Lobby/LobbyState.h"
#include "Scoreboard/ScoreboardController.h"
#include "Net/UnrealNetwork.h"

ADSGameState::ADSGameState()
{
	bReplicates = true;
	bAlwaysRelevant = true;
}

void ADSGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
 
	DOREPLIFETIME(ADSGameState, LobbyState);
	DOREPLIFETIME(ADSGameState, ScoreboardController);
}

void ADSGameState::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		CreateLobbyState();
		OnLobbyStateInitialized.Broadcast(LobbyState);
	}
	if (HasAuthority())
	{
	CreateScoreboardController();
	}
}

void ADSGameState::CreateLobbyState()
{
	if (UWorld* World = GetWorld(); IsValid(World))
	{
		FActorSpawnParameters SpawnParameters;
		LobbyState = World->SpawnActor<ALobbyState>(
			ALobbyState::StaticClass(),
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			SpawnParameters
		);
		if (IsValid(LobbyState))
		{
			UE_LOG(LogTemp, Warning, TEXT("Lobby State has been created and is Valid."));
			LobbyState->SetOwner(this);
		}
	}
}

void ADSGameState::CreateScoreboardController()
{
	if (UWorld* World = GetWorld(); IsValid(World))
	{
		FActorSpawnParameters SpawnParameters;
		ScoreboardController = World->SpawnActor<AScoreboardController>(
			AScoreboardController::StaticClass(),
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			SpawnParameters
		);

		if (IsValid(ScoreboardController))
		{
			UE_LOG(LogTemp, Warning, TEXT("Scoreboard Controller has been created and is Valid."));
			ScoreboardController->SetOwner(this);
			OnScoreboardControllerInitialized.Broadcast(ScoreboardController);
		}
	}
}



void ADSGameState::OnRep_LobbyState()
{
	OnLobbyStateInitialized.Broadcast(LobbyState);
}

void ADSGameState::OnRep_ScoreboardController()
{
	OnScoreboardControllerInitialized.Broadcast(ScoreboardController);
}












