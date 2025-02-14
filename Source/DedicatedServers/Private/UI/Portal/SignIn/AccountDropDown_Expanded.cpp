// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Portal/SignIn/AccountDropDown_Expanded.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Player/DSLocalPlayerSubSystem.h"
#include "UI/Portal/PortalManager.h"

void UAccountDropDown_Expanded::NativeConstruct()
{
	Super::NativeConstruct();

	check(PortalManagerClass)
	PortalManager = NewObject<UPortalManager>(this, PortalManagerClass);
	
	Button_SignOut->OnClicked.AddDynamic(this, &UAccountDropDown_Expanded::SignOutButton_OnClicked);
	Button_SignOut->OnHovered.AddDynamic(this, &UAccountDropDown_Expanded::SignOutButton_Hover);
	Button_SignOut->OnUnhovered.AddDynamic(this, &UAccountDropDown_Expanded::SignOutButton_Unhover);

	UDSLocalPlayerSubSystem* LocalPlayerSubSystem = GetLocalPlayerSubSystem();

	if (IsValid(LocalPlayerSubSystem))
	{
		TextBlock_Email->SetText(FText::FromString(LocalPlayerSubSystem->Email));
	}
}

void UAccountDropDown_Expanded::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	SetSignOutButtonTransparent();
	SignOutButton_Unhover();
}

void UAccountDropDown_Expanded::SignOutButton_Hover()
{
	TextBlock_SignOutButtonText->SetColorAndOpacity(HoveredTextColor);
}

void UAccountDropDown_Expanded::SignOutButton_Unhover()
{
	TextBlock_SignOutButtonText->SetColorAndOpacity(UnHoveredTextColor);
}

void UAccountDropDown_Expanded::SignOutButton_OnClicked()
{
	Button_SignOut->SetIsEnabled(false);

	check(PortalManager)

	UDSLocalPlayerSubSystem* LocalPlayerSubSystem = GetLocalPlayerSubSystem();

	if (IsValid(LocalPlayerSubSystem))
	{
		FDSAuthenticationResult AuthResults = LocalPlayerSubSystem->GetAuthResults();
		PortalManager->SignOut(AuthResults.AccessToken);
	}
	
}

void UAccountDropDown_Expanded::SetSignOutButtonTransparent()
{
	FButtonStyle Style;

	FSlateBrush Brush;
	Brush.TintColor = FSlateColor(FLinearColor(0.f,0.f,0.f,0.f ));

	Style.Disabled = Brush;
	Style.Hovered = Brush;
	Style.Pressed = Brush;
	Style.Normal = Brush;

	Button_SignOut->SetStyle(Style);
}

UDSLocalPlayerSubSystem* UAccountDropDown_Expanded::GetLocalPlayerSubSystem()
{
	check(PortalManager)

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
