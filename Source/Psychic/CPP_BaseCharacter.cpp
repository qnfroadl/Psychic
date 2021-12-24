// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_BaseCharacter.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "CPP_BaseCharacterAnim.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
ACPP_BaseCharacter::ACPP_BaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_MANNEQUIN(TEXT("SkeletalMesh'/Game/Mannequin/Character/Mesh/SK_Mannequin.SK_Mannequin'"));
	
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
	GetMesh()->SetAnimInstanceClass(UCPP_BaseCharacterAnim::StaticClass());
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

}

// Called to bind functionality to input
void ACPP_BaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ACPP_BaseCharacter::OnMoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ACPP_BaseCharacter::OnMoveRight);

	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ACPP_BaseCharacter::OnLookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ACPP_BaseCharacter::OnTurn);

}

void ACPP_BaseCharacter::OnMoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector(), AxisValue);
}

void ACPP_BaseCharacter::OnMoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector(), AxisValue);
}

void ACPP_BaseCharacter::OnLookUp(float AxisValue)
{
	AddControllerPitchInput(AxisValue);
}

void ACPP_BaseCharacter::OnTurn(float AxisValue)
{

	AddControllerYawInput(AxisValue);
}

