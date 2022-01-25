// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CPP_BasePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PSYCHIC_API ACPP_BasePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ACPP_BasePlayerController();

	void BeginPlay() override;


};
