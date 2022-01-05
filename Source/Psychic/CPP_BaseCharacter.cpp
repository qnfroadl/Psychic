// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_BaseCharacter.h"

#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <Components/SkeletalMeshComponent.h>
#include <Components/SceneComponent.h>
#include <Gameframework/CharacterMovementComponent.h>
#include <Net/UnrealNetwork.h>
#include <Kismet/KismetMathLibrary.h>

#include "CPP_BaseCharacterAnim.h"
#include "CPP_BasePlayerState.h"
#include "CPP_BaseGun.h"

// Sets default values
ACPP_BaseCharacter::ACPP_BaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set SkeletalMesh
	ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_MANNEQUIN(TEXT("SkeletalMesh'/Game/Mannequin/Mesh/SK_Mannequin.SK_Mannequin'"));
	if (SK_MANNEQUIN.Succeeded())
	{	
		GetMesh()->SetSkeletalMesh(SK_MANNEQUIN.Object);
		GetMesh()->SetRelativeLocation(FVector(0,0,-90));
		GetMesh()->SetRelativeRotation(FRotator(0,-90,0));	// pitch, yaw, roll 순서.
	}

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	TPPCamera = CreateDefaultSubobject<UCameraComponent>("TPPCamera");
	FPPCamera = CreateDefaultSubobject<UCameraComponent>("FPPCamera");
	

	// Set SpringArm.
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->SetRelativeRotation(FRotator(-20, 0, 0));
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->SocketOffset = this->NormalSocketOffset;

	// Set Camera.
	TPPCamera->SetupAttachment(SpringArm);
	FPPCamera->SetupAttachment(GetMesh(), TEXT("head"));
	FPPCamera->SetRelativeRotation(FRotator(0.f, 90.f, -90.f));
	FPPCamera->bUsePawnControlRotation = true;	// 안하면 머리 따라서 흔들림.

	// Set Gun StaticMesh.
// 	Gun = CreateDefaultSubobject<UCPP_BaseGun>(TEXT("Gun"));
// 	ConstructorHelpers::FObjectFinder<UStaticMesh> SM_GUN(TEXT("StaticMesh'/Game/FPS_Weapon_Bundle/Weapons/Meshes/KA74U/SM_KA74U_X.SM_KA74U_X'"));
// 	if (SM_GUN.Succeeded())
// 	{
		//Gun->SetStaticMesh(SM_GUN.Object);
		//Gun->SetupAttachment(GetMesh(), TEXT("GunSocket"));
		//Gun->SetSimulatePhysics(false);
		//Gun->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
//	}
	
	// Set Character.
	bUseControllerRotationYaw = true;

	// Set Character Animation.
	ConstructorHelpers::FClassFinder<UCPP_BaseCharacterAnim> ABP_ANIM(TEXT("AnimBlueprint'/Game/Blueprints/Animation/ABP_BaseAnim.ABP_BaseAnim_C'"));
	if (ABP_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(ABP_ANIM.Class);
	}
	
	// Set CharacterMovement.
	GetCharacterMovement()->MaxWalkSpeed = JOGSPEED;

}

// Called when the game starts or when spawned
void ACPP_BaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// SetPlayerState
	PlayerState = Cast<ACPP_BasePlayerState>(GetPlayerState());
	if (PlayerState)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerState"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("PlayerState is nullptr"));
	}

	USkeletalMeshComponent* mesh = GetMesh();
	if (mesh)
	{
		AnimInstance = Cast<UCPP_BaseCharacterAnim>(mesh->GetAnimInstance());
	}


	// Create Default Weapon(Gun)
	FActorSpawnParameters param;
	param.Owner = this;
	param.Instigator = this;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;



	// testing...
	FTransform transform;
	Gun = GetWorld()->SpawnActor<ACPP_BaseGun>(ACPP_BaseGun::StaticClass(), transform, param);
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("GunSocket"));
}

// Called every frame
void ACPP_BaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsLocallyControlled())
	{
		CS_UpdateControlRotation(GetControlRotation());
	}
	else {
		// GetMesh()->SetWorldRotation(this->ControlRotation);
		// TPPCamera->SetWorldRotation(this->ControlRotation);
	}

}

// Called to bind functionality to input
void ACPP_BaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ACPP_BaseCharacter::OnMoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ACPP_BaseCharacter::OnMoveRight);

	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ACPP_BaseCharacter::OnLookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ACPP_BaseCharacter::OnTurn);

	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Pressed, this, &ACPP_BaseCharacter::CS_OnSprint);
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Released, this, &ACPP_BaseCharacter::CS_OffSprint);

	// PlayerInputComponent->BindAction(TEXT("Walk"), IE_Pressed, this, &ACPP_BaseCharacter::CS_OnWalk);
	// PlayerInputComponent->BindAction(TEXT("Walk"), IE_Released, this, &ACPP_BaseCharacter::CS_OffWalk);

	PlayerInputComponent->BindAction(TEXT("Crouch"), IE_Pressed, this, &ACPP_BaseCharacter::CS_OnCrouch);
	PlayerInputComponent->BindAction(TEXT("Crouch"), IE_Released, this, &ACPP_BaseCharacter::CS_OffCrouch);

	PlayerInputComponent->BindAction(TEXT("Ironsights"), IE_Pressed, this, &ACPP_BaseCharacter::CS_OnIronsights);
	PlayerInputComponent->BindAction(TEXT("Ironsights"), IE_Released, this, &ACPP_BaseCharacter::CS_OffIronsights);
	
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ACPP_BaseCharacter::CS_OnFire);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Released, this, &ACPP_BaseCharacter::CS_OffFire);

	PlayerInputComponent->BindAction(TEXT("Prespective"), IE_Pressed, this, &ACPP_BaseCharacter::TogglePrespective);
	

}

void ACPP_BaseCharacter::OnMoveForward(float AxisValue)
{
	//AddMovementInput(GetActorForwardVector(), AxisValue);

	this->AddMovementInput(UKismetMathLibrary::GetForwardVector(FRotator(0, GetControlRotation().Yaw, 0)), AxisValue);

}

void ACPP_BaseCharacter::OnMoveRight(float AxisValue)
{
	//AddMovementInput(GetActorRightVector(), AxisValue);

	this->AddMovementInput(UKismetMathLibrary::GetRightVector(FRotator(0, GetControlRotation().Yaw, 0)), AxisValue);

}

void ACPP_BaseCharacter::OnLookUp(float AxisValue)
{
	AddControllerPitchInput(AxisValue);
}

void ACPP_BaseCharacter::OnTurn(float AxisValue)
{
	AddControllerYawInput(AxisValue);
}

void ACPP_BaseCharacter::CS_OnSprint_Implementation()
{
	MC_OnSprint();
}
void ACPP_BaseCharacter::MC_OnSprint_Implementation()
{
	if(IsCrouch()) { UpdateCrouchState(false);}
	if(IsIronsights()){	UpdateIronsightsState(false);}
	
	UpdateSprintState(true);

	UpdateMoveSpeed();
}

void ACPP_BaseCharacter::CS_OffSprint_Implementation()
{
	MC_OffSprint();
}

void ACPP_BaseCharacter::MC_OffSprint_Implementation()
{
	UpdateMoveSpeed();
	UpdateSprintState(false);
}

void ACPP_BaseCharacter::UpdateMoveSpeed()
{
	if (bSprint)
	{
		GetCharacterMovement()->MaxWalkSpeed = SPRINTSPEED;
	}
	else if (bCrouch)
	{
		GetCharacterMovement()->MaxWalkSpeed = CROUCHSPEED;
	}
	else if (bIronsights)
	{
		GetCharacterMovement()->MaxWalkSpeed = WALKSPEED;
	}
	else {
		GetCharacterMovement()->MaxWalkSpeed = JOGSPEED;
	}
// 
// 	if (HasAuthority())
// 	{
// 		UE_LOG(LogTemp, Warning, TEXT("Listen"));
// 	}
// 	else {
// 		UE_LOG(LogTemp, Warning, TEXT("Client"));
// 	}
// 	UE_LOG(LogTemp, Warning, TEXT("%lf"), GetCharacterMovement()->MaxWalkSpeed);

}

void ACPP_BaseCharacter::UpdateSprintState(bool Sprint)
{
	this->bSprint = Sprint;
	if(AnimInstance)
	{
		AnimInstance->bSprint = this->bSprint;
	}
}

void ACPP_BaseCharacter::CS_OnCrouch_Implementation()
{
	MC_OnCrouch();
}

void ACPP_BaseCharacter::MC_OnCrouch_Implementation()
{
	if(IsSprint()) {UpdateSprintState(false); }

	if (PlayerState && PlayerState->bCrouchToggleMode)
	{
		UpdateCrouchState(!this->bCrouch);
	}
	else {
		UpdateCrouchState(true);
	}

	UpdateMoveSpeed();
}

void ACPP_BaseCharacter::CS_OffCrouch_Implementation()
{
	MC_OffCrouch();
}

void ACPP_BaseCharacter::MC_OffCrouch_Implementation()
{
	if (PlayerState && !PlayerState->bCrouchToggleMode)
	{
		UpdateCrouchState(false);
	}
	UpdateMoveSpeed();
	
}

void ACPP_BaseCharacter::UpdateCrouchState(bool Crouch)
{
	this->bCrouch = Crouch;
	if(AnimInstance)
	{
		AnimInstance->bCrouch = this->bCrouch;
	}
}

void ACPP_BaseCharacter::CS_OnIronsights_Implementation()
{
	if(IsSprint()) {UpdateSprintState(false);}

	MC_OnIronsights();
}

void ACPP_BaseCharacter::MC_OnIronsights_Implementation()
{
	if (PlayerState && PlayerState->bIronsightsToggleMode)
	{
		UpdateIronsightsState(!bIronsights);
	}
	else {
		UpdateIronsightsState(true);
	}
	UpdateMoveSpeed();
}

void ACPP_BaseCharacter::CS_OffIronsights_Implementation()
{
	MC_OffIronsights();
}

void ACPP_BaseCharacter::MC_OffIronsights_Implementation()
{
	if (PlayerState && !PlayerState->bIronsightsToggleMode)
	{
		UpdateIronsightsState(false);
	}
	UpdateMoveSpeed();
}

void ACPP_BaseCharacter::UpdateIronsightsState(bool Ironsights)
{
	this->bIronsights = Ironsights;
	if(AnimInstance)
	{
		AnimInstance->bIronSight = this->bIronsights;
	}
}

void ACPP_BaseCharacter::CS_OnFire_Implementation()
{
	MC_OnFire();
}

void ACPP_BaseCharacter::MC_OnFire_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT(__FUNCTION__));

	FVector Forward = TPPCamera->GetForwardVector();

	Gun->OnFire(Forward + 1000, 1000);

}

void ACPP_BaseCharacter::CS_OffFire_Implementation()
{
	MC_OffFire();
}

void ACPP_BaseCharacter::MC_OffFire_Implementation()
{
	// Gun->OffFire();
}

void ACPP_BaseCharacter::CS_UpdateControlRotation_Implementation(const FRotator& rotation)
{
	MC_UpdateControlRotation(rotation);
}

void ACPP_BaseCharacter::MC_UpdateControlRotation_Implementation(const FRotator& rotation)
{
	this->ControlRotation = rotation;
}

void ACPP_BaseCharacter::TogglePrespective()
{
	bFPP = !bFPP;

	TPPCamera->SetActive(!bFPP);
	FPPCamera->SetActive(bFPP);

	bUseControllerRotationYaw = true;
}
