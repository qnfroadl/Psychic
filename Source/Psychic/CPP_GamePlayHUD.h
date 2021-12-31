// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CPP_GamePlayHUD.generated.h"

/**
 * 
 */
UCLASS()
class PSYCHIC_API ACPP_GamePlayHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	void DrawHUD() override;

	void DrawCrosshair();
};
