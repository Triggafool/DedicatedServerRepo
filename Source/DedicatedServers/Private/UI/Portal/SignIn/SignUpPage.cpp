// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Portal/SignIn/SignUpPage.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

void USignUpPage::UpdateStatusMessage(const FString& Message, bool bResetWidget)
{
	TextBlock_StatusMessage->SetText(FText::FromString(Message));

	if (bResetWidget)
	{
		Button_SignUp->SetIsEnabled(true);
	}
	
}
