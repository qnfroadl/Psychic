// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "CPP_BaseCameraManager.generated.h"

/**
 * 
 */
UCLASS()
class PSYCHIC_API ACPP_BaseCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()
	
public:
	ACPP_BaseCameraManager();

	float NormalFOV;
	float IronsightsFOV;

	virtual void UpdateCamera(float DeltaTime) override;

};
