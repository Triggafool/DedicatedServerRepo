﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Portal/SignIn/SignUpPage.h"

#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"

void USignUpPage::ClearTextBoxes()
{
	TextBox_Email->SetText(FText::GetEmpty());
	TextBox_UserName->SetText(FText::GetEmpty());
	TextBox_Password->SetText(FText::GetEmpty());
	TextBox_ConfirmPassword->SetText(FText::GetEmpty());
	TextBlock_StatusMessage->SetText(FText::GetEmpty());
}

void USignUpPage::UpdateStatusMessage(const FString& Message, bool bResetWidget)
{
	TextBlock_StatusMessage->SetText(FText::FromString(Message));

	if (bResetWidget)
	{
		Button_SignUp->SetIsEnabled(true);
	}
	
}

void USignUpPage::NativeConstruct()
{
	TextBlock_StatusMessage->SetText(FText::FromString(TEXT("Please Fill All Fields out.")));
	
	Super::NativeConstruct();
	Button_SignUp->SetIsEnabled(false);

	TextBox_UserName->OnTextChanged.AddDynamic(this, &USignUpPage::UpdateSignUpButtonState);
	TextBox_Password->OnTextChanged.AddDynamic(this, &USignUpPage::UpdateSignUpButtonState);
	TextBox_ConfirmPassword->OnTextChanged.AddDynamic(this, &USignUpPage::UpdateSignUpButtonState);
	TextBox_Email->OnTextChanged.AddDynamic(this, &USignUpPage::UpdateSignUpButtonState);
	Button_SignUp->SetIsEnabled(false);
}

void USignUpPage::UpdateSignUpButtonState(const FText& Text)
{
	const bool bIsUsernameValid = !TextBox_UserName->GetText().ToString().IsEmpty();
	const bool bArePasswordsEqual = TextBox_Password->GetText().ToString() == TextBox_ConfirmPassword->GetText().ToString();
	const bool bIsValiEmail = IsValidEmail(TextBox_Email->GetText().ToString());
	const bool bIsPasswordLongEnough = TextBox_Password->GetText().ToString().Len() >= 8;

	FString StatusMessage;
	const bool bIsStrongPassword = IsStrongPassword(TextBox_Password->GetText().ToString(), StatusMessage);
	if (!bIsStrongPassword)
	{
		TextBlock_StatusMessage->SetText(FText::FromString(StatusMessage));
	}
	else if (!bIsUsernameValid)
	{
		TextBlock_StatusMessage->SetText(FText::FromString(TEXT("Please enter a valid Username.")));
	}
	else if (!bArePasswordsEqual)
	{
		TextBlock_StatusMessage->SetText(FText::FromString(TEXT("Please ensure that passwords match.")));
	}
	else if (!bIsValiEmail)
	{
		TextBlock_StatusMessage->SetText(FText::FromString(TEXT("Please enter a valid email.")));
	}
	else if (!bIsPasswordLongEnough)
	{
		TextBlock_StatusMessage->SetText(FText::FromString(TEXT("Passwords must be at least 8 characters.")));
	}
	else
	{
		TextBlock_StatusMessage->SetText(FText::GetEmpty());
	}
	Button_SignUp->SetIsEnabled(bIsPasswordLongEnough && bIsUsernameValid && bArePasswordsEqual && bIsValiEmail && bIsStrongPassword);
}

bool USignUpPage::IsValidEmail(const FString& Email)
{
	const FRegexPattern EmailPattern(TEXT(R"((^[^\s@]+@[^\s@]+\.[^\s@]{2,}$))"));
	FRegexMatcher Matcher(EmailPattern, Email);
	return Matcher.FindNext();
}

bool USignUpPage::IsStrongPassword(const FString& Password, FString& StatusMessage)
{
	const FRegexPattern NumberPattern(TEXT(R"(\d)")); // contains at least one number
	const FRegexPattern SpecialCharPattern(TEXT(R"([^\w\s])")); // contains at least one special character
	const FRegexPattern UppercasePattern(TEXT(R"([A-Z])")); // contains at least one uppercase character
	const FRegexPattern LowercasePattern(TEXT(R"([a-z])")); // contains at least one lowercase character

	FRegexMatcher NumberMatcher(NumberPattern, Password);
	FRegexMatcher SpecialCharMatcher(SpecialCharPattern, Password);
	FRegexMatcher UppercaseMatcher(UppercasePattern, Password);
	FRegexMatcher LowercaseMatcher(LowercasePattern, Password);

	if (!NumberMatcher.FindNext())
	{
		StatusMessage = TEXT("Password must contain at least 1 number.");
		return false;
	}
	if (!SpecialCharMatcher.FindNext())
	{
		StatusMessage = TEXT("Password must contain at least one special character.");
		return false;
	}
	if (!UppercaseMatcher.FindNext())
	{
		StatusMessage = TEXT("Password must contain at least one uppercase character.");
		return false;
	}
	if (!LowercaseMatcher.FindNext())
	{
		StatusMessage = TEXT("Password must contain at least one lowercase character.");
		return false;
	}
	return true;
}

/*
void USignUpPage::UpdateOnUsernameChangedText(const FText& Text)
{
	CheckForEmptyFields();
	Button_SignUp->SetIsEnabled(ShouldEnableSignUpButton());
}

void USignUpPage::CheckPasswordsMatch()
{
	bool bPasswordFieldEmpty = TextBox_Password->GetText().IsEmpty();
	if (TextBox_ConfirmPassword->GetText().EqualTo(TextBox_Password->GetText()) && !bPasswordFieldEmpty)
	{
		bMatchingPasswords = true;
		CheckForEmptyFields();
	}
	else
	{
		bMatchingPasswords = false;
		TextBlock_StatusMessage->SetText(FText::FromString(TEXT("Passwords must match.")));
	}
}

void USignUpPage::UpdateOnConfirmPasswordChangedText(const FText& Text)
{
	CheckPasswordsMatch();
	Button_SignUp->SetIsEnabled(ShouldEnableSignUpButton());
}
void USignUpPage::UpdateOnPasswordChangedText(const FText& Text)
{
	const FRegexPattern PasswordPattern(TEXT(R"((?=.*[a-z])(?=.*[A-Z])(?=.*\d)(?=.*[@$!%*?&])[A-Za-z\d@$!%*?&]{8,})"));
	FRegexMatcher Matcher(PasswordPattern, Text.ToString());

	const bool bValidConfirmCode = Matcher.FindNext();
	
	if (bValidConfirmCode)
	{
		TextBlock_StatusMessage->SetText(FText::FromString(TEXT("Information looks good.")));
		CheckPasswordsMatch();
		bPasswordAcceptable = true;
	}
	else
	{
		TextBlock_StatusMessage->SetText(FText::FromString(TEXT("Password must be Stronger.")));
		bPasswordAcceptable = false;
	}
	Button_SignUp->SetIsEnabled(ShouldEnableSignUpButton());
}

void USignUpPage::UpdateOnEmailChangedText(const FText& Text)
{
	CheckForEmptyFields();
	Button_SignUp->SetIsEnabled(ShouldEnableSignUpButton());
}

bool USignUpPage::ShouldEnableSignUpButton()
{
	if (bPasswordAcceptable && bMatchingPasswords && bFieldsFilledOut)
	{
		return true;
	}
	return false;
}

void USignUpPage::CheckForEmptyFields()
{
	bool bEmptyFields = TextBox_ConfirmPassword->GetText().IsEmpty() || TextBox_Password->GetText().IsEmpty() || TextBox_UserName->GetText().IsEmpty() || TextBox_Email->GetText().IsEmpty();

	if (bEmptyFields)
	{
		TextBlock_StatusMessage->SetText(FText::FromString(TEXT("Please Fill All Fields out.")));
		bFieldsFilledOut = false;
	} else
	{
		TextBlock_StatusMessage->SetText(FText::FromString(TEXT("")));
		bFieldsFilledOut = true;
	}
}
*/