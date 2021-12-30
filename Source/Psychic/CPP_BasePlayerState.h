// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "CPP_BasePlayerState.generated.h"

/**
 * 
 */
UCLASS()
class PSYCHIC_API ACPP_BasePlayerState : public APlayerState
{
	GENERATED_BODY()
	

public:
	ACPP_BasePlayerState() = default;

	bool bCrouchToggleMode = true;
	bool bIronsightsToggleMode = true;
};
