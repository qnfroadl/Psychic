// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CPP_BaseCharacter.generated.h"

UCLASS()
class PSYCHIC_API ACPP_BaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACPP_BaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCameraComponent* Camera;

	UFUNCTION()
	void OnMoveForward(float AxisValue);

	UFUNCTION()
	void OnMoveRight(float AxisValue);

	UFUNCTION()
	void OnLookUp(float AxisValue);

	UFUNCTION()
	void OnTurn(float AxisValue);

public:
	const FVector NormalSocketOffset = FVector(0, 140, 70);
	const float NormalFOV = 90.f;

};
