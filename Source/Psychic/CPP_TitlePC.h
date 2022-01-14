// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CPP_TitlePC.generated.h"

/**
 * 
 */
UCLASS()
class PSYCHIC_API ACPP_TitlePC : public APlayerController
{
	GENERATED_BODY()

public:
	ACPP_TitlePC();

	void BeginPlay() override;

	class ACPP_TitleHUD* TitleHUD;
};
