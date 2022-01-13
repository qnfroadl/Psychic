// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_BaseGun.h"

#include <Components/StaticMeshComponent.h>
#include <Components/SceneComponent.h>
#include <Camera/CameraComponent.h>
#include <Kismet/GameplayStatics.h>
#include <Particles/ParticleSystem.h>
#include <Kismet/KismetMathLibrary.h>
#include <Engine/SkeletalMesh.h>

#include "CPP_BaseCharacter.h"
#include "CPP_BaseBullet.h"

#include <DrawDebugHelpers.h>

// Sets default values
ACPP_BaseGun::ACPP_BaseGun()
:SK_Gun(nullptr), CurrentAmmo(0), RemainingAmmo(0), MaxMagazine(0), MuzzleSocketName(TEXT("Muzzle")), ScopeCameraSocketName(TEXT("ScopeCamera"))
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

// 	this->Gun = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gun"));
// 	ConstructorHelpers::FObjectFinder<UStaticMesh> SM_BASEGUN(TEXT("StaticMesh'/Game/FPS_Weapon_Bundle/Weapons/Meshes/KA74U/SM_KA74U_X.SM_KA74U_X'"));
// 	if (SM_BASEGUN.Succeeded())
// 	{
// 		this->Gun->SetStaticMesh(SM_BASEGUN.Object);
// 	}
// 
// 
// 	// Set CollisionPreset.
// 	this->Gun->SetCollisionProfileName(TEXT("CharacterMesh"));


	this->SK_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SK_Gun"));
	ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_BASEGUN(TEXT("SkeletalMesh'/Game/FPS_Weapon_Bundle/Weapons/Meshes/KA74U/SK_KA74U_X.SK_KA74U_X'"));
	if (SK_BASEGUN.Succeeded())
	{
		this->SK_Gun->SetSkeletalMesh(SK_BASEGUN.Object);
	}
	this->SK_Gun->SetCollisionProfileName(TEXT("CharacterMesh"));



	ConstructorHelpers::FObjectFinder<UParticleSystem> PS_EXPLOSION(TEXT("ParticleSystem'/Game/StarterContent/Particles/P_Explosion.P_Explosion'"));
	if (PS_EXPLOSION.Succeeded())
	{
		Explosion = PS_EXPLOSION.Object;
	}

}

// Called when the game starts or when spawned
void ACPP_BaseGun::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACPP_BaseGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACPP_BaseGun::OnFire(const FVector& EndLocation, float Distance)
{
	OnFireEffect();
	
	FVector MuzzleLocation = this->SK_Gun->GetSocketLocation(MuzzleSocketName);
	FVector  Direction;// = this->Gun->GetSocketTransform(MuzzleSocketName).Rotator().Vector();
	FVector AimEndLocation = EndLocation;

	Direction = UKismetMathLibrary::FindLookAtRotation(MuzzleLocation, AimEndLocation).Vector();

	CS_FireProjectile(MuzzleLocation, Direction);

}

void ACPP_BaseGun::CS_FireProjectile_Implementation(FVector StartLocation, FVector_NetQuantizeNormal Direction)
{
	FTransform SpawnTM(Direction.Rotation(), StartLocation, FVector(5,5,5));

	ACPP_BaseBullet* Bullet = Cast<ACPP_BaseBullet>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, ACPP_BaseBullet::StaticClass(), SpawnTM));
	if (Bullet)
	{
		Bullet->SetInstigator(GetInstigator());
		Bullet->SetOwner(this);
		// Bullet->InitVelocity(Direction);

		UGameplayStatics::FinishSpawningActor(Bullet, SpawnTM);

		DrawDebugLine(GetWorld(), StartLocation, StartLocation + Direction * 10000.f, FColor::Blue, false, 10.f, 0, 1.f);
	}
}

void ACPP_BaseGun::OnFireEffect()
{
	//FTransform MuzzleTransform = this->Gun->GetSocketTransform(MuzzleSocketName);
	FVector MuzzleLocation = this->SK_Gun->GetSocketLocation(MuzzleSocketName);

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Explosion, FTransform(FRotator(), MuzzleLocation, FVector(0.1f)), true);
}

void ACPP_BaseGun::SetOwningPawn(ACPP_BaseCharacter* NewOwner)
{
	if (this->BaseCharacter != NewOwner)
	{
		SetInstigator(NewOwner);
		BaseCharacter = NewOwner;
		
		SetOwner(NewOwner);
	}
}

