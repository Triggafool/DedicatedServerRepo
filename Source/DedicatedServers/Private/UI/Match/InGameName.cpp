// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Match/InGameName.h"
#include "Components/TextBlock.h"

void UInGameName::SetPlayerName(const FString& Username)
{
	if (Username.IsEmpty()) return;
	if (IsValid(TextBlock_PlayerName))
	{
		TextBlock_PlayerName->SetText(FText::FromString(Username));
	}
}
