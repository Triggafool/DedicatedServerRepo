// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DS_MatchPlayerState.h"
#include "Game/DSGameState.h"
#include "Net/UnrealNetwork.h"
#include "Player/DSLocalPlayerSubSystem.h"
#include "Player/DSPlayerController.h"
#include "Scoreboard/ScoreboardController.h"
#include "UI/GameStats/GameStatsManager.h"

ADS_MatchPlayerState::ADS_MatchPlayerState()
{
	DisplayUsername = FString("");
}

void ADS_MatchPlayerState::Server_SetDisplayUsername_Implementation(const FString& InDisplayUsername)
{
	DisplayUsername = InDisplayUsername;
	if (IsValid(ScoreboardController))
	{
		ScoreboardController->SetUsernameInInfo(CurrentScoreboardStats, DisplayUsername);
	}
}

void ADS_MatchPlayerState::SetCurrentScoreboardStatsController(AScoreboardController* InScoreboardController)
{
	ScoreboardController = InScoreboardController;
	
}
void ADS_MatchPlayerState::SetUsernameInStats(AController* PlayerController)
{
	ADSPlayerController* DSPlayerController = Cast<ADSPlayerController>(PlayerController);
	Client_GetUsernameFromSubsystem(DSPlayerController);
}

void ADS_MatchPlayerState::OnUserAddedToGame()
{
	if (IsValid(ScoreboardController))
	{
		
	}
}

void ADS_MatchPlayerState::OnRep_ScoreboardController()
{
}

void ADS_MatchPlayerState::OnRep_DisplayUsername()
{
	OnDisplayNameReplicated.Broadcast(DisplayUsername);
}

FString ADS_MatchPlayerState::GetDisplayName()
{
	return DisplayUsername;
}

void ADS_MatchPlayerState::Client_GetUsernameFromSubsystem_Implementation(APlayerController* PlayerController)
{
	if (DisplayUsername.IsEmpty())
	{
		APlayerController* LocalPlayerController = PlayerController;
		if (LocalPlayerController)
		{
			ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(LocalPlayerController->Player);
			if (LocalPlayer)
			{
				UDSLocalPlayerSubSystem* DSLocalPlayerSubSystem = LocalPlayer->GetSubsystem<UDSLocalPlayerSubSystem>();
				if (IsValid(DSLocalPlayerSubSystem))
				{
					DisplayUsername = DSLocalPlayerSubSystem->Username;
					Server_SetDisplayUsername(DisplayUsername);
				}
			}
		}
	}
}

void ADS_MatchPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ADS_MatchPlayerState, ScoreboardController);
	DOREPLIFETIME_CONDITION_NOTIFY(ADS_MatchPlayerState, DisplayUsername, COND_None, REPNOTIFY_Always);

}

void ADS_MatchPlayerState::OnMatchEnded(const FString& Username)
{
	
}

void ADS_MatchPlayerState::OnScoreboardOpen(bool bOpen)
{
	OnScoreboardStatsChangedDelegate.Broadcast();

}

void ADS_MatchPlayerState::BeginPlay()
{
	Super::BeginPlay();
	
	GameStatsManager = NewObject<UGameStatsManager>(this, GameStatsManagerClass);

	ADSPlayerController* DSPlayerController = Cast<ADSPlayerController>(GetPlayerController());
	if (IsValid(DSPlayerController))
	{
		DSPlayerController->OnScoreboardMenuOpen.AddDynamic(this, &ADS_MatchPlayerState::OnScoreboardOpen);
	}
	
	ADSGameState* DSGameState = Cast<ADSGameState>(GetWorld()->GetGameState());
	if (IsValid(DSGameState))
	{
		ScoreboardController = DSGameState->ScoreboardController;
	}
	
	OnScoreboardControllerStatsInitializedDelegate.Broadcast(this);
}

void ADS_MatchPlayerState::RecordMatchStats(const FDSRecordMatchStatsInput& RecordMatchStatsInput) const
{
	check(IsValid(GameStatsManager))
	GameStatsManager->RecordMatchStats(RecordMatchStatsInput);
}


void ADS_MatchPlayerState::AddPlayerToScoreboard(AController* NewPlayer)
{
	if (HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Scoreboard controllers are valid from playerstate and gamestate."))
		if (IsValid(ScoreboardController))
		{
			if (!ScoreboardController->PlayerExistsInPlayerArray(DisplayUsername))
			{
				ScoreboardController->AddPlayerInfo(CurrentScoreboardStats);
				UE_LOG(LogTemp, Warning, TEXT("Scoreboard Controller is valid, so we should be setting username now."))
			}
		}
	}
}