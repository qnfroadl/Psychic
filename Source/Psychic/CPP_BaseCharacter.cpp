// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_BaseCharacter.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "CPP_BaseCharacterAnim.h"
#include "Components/SkeletalMeshComponent.h"
#include "Gameframework/CharacterMovementComponent.h"

#include "Net/UnrealNetwork.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
ACPP_BaseCharacter::ACPP_BaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_MANNEQUIN(TEXT("SkeletalMesh'/Game/Mannequin/Mesh/SK_Mannequin.SK_Mannequin'"));
	
	if (SK_MANNEQUIN.Succeeded())
	{	
		GetMesh()->SetSkeletalMesh(SK_MANNEQUIN.Object);
		GetMesh()->SetRelativeLocation(FVector(0,0,-90));
		GetMesh()->SetRelativeRotation(FRotator(0,-90,0));	// pitch, yaw, roll ¼ø¼­.
	}

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	
	SpringArm->SetupAttachment(GetRootComponent());
	Camera->SetupAttachment(SpringArm);
	
	// Set SpringArm.
	SpringArm->SetRelativeRotation(FRotator(-20, 0, 0));
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->SocketOffset = this->NormalSocketOffset;

	// Set Camera.
	Camera->SetFieldOfView(this->NormalFOV);

	// Set Character.
	bUseControllerRotationYaw = true;

	// Set Character Animation.
	ConstructorHelpers::FClassFinder<UCPP_BaseCharacterAnim> ABP_ANIM(TEXT("AnimBlueprint'/Game/Blueprints/Animation/ABP_BaseAnim.ABP_BaseAnim_C'"));
	if (ABP_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(ABP_ANIM.Class);
	}
	
	// Set CharacterMovement;
	GetCharacterMovement()->MaxWalkSpeed = MOVESPEED;
}

// Called when the game starts or when spawned
void ACPP_BaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
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
		Camera->SetWorldRotation(this->ControlRotation);
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

	PlayerInputComponent->BindAction(TEXT("Run"), IE_Pressed, this, &ACPP_BaseCharacter::OnRun);
	PlayerInputComponent->BindAction(TEXT("Run"), IE_Released, this, &ACPP_BaseCharacter::OffRun);
	PlayerInputComponent->BindAction(TEXT("Walk"), IE_Pressed, this, &ACPP_BaseCharacter::OnWalk);
	PlayerInputComponent->BindAction(TEXT("Walk"), IE_Released, this, &ACPP_BaseCharacter::OffWalk);

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


void ACPP_BaseCharacter::OnRun()
{	
	CS_OnRun();
}
void ACPP_BaseCharacter::CS_OnRun_Implementation()
{
	MC_OnRun();
}
void ACPP_BaseCharacter::MC_OnRun_Implementation()
{
	bRun = true;
	MoveSpeedUpdate();
}

void ACPP_BaseCharacter::OffRun()
{
	CS_OffRun();
}

void ACPP_BaseCharacter::CS_OffRun_Implementation()
{
	MC_OffRun();
}

void ACPP_BaseCharacter::MC_OffRun_Implementation()
{
	bRun = false;
	MoveSpeedUpdate();
}

void ACPP_BaseCharacter::OnWalk()
{
	CS_OnWalk();
}

void ACPP_BaseCharacter::CS_OnWalk_Implementation()
{
	MC_OnWalk();
}

void ACPP_BaseCharacter::MC_OnWalk_Implementation()
{
	bWalk = true;
	MoveSpeedUpdate();
}

void ACPP_BaseCharacter::OffWalk()
{
	CS_OffWalk();
}

void ACPP_BaseCharacter::CS_OffWalk_Implementation()
{
	MC_OffWalk();
}

void ACPP_BaseCharacter::MC_OffWalk_Implementation()
{
	bWalk = false;
	MoveSpeedUpdate();
}

void ACPP_BaseCharacter::MoveSpeedUpdate()
{
	UE_LOG(LogTemp, Warning, TEXT(__FUNCTION__));

	if (bRun)
	{
		GetCharacterMovement()->MaxWalkSpeed = RUNSPEED;
	}
	else if (bWalk)
	{
		GetCharacterMovement()->MaxWalkSpeed = WALKSPEED;
	}
	else {
		GetCharacterMovement()->MaxWalkSpeed = MOVESPEED;
	}

	if (HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Listen"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Client"));
	}
	UE_LOG(LogTemp, Warning, TEXT("%lf"), GetCharacterMovement()->MaxWalkSpeed);
}

void ACPP_BaseCharacter::CS_UpdateControlRotation_Implementation(const FRotator& rotation)
{
	MC_UpdateControlRotation(rotation);
}

void ACPP_BaseCharacter::MC_UpdateControlRotation_Implementation(const FRotator& rotation)
{
	this->ControlRotation = rotation;
}
