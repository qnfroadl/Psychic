// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_BaseCharacterAnim.h"

#include "CPP_BaseCharacter.h"

UCPP_BaseCharacterAnim::UCPP_BaseCharacterAnim()
	:Speed(0.f), BaseCharacter(nullptr)
{

// 	ConstructorHelpers::FObjectFinder<UAnimMontage>FIRE_MONTAGE (TEXT("AnimMontage'/Game/Blueprints/Animation/Fire_Rifle_Hip_Montage.Fire_Rifle_Hip_Montage'"));
// 	if (FIRE_MONTAGE.Succeeded())
// 	{
// 		FireMontage = FIRE_MONTAGE.Object;
// 	}

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

		this->AimPitch = FMath::ClampAngle(BaseCharacter->ControlRotation.Pitch, -90, 90);
		
		this->bIronSight = BaseCharacter->bIronsights;
		this->bCrouch = BaseCharacter->bCrouch;
		this->bSprint = BaseCharacter->bSprint;

	}
}