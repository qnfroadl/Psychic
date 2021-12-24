// Copyright Epic Games, Inc. All Rights Reserved.


#include "PsychicGameModeBase.h"
#include "CPP_BaseCharacter.h"

APsychicGameModeBase::APsychicGameModeBase()
{
	ConstructorHelpers::FClassFinder<ACPP_BaseCharacter>CHARACTER_CLASS (TEXT("Blueprint'/Game/Blueprints/BP_BaseCharacter.BP_BaseCharacter_C'"));

	if(CHARACTER_CLASS.Succeeded())
	{
		DefaultPawnClass = CHARACTER_CLASS.Class;
	}

}
