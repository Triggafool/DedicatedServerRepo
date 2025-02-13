// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ConfirmSignUpPage.generated.h"
class UTextBlock;
class UEditableTextBox;
class UButton;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UConfirmSignUpPage : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_StatusMessage;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> TextBox_ConfirmationCode;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Confirm;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Back;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_DestinationEmail;

	void ClearTextBoxes();

	UFUNCTION()
	void UpdateStatusMessage(const FString& Message, bool bResetWidget);
protected:
	void NativeConstruct() override;
private:
	UFUNCTION()
	void UpdateConfirmButtonState(const FText& Text);
	
};
