// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PsychicGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class PSYCHIC_API APsychicGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	APsychicGameModeBase();


	void PostLogin(APlayerController* NewPlayer) override;

	TSubclassOf<class ACPP_BaseGun> BaseGunClass;
};
