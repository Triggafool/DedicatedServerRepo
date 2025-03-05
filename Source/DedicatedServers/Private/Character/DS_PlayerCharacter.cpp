// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/DS_PlayerCharacter.h"
#include "Components/WidgetComponent.h"
#include "Player/DS_MatchPlayerState.h"
#include "UI/Match/InGameName.h"

ADS_PlayerCharacter::ADS_PlayerCharacter()
{
	// Begin Play Gets called and The username does show up, but only after the first death, so PlayerState isnt ready at this point.
	// We need to Inject this Name into the Character's information some other time after this.
	PrimaryActorTick.bCanEverTick = true;
	
	DisplayNameComponent = CreateDefaultSubobject<UWidgetComponent>("InGameNameWidget");
	DisplayNameComponent->SetupAttachment(GetRootComponent());
}

void ADS_PlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	ADS_MatchPlayerState* DSPlayerState = Cast<ADS_MatchPlayerState>(GetPlayerState());
	if (IsValid(DSPlayerState))
	{
		DSPlayerState->SetUsernameInStats(NewController);
		DSPlayerState->AddPlayerToScoreboard(GetController());
	}
}

void ADS_PlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	ADS_MatchPlayerState* DSPlayerState = Cast<ADS_MatchPlayerState>(GetPlayerState());
	if (IsValid(DSPlayerState))
	{
		SetDisplayName(DSPlayerState->GetDisplayName());
		DSPlayerState->OnDisplayNameReplicated.AddDynamic(this, &ADS_PlayerCharacter::SetDisplayName);
	}
}

void ADS_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	if(GetLocalRole() == ROLE_AutonomousProxy)
	{
		DisplayNameComponent->bHiddenInGame = true;
	}

	DisplayName = Cast<UInGameName>(DisplayNameComponent->GetWidget());
	
	ADS_MatchPlayerState* DSPlayerState = Cast<ADS_MatchPlayerState>(GetPlayerState());
	if (IsValid(DSPlayerState))
	{
		SetDisplayName(DSPlayerState->GetDisplayName());
	}
}


void ADS_PlayerCharacter::SetDisplayName(const FString& NewDisplayName)
{
	DisplayName->SetPlayerName(NewDisplayName);
}

void ADS_PlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Get the display name and check if this is a simulated proxy..
	// Then rotate the username with get look at rotation so it is always facing the observer
}

