// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Portal/Dropdown/AccountDropDown.h"

#include "Components/TextBlock.h"
#include "Player/DSLocalPlayerSubSystem.h"

void UAccountDropDown::NativeConstruct()
{
	Super::NativeConstruct();
	
	UDSLocalPlayerSubSystem* LocalPlayerSubSystem = GetLocalPlayerSubSystem();
	if (IsValid(LocalPlayerSubSystem))
	{
		TextBlock_ButtonText->SetText(FText::FromString(LocalPlayerSubSystem->Username));
	}
}

UDSLocalPlayerSubSystem* UAccountDropDown::GetLocalPlayerSubSystem()
{

	APlayerController* PlayerController = GetOwningPlayer();
	if (IsValid(PlayerController) && IsValid(PlayerController->GetLocalPlayer()))
	{
		UDSLocalPlayerSubSystem* PlayerSubSystem = PlayerController->GetLocalPlayer()->GetSubsystem<UDSLocalPlayerSubSystem>();

		if (IsValid(PlayerSubSystem))
		{
			return PlayerSubSystem;
		}
	}
	return nullptr;
}
