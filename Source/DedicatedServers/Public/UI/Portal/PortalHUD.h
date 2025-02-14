// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/Interfaces/HUDManagement.h"
#include "PortalHUD.generated.h"

class UDashboardOverlay;
class USignInOverlay;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API APortalHUD : public AHUD, public IHUDManagement
{
	GENERATED_BODY()

public:
	/*<IHUDManagement>*/

	virtual void OnSignIn() override;
	virtual void OnSignOut() override;
	
	/*</IHUDManagement>*/
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USignInOverlay> SignInOverlayClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDashboardOverlay> DashboardOverlayClass;

	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TObjectPtr<USignInOverlay> SignInOverlay;

	UPROPERTY()
	TObjectPtr<UDashboardOverlay> DashboardOverlay;
};
