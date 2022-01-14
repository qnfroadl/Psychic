// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CPP_TitleGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PSYCHIC_API ACPP_TitleGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ACPP_TitleGameMode();

	virtual void BeginPlay() override;

};
