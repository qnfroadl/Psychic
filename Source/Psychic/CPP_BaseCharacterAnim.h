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

	UFUNCTION(BlueprintImplementableEvent)
	void OnFire();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Direction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AimPitch;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIronSight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bSprint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bCrouch;

private:
	class ACPP_BaseCharacter* BaseCharacter;
};
