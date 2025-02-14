// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AccountDropDown_Expanded.generated.h"

class UDSLocalPlayerSubSystem;
class UPortalManager;
class UTextBlock;
class UButton;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UAccountDropDown_Expanded : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPortalManager> PortalManagerClass;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_SignOut;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_SignOutButtonText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Email;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSlateColor HoveredTextColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSlateColor UnHoveredTextColor;

protected:
	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;;
		
	UFUNCTION()
	void SignOutButton_Hover();
	
	UFUNCTION()
	void SignOutButton_Unhover();

	UFUNCTION()
	void SignOutButton_OnClicked();

private:
	
	UPROPERTY()
	TObjectPtr<UPortalManager> PortalManager;

	void SetSignOutButtonTransparent();

	UDSLocalPlayerSubSystem* GetLocalPlayerSubSystem();
};
