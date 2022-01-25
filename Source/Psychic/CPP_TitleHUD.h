// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

#include "CPP_TitleHUD.generated.h"

/**
 * 
 */
class UCPP_TitleWidget;

UCLASS()
class PSYCHIC_API ACPP_TitleHUD : public AHUD
{
	GENERATED_BODY()

public:	
	ACPP_TitleHUD();

	void BeginPlay() override;

	TSubclassOf<UCPP_TitleWidget> TitleWidgetClass;

	UCPP_TitleWidget* TitleWidget;
};
