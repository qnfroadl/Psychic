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
	class UCameraComponent* TPPCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCameraComponent* FPPCamera;

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
	UFUNCTION(Server, Reliable)
	void CS_OnSprint();
	void CS_OnSprint_Implementation();
	UFUNCTION(NetMulticast, Reliable)
	void MC_OnSprint();
	void MC_OnSprint_Implementation();

	// OffSprint
	UFUNCTION(Server, Reliable)
	void CS_OffSprint();
	void CS_OffSprint_Implementation();
	UFUNCTION(NetMulticast, Reliable)
	void MC_OffSprint();
	void MC_OffSprint_Implementation();

	void UpdateSprintState(bool Sprint);

	// OnWalk
// 	UFUNCTION(Server, Reliable)
// 	void CS_OnWalk();
// 	void CS_OnWalk_Implementation();
// 	UFUNCTION(NetMulticast, Reliable)
// 	void MC_OnWalk();
// 	void MC_OnWalk_Implementation();

	// OffWalk
// 	UFUNCTION(Server, Reliable)
// 	void CS_OffWalk();
// 	void CS_OffWalk_Implementation();
// 	UFUNCTION(NetMulticast, Reliable)
// 	void MC_OffWalk();
// 	void MC_OffWalk_Implementation();

	void UpdateMoveSpeed();

	// OnCrouch
	UFUNCTION(Server, Reliable)
	void CS_OnCrouch();
	void CS_OnCrouch_Implementation();
	UFUNCTION(NetMulticast, Reliable)
	void MC_OnCrouch();
	void MC_OnCrouch_Implementation();

	// OffCrouch
	UFUNCTION(Server, Reliable)
	void CS_OffCrouch();
	void CS_OffCrouch_Implementation();
	UFUNCTION(NetMulticast, Reliable)
	void MC_OffCrouch();
	void MC_OffCrouch_Implementation();

	void UpdateCrouchState(bool Crouch);

	// OnWalk
	UFUNCTION(Server, Reliable)
	void CS_OnIronsights();
	void CS_OnIronsights_Implementation();
	UFUNCTION(NetMulticast, Reliable)
	void MC_OnIronsights();
	void MC_OnIronsights_Implementation();

	// OffWalk
	UFUNCTION(Server, Reliable)
	void CS_OffIronsights();
	void CS_OffIronsights_Implementation();
	UFUNCTION(NetMulticast, Reliable)
	void MC_OffIronsights();
	void MC_OffIronsights_Implementation();

	void UpdateIronsightsState(bool Ironsights);

	// OnFire
	UFUNCTION(Server, Reliable)
	void CS_OnFire();
	void CS_OnFire_Implementation();
	UFUNCTION(NetMulticast, Reliable)
	void MC_OnFire();
	void MC_OnFire_Implementation();

	// OffFire
	UFUNCTION(Server, Reliable)
	void CS_OffFire();
	void CS_OffFire_Implementation();
	UFUNCTION(NetMulticast, Reliable)
	void MC_OffFire();
	void MC_OffFire_Implementation();


	UFUNCTION(Server, Reliable)
	void CS_UpdateControlRotation(const FRotator& rotator);
	void CS_UpdateControlRotation_Implementation(const FRotator& rotation);

	UFUNCTION(NetMulticast, Reliable)
	void MC_UpdateControlRotation(const FRotator& rotation);
	void MC_UpdateControlRotation_Implementation(const FRotator& rotation);

	void TogglePrespective();


	bool IsSprint() {return this->bSprint;}
	bool IsIronsights() {return this->bIronsights;}
	bool IsCrouch() { return this->bCrouch;}

public:
	const FVector NormalSocketOffset = FVector(0, 70, 70);

	bool bSprint = false;
	bool bIronsights = false;
	bool bCrouch = false;
	bool bFPP = false;

	//test.
	bool bWalk = false;

	FRotator ControlRotation;	// Client에서 입력한 로테이션값

private:
	constexpr static float SPRINTSPEED = 450.f;
	constexpr static float JOGSPEED = 250.f;
	constexpr static float WALKSPEED = 150.f;
	constexpr static float CROUCHSPEED = 100.f;

	class ACPP_BasePlayerState* PlayerState = nullptr;
	class UCPP_BaseCharacterAnim* AnimInstance = nullptr;
	class ACPP_BaseGun* Gun = nullptr;

};