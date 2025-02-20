// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/LobbyPlayerBox.h"
#include "Components/ScrollBox.h"
#include "Game/DSGameState.h"
#include "Lobby/LobbyState.h"
#include "UI/Lobby/PlayerLabel.h"
#include "Lobby/LobbyPlayerInfo.h"

void ULobbyPlayerBox::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ADSGameState* DSGameState = GetWorld()->GetGameState<ADSGameState>();
	if (!IsValid(DSGameState)) return;

	if (IsValid(DSGameState->LobbyState))
	{
		OnLobbyStateInitialized(DSGameState->LobbyState);
	}
	else
	{
		DSGameState->OnLobbyStateInitialized.AddDynamic(this, &ULobbyPlayerBox::OnLobbyStateInitialized);
	}
}

void ULobbyPlayerBox::UpdatePlayerInfo(ALobbyState* LobbyState)
{
	ScrollBox_PlayerInfo->ClearChildren();

	for (const FLobbyPlayerInfo& PlayerInfo : LobbyState->GetPlayers())
	{
		UE_LOG(LogTemp, Warning, TEXT("ULobbyPlayerBox::UpdatePlayerInfo AddedPlayer: %s"), *PlayerInfo.Username);
		CreateAndAddPlayerLabel(PlayerInfo);
	}
}

void ULobbyPlayerBox::OnLobbyStateInitialized(ALobbyState* LobbyState)
{
	if (!IsValid(LobbyState)) return;
	
	LobbyState->OnPlayerInfoAdded.AddDynamic(this, &ULobbyPlayerBox::CreateAndAddPlayerLabel);
	LobbyState->OnPlayerInfoRemoved.AddDynamic(this, &ULobbyPlayerBox::OnPlayerRemoved);
	UpdatePlayerInfo(LobbyState);
}

void ULobbyPlayerBox::CreateAndAddPlayerLabel(const FLobbyPlayerInfo& PlayerInfo)
{
	if (FindPlayerLabelByUsername(PlayerInfo.Username)) 
	{
	UE_LOG(LogTemp, Warning, TEXT("Found Duplicate Username"));
	return; // make sure we don't create a duplicate player label
	}
	
	UPlayerLabel* PlayerLabel = CreateWidget<UPlayerLabel>(this, PlayerLabelClass);

	if (!IsValid(PlayerLabel)) return;
	
	UE_LOG(LogTemp, Warning, TEXT("ULobbyPlayerBox::CreateAndAddPlayerLabel AddedPlayer: %s"), *PlayerInfo.Username);
	PlayerLabel->SetUserName(PlayerInfo.Username);
	ScrollBox_PlayerInfo->AddChild(PlayerLabel);
	
}

void ULobbyPlayerBox::OnPlayerRemoved(const FLobbyPlayerInfo& PlayerInfo)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Cyan, "Removing Player Label");
	if (UPlayerLabel* PlayerLabel = FindPlayerLabelByUsername(PlayerInfo.Username))
	{
		ScrollBox_PlayerInfo->RemoveChild(PlayerLabel);
	}
}

UPlayerLabel* ULobbyPlayerBox::FindPlayerLabelByUsername(const FString& Username)
{
	for (UWidget* Child : ScrollBox_PlayerInfo->GetAllChildren())
	{
		UPlayerLabel* PlayerLabel = Cast<UPlayerLabel>(Child);
		if (IsValid(PlayerLabel) && PlayerLabel->GetUsername() == Username)
		{
			return PlayerLabel;
		}
	}

	return nullptr;
}
