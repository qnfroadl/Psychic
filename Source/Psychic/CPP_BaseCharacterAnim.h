// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CPP_BaseCharacterAnim.generated.h"

/**
 * 
 */
UCLASS()
class PSYCHIC_API UCPP_BaseCharacterAnim : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UCPP_BaseCharacterAnim();

	void NativeBeginPlay() override;
	void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Speed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Direction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float AimPitch;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIronSight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bSprint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bCrouch;

private:
	class ACPP_BaseCharacter* BaseCharacter;

};
