﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "APITestOverlay.generated.h"

class UAPITestManager;
class UListFleetsBox;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UAPITestOverlay : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAPITestManager> APITestManagerClass;

protected:

	virtual void NativeConstruct() override;

	
private:
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UListFleetsBox> ListFleetsBox;

	UPROPERTY()
	TObjectPtr<UAPITestManager> TestManager;

};
