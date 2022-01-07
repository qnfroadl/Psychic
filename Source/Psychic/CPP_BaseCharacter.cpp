// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_BaseCharacter.h"

#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <Components/SkeletalMeshComponent.h>
#include <Components/SceneComponent.h>
#include <Gameframework/CharacterMovementComponent.h>
#include <Net/UnrealNetwork.h>
#include <Kismet/KismetMathLibrary.h>
#include <Net/UnrealNetwork.h>

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
	ScopeCamera = CreateDefaultSubobject<UCameraComponent>("ScopeCamera");
	ScopeCamera->SetActive(false);

	// Set SpringArm.
	SpringArm->SetupAttachment(GetMesh());
	SpringArm->SetRelativeLocation(FVector(0.f,0.f,150.f));
	//SpringArm->SetRelativeRotation(FRotator(0, 0, 0));
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->TargetArmLength = 150.f;
	SpringArm->SocketOffset = this->NormalSocketOffset;

	// Set Camera.
	TPPCamera->SetupAttachment(SpringArm);

	FPPCamera->SetupAttachment(GetMesh(), TEXT("head"));
	FPPCamera->SetRelativeRotation(FRotator(0.f, 90.f, -90.f));
	FPPCamera->SetRelativeLocation(FVector(-5.f, 30.f, 0.f));
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




	// testing...
	ConstructorHelpers::FClassFinder<ACPP_BaseGun>BP_BASEGUN(TEXT("Blueprint'/Game/Blueprints/BP_BaseGun.BP_BaseGun_C'"));
	if (BP_BASEGUN.Succeeded())
	{
		BaseGunClass = BP_BASEGUN.Class;
	}
}

// Called when the game starts or when spawned
void ACPP_BaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	USkeletalMeshComponent* mesh = GetMesh();
	if (mesh)
	{
		AnimInstance = Cast<UCPP_BaseCharacterAnim>(mesh->GetAnimInstance());
	}

	// Create Default Weapon(Gun) testing...
	FActorSpawnParameters param;
	param.Owner = this;
	param.Instigator = this;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;


	FTransform transform;
	Gun = GetWorld()->SpawnActor<ACPP_BaseGun>(BaseGunClass/*ACPP_BaseGun::StaticClass()*/, transform, param);
	Gun->SetOwningPawn(this);
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

	PlayerInputComponent->BindAction(TEXT("Crouch"), IE_Pressed, this, &ACPP_BaseCharacter::CS_OnCrouch);
	PlayerInputComponent->BindAction(TEXT("Crouch"), IE_Released, this, &ACPP_BaseCharacter::CS_OffCrouch);

	PlayerInputComponent->BindAction(TEXT("Ironsights"), IE_Pressed, this, &ACPP_BaseCharacter::OnStartIronsights);
	PlayerInputComponent->BindAction(TEXT("Ironsights"), IE_Released, this, &ACPP_BaseCharacter::OnStopIronsights);
	
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
	// if(IsIronsights()){	UpdateIronsightsState(false);}
	
	UpdateSprintState(true);

	UpdateMoveSpeed();
}

void ACPP_BaseCharacter::CS_OffSprint_Implementation()
{
	MC_OffSprint();
}

void ACPP_BaseCharacter::MC_OffSprint_Implementation()
{
	UpdateSprintState(false);
	UpdateMoveSpeed();

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

	if (GetBasePlayerState()->bCrouchToggle)
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
	if (false == GetBasePlayerState()->bCrouchToggle)
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

void ACPP_BaseCharacter::OnStartIronsights()
{
	// 조건 체크.
	SetIronsights(true, GetBasePlayerState()->bSprintToggle);

}

void ACPP_BaseCharacter::OnStopIronsights()
{
	// 조건 체크.
	SetIronsights(false, GetBasePlayerState()->bSprintToggle);

}

void ACPP_BaseCharacter::SetIronsights(bool bIronsight, bool bToggle)
{
	this->bIronsights = bIronsight;
// 	if (AnimInstance)
// 	{
// 		AnimInstance->bIronSight = this->bIronsights;
// 	}

	if (false == HasAuthority())
	{
		ServerSetIronsights(bIronsight, bToggle);
	}
}

void ACPP_BaseCharacter::ServerSetIronsights_Implementation(bool bIronsight, bool bToggle)
{
	SetIronsights(bIronsight, bToggle);
}


void ACPP_BaseCharacter::CS_OnFire_Implementation()
{
	// 뛰는중에는 발사 금지.
	if (IsSprint())
	{
		return;
	}

	const FVector Start = this->GetCameraLocation();
	const FVector End = Start + (this->GetCameraForward() * 100000.f);
	FVector AimEndLocation = End;

	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility);
	if (HitResult.bBlockingHit)
	{
		AimEndLocation = HitResult.ImpactPoint;
	}

	MC_OnFire(AimEndLocation);
}

void ACPP_BaseCharacter::MC_OnFire_Implementation(const FVector& EndLocation)
{
	UE_LOG(LogTemp, Warning, TEXT(__FUNCTION__));

	Gun->OnFire(EndLocation, 1000);
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

UCameraComponent* ACPP_BaseCharacter::GetCurrentCamera()
{
	return bFPP? FPPCamera : TPPCamera;
}

FVector ACPP_BaseCharacter::GetCameraLocation()
{
	return GetCurrentCamera()->GetComponentLocation();
}

FVector ACPP_BaseCharacter::GetCameraForward()
{
	return GetCurrentCamera()->GetForwardVector();
}

ACPP_BasePlayerState* ACPP_BaseCharacter::GetBasePlayerState()
{
	// SetPlayerState
	if (nullptr == PlayerState)
	{
		PlayerState = Cast<ACPP_BasePlayerState>(GetPlayerState());
	}
	
	check(PlayerState);

	return PlayerState;
}

void ACPP_BaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	
	DOREPLIFETIME(ACPP_BaseCharacter, bIronsights);
}