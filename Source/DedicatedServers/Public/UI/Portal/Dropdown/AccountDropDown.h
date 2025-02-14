// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Dropdown.h"
#include "AccountDropDown.generated.h"

class UDSLocalPlayerSubSystem;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UAccountDropDown : public UDropdown
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

private:

	UDSLocalPlayerSubSystem* GetLocalPlayerSubSystem();
};
