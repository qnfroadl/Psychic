// Copyright Epic Games, Inc. All Rights Reserved.


#include "PsychicGameModeBase.h"
#include "CPP_BaseCharacter.h"
#include "CPP_BasePlayerState.h"

APsychicGameModeBase::APsychicGameModeBase()
{
	ConstructorHelpers::FClassFinder<ACPP_BaseCharacter>CHARACTER_CLASS (TEXT("Blueprint'/Game/Blueprints/BP_BaseCharacter.BP_BaseCharacter_C'"));

	if(CHARACTER_CLASS.Succeeded())
	{
		DefaultPawnClass = CHARACTER_CLASS.Class;
		PlayerStateClass = ACPP_BasePlayerState::StaticClass();
	}
	else 
	{
		DefaultPawnClass = ACPP_BaseCharacter::StaticClass();
		PlayerStateClass = ACPP_BasePlayerState::StaticClass();
	}

}
