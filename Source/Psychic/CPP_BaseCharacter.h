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

	UFUNCTION()
	void OnRun();
	UFUNCTION(Server, Reliable)
	void CS_OnRun();
	void CS_OnRun_Implementation();
	UFUNCTION(NetMulticast, Reliable)
	void MC_OnRun();
	void MC_OnRun_Implementation();

	UFUNCTION()
	void OffRun();
	UFUNCTION(Server, Reliable)
	void CS_OffRun();
	void CS_OffRun_Implementation();
	UFUNCTION(NetMulticast, Reliable)
	void MC_OffRun();
	void MC_OffRun_Implementation();

	UFUNCTION()
	void OnWalk();
	UFUNCTION(Server, Reliable)
	void CS_OnWalk();
	void CS_OnWalk_Implementation();
	UFUNCTION(NetMulticast, Reliable)
	void MC_OnWalk();
	void MC_OnWalk_Implementation();

	UFUNCTION()
	void OffWalk();
	UFUNCTION(Server, Reliable)
	void CS_OffWalk();
	void CS_OffWalk_Implementation();
	UFUNCTION(NetMulticast, Reliable)
	void MC_OffWalk();
	void MC_OffWalk_Implementation();

	void MoveSpeedUpdate();

	UFUNCTION(Server, Reliable)
	void CS_UpdateControlRotation(const FRotator& rotator);
	void CS_UpdateControlRotation_Implementation(const FRotator& rotation);

	UFUNCTION(NetMulticast, Reliable)
	void MC_UpdateControlRotation(const FRotator& rotation);
	void MC_UpdateControlRotation_Implementation(const FRotator& rotation);

public:
	const FVector NormalSocketOffset = FVector(0, 140, 70);
	const float NormalFOV = 90.f;

	bool bRun;
	bool bWalk;

	FRotator ControlRotation;	// Client에서 입력한 로테이션값

private:
	constexpr static float RUNSPEED = 600.f;
	constexpr static float MOVESPEED = 300.f;
	constexpr static float WALKSPEED = 150.f;
	constexpr static float CROUCHSPEED = 150.f;
	
};