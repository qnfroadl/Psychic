// Copyright Epic Games, Inc. All Rights Reserved.


#include "PsychicGameModeBase.h"

#include "CPP_BaseCharacter.h"
#include "CPP_BasePlayerState.h"
#include "CPP_BasePlayerController.h"
#include "CPP_GamePlayHUD.h"

APsychicGameModeBase::APsychicGameModeBase()
{
	ConstructorHelpers::FClassFinder<ACPP_BaseCharacter>CHARACTER_CLASS (TEXT("Blueprint'/Game/Blueprints/BP_BaseCharacter.BP_BaseCharacter_C'"));

	if(CHARACTER_CLASS.Succeeded())
	{
		DefaultPawnClass = CHARACTER_CLASS.Class;
		PlayerStateClass = ACPP_BasePlayerState::StaticClass();
		PlayerControllerClass = ACPP_BasePlayerController::StaticClass();
		HUDClass = ACPP_GamePlayHUD::StaticClass();
	}
	else 
	{
		DefaultPawnClass = ACPP_BaseCharacter::StaticClass();
		PlayerStateClass = ACPP_BasePlayerState::StaticClass();
		PlayerControllerClass = ACPP_BasePlayerController::StaticClass();
		HUDClass = ACPP_GamePlayHUD::StaticClass();
	}

}
