// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_BaseCameraManager.h"
#include "CPP_BaseCharacter.h"

#include <Camera/CameraComponent.h>

ACPP_BaseCameraManager::ACPP_BaseCameraManager()
{
	
	NormalFOV = 90.0f;
	IronsightsFOV = 60.0f;

	
	ViewPitchMax = 75.f;
	ViewPitchMin = -75.f;

}

void ACPP_BaseCameraManager::UpdateCamera(float DeltaTime)
{
	check(PCOwner != nullptr);

	ACPP_BaseCharacter* BaseCharacter = Cast<ACPP_BaseCharacter>(PCOwner->GetPawn());
	if (BaseCharacter)
	{
		const float TargetFOV = BaseCharacter->IsIronsights() ? IronsightsFOV : NormalFOV;
		if (TargetFOV != DefaultFOV)
		{
			DefaultFOV = FMath::FInterpTo(DefaultFOV, TargetFOV, DeltaTime, 20.0f);
			BaseCharacter->TPPCamera->SetFieldOfView(DefaultFOV);
		}
	}

	Super::UpdateCamera(DeltaTime);
}
