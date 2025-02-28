// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Match/MatchOverlay.h"
#include "UI/HTTP/HTTPRequestTypes.h"
#include "ShooterMatchOverlay.generated.h"

/**
 *  Parent class for the Overlay for the Shooter Game so we can create some functionality in C++ here.
 */
UCLASS()
class FPSTEMPLATE_API UShooterMatchOverlay : public UMatchOverlay
{
	GENERATED_BODY()

public:

	bool bScoreboardIsOpen = false;
	virtual void OnScoreboardOpen(bool bOpen) override;

	virtual void RefreshMatchStats() override;
	
};
