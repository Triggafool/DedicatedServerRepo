// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Portal/SignIn/ConfirmSignUpPage.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"

void UConfirmSignUpPage::ClearTextBoxes()
{
	TextBox_ConfirmationCode->SetText(FText::GetEmpty());
	TextBlock_StatusMessage->SetText(FText::GetEmpty());
	TextBlock_DestinationEmail->SetText(FText::GetEmpty());
}

void UConfirmSignUpPage::UpdateStatusMessage(const FString& Message, bool bResetWidget)
{
	TextBlock_StatusMessage->SetText(FText::FromString(Message));

	if (bResetWidget)
	{
		Button_Confirm->SetIsEnabled(true);
	}
}

void UConfirmSignUpPage::NativeConstruct()
{
	Super::NativeConstruct();
	Button_Confirm->SetIsEnabled(false);
	TextBox_ConfirmationCode->OnTextChanged.AddDynamic(this, &UConfirmSignUpPage::UpdateConfirmButtonState);
}

void UConfirmSignUpPage::UpdateConfirmButtonState(const FText& Text)
{
	const FRegexPattern SixDigitsPattern(TEXT(R"(^\d{6}$)"));
	FRegexMatcher Matcher(SixDigitsPattern, Text.ToString());

	const bool bValidConfirmCode = Matcher.FindNext();

	Button_Confirm->SetIsEnabled(bValidConfirmCode);

	if (bValidConfirmCode)
	{
		TextBlock_StatusMessage->SetText(FText::FromString(TEXT("")));
	}
	else
	{
		TextBlock_StatusMessage->SetText(FText::FromString(TEXT("Please Enter 6 Numbers. You Absolute IDIOT.")));
	}
	
}
