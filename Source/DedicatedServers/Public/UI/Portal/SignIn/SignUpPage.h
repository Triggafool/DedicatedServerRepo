// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SignUpPage.generated.h"

class UTextBlock;
class UEditableTextBox;
class UButton;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API USignUpPage : public UUserWidget
{
	GENERATED_BODY()

public:
	bool bShouldEnableButton = false;
	bool bPasswordAcceptable = false;
	bool bMatchingPasswords = false;
	bool bFieldsFilledOut = false;
	void ClearTextBoxes();
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_StatusMessage;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> TextBox_UserName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> TextBox_Password;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> TextBox_ConfirmPassword;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> TextBox_Email;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_SignUp;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Back;

	UFUNCTION()
	void UpdateStatusMessage(const FString& Message, bool bResetWidget);

protected:
	void NativeConstruct() override;

private:

	UFUNCTION()
	void UpdateSignUpButtonState(const FText& Text);

	bool IsValidEmail(const FString& Email);
	
	bool IsStrongPassword(const FString& Password, FString& StatusMessage);


/*
	UFUNCTION()
	void UpdateOnPasswordChangedText(const FText& Text);

	UFUNCTION()
	void UpdateOnUsernameChangedText(const FText& Text);
	void CheckPasswordsMatch();
	
	UFUNCTION()
	void UpdateOnConfirmPasswordChangedText(const FText& Text);

	void CheckForEmptyFields();

	UFUNCTION()
	void UpdateOnEmailChangedText(const FText& Text);

	bool ShouldEnableSignUpButton();
	*/
};
