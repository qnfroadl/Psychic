// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_BaseCharacterAnim.h"

#include "CPP_BaseCharacter.h"

UCPP_BaseCharacterAnim::UCPP_BaseCharacterAnim()
	:Speed(0.f), BaseCharacter(nullptr)
{

}

void UCPP_BaseCharacterAnim::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	BaseCharacter = Cast<ACPP_BaseCharacter>(TryGetPawnOwner());
}

void UCPP_BaseCharacterAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (BaseCharacter && BaseCharacter->IsValidLowLevel())
	{
		// Update Speed, Direction, Rotation
		FVector Vel = BaseCharacter->GetVelocity();
		this->Speed = Vel.Size();
		this->Direction = CalculateDirection(Vel, BaseCharacter->GetActorRotation());
		this->AimPitch = BaseCharacter->ControlRotation.Pitch;

		// UE_LOG(LogTemp, Warning, TEXT("%lf"), this->Speed);
	}
}