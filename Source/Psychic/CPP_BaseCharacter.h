// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CPP_BaseCharacter.generated.h"

class ACPP_BaseGun;

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
	class UCameraComponent* TPPCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCameraComponent* FPPCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCameraComponent* ScopeCamera;

// 
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite)
// 	class ACPP_BaseGun* Gun;

	UFUNCTION()
	void OnMoveForward(float AxisValue);

	UFUNCTION()
	void OnMoveRight(float AxisValue);

	UFUNCTION()
	void OnLookUp(float AxisValue);

	UFUNCTION()
	void OnTurn(float AxisValue);

	// OnSprint

	UFUNCTION()
	void OnStartSprint();
	UFUNCTION()
	void OnStopSprint();

	void SetSprint(bool _bSprint);

	UFUNCTION(Server, Reliable)
	void ServerSetSprint(bool _bSprint);

	void UpdateMoveSpeed();

	// On Crouch
	UFUNCTION()
	void OnStartCrouchAction();
	
	UFUNCTION()
	void OnStopCrouchAction();

	void SetCrouch(bool Crouch);
	UFUNCTION(Server, Reliable)
	void ServerSetCrouch(bool Crouch);

	// On Ironsights
	UFUNCTION()
	void OnStartIronsights();
	UFUNCTION()
	void OnStopIronsights();

	void SetIronsights(bool Ironsight);
	UFUNCTION(Server, Reliable)
	void ServerSetIronsights(bool Ironsight);

	// OnFire
	void OnStartFire();
	void OnStopFire();

	UFUNCTION()
	void OnReload();
	UFUNCTION(Server, Reliable)
	void ServerReload();


	UFUNCTION(Server, Reliable)
	void CS_UpdateControlRotation(const FRotator& rotator);
	void CS_UpdateControlRotation_Implementation(const FRotator& rotation);

	UFUNCTION(NetMulticast, Reliable)
	void MC_UpdateControlRotation(const FRotator& rotation);
	void MC_UpdateControlRotation_Implementation(const FRotator& rotation);

	void TogglePrespective();

	bool IsRunning();

	bool IsSprint();
	bool IsIronsights() {return this->bIronsights;}
	bool IsCrouch() { return this->bCrouch;}

	UCameraComponent* GetCurrentCamera();
	FVector GetCameraLocation();
	FVector GetCameraForward();

	void SetGun(ACPP_BaseGun* Gun);
	UFUNCTION(Server, Reliable)
	void ServerSetGun(ACPP_BaseGun* Gun);

	UFUNCTION()
	void OnRep_SetGun();
	
	void OnFireAnim();

private:
	void UpdateCrouchCamera();


public:
	const FVector NormalSocketOffset = FVector(0, 30, 20);
	const FVector CrouchSocketOffset = FVector(0,30,-40);


	UPROPERTY(Replicated)
	bool bSprint = false;

	UPROPERTY(Replicated)	// Transient ???????? ????????. ???? ???? ?????? ?????? ????. ???????? ????.
	bool bIronsights = false;

	UPROPERTY(Replicated)
	bool bCrouch = false;

	bool bFPP = false;

	UPROPERTY(ReplicatedUsing=OnRep_SetGun)
	ACPP_BaseGun* BaseGun = nullptr;

	FRotator ControlRotation;	// Client???? ?????? ??????????

private:
	bool bWantsToSprint;

	constexpr static float SPRINTSPEED = 400.f;
	constexpr static float JOGSPEED = 200.f;
	constexpr static float WALKSPEED = 130.f;
	constexpr static float CROUCHSPEED = 80.f;

	class ACPP_BasePlayerState* PlayerState = nullptr;
	class UCPP_BaseCharacterAnim* AnimInstance = nullptr;

	

	class ACPP_BasePlayerState* GetBasePlayerState();

	TSubclassOf<ACPP_BaseGun> BaseGunClass;

	
};