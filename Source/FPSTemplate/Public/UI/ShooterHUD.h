// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ShooterHUD.generated.h"

class UShooterMatchOverlay;
class UUserWidget;

/**
 * 
 */
UCLASS()
class FPSTEMPLATE_API AShooterHUD : public AHUD
{
	GENERATED_BODY()
public:
	UShooterMatchOverlay* GetShooterOverlay() { return Overlay; }

protected:
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditDefaultsOnly, Category = "Overlay")
	TSubclassOf<UShooterMatchOverlay> ShooterOverlayClass;

	UPROPERTY()
	TObjectPtr<UShooterMatchOverlay> Overlay;
};
