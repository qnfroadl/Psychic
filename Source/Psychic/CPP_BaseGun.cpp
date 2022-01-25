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
#include <Net/UnrealNetwork.h>

// Sets default values
ACPP_BaseGun::ACPP_BaseGun()
:SK_Gun(nullptr), CurrentAmmo(0), RemainingAmmo(0), MaxMagazine(0), MuzzleSocketName(TEXT("Muzzle")), 
ScopeCameraSocketName(TEXT("ScopeCamera")), CurrentPawn(nullptr), CurrentState(EGunState::Idle), BursterCounter(0),
RPM(600)
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
	bReplicates = true;

	this->SK_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SK_Gun"));
	ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_BASEGUN(TEXT("SkeletalMesh'/Game/FPS_Weapon_Bundle/Weapons/Meshes/Ka47/SK_KA47_X.SK_KA47_X'"));
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

	if (HasAuthority())
	{
		UE_LOG(LogTemp, Log, TEXT("Gun has authority"));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Gun has not authority"));

	}

}

// Called every frame
void ACPP_BaseGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACPP_BaseGun::StartFire(const FVector& EndLocation, float Distance)
{

	if (false == HasAuthority())
	{
		ServerStartFire(EndLocation, Distance);
	}
	else
	{
		// Start Timer. auto Rifle



		FVector MuzzleLocation = this->SK_Gun->GetSocketLocation(MuzzleSocketName);
		FVector  Direction;// = this->Gun->GetSocketTransform(MuzzleSocketName).Rotator().Vector();
		FVector AimEndLocation = EndLocation;

		Direction = UKismetMathLibrary::FindLookAtRotation(MuzzleLocation, AimEndLocation).Vector();

		ServerFireProjectile(MuzzleLocation, Direction);

		BursterCounter++;
		OnRep_BurstCounter();	// for server
	}

}

void ACPP_BaseGun::ServerStartFire_Implementation(const FVector& EndLocation, float Distance)
{
	StartFire(EndLocation, Distance);
}

void ACPP_BaseGun::StopFire()
{
	ServerStopFire();
}

void ACPP_BaseGun::ServerStopFire_Implementation()
{
	BursterCounter = 0;
}

void ACPP_BaseGun::ServerFireProjectile_Implementation(FVector StartLocation, FVector_NetQuantizeNormal Direction)
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
	UE_LOG(LogTemp, Log, TEXT(__FUNCTION__));

	//FTransform MuzzleTransform = this->Gun->GetSocketTransform(MuzzleSocketName);
	//FVector MuzzleLocation = this->SK_Gun->GetSocketLocation(MuzzleSocketName);

	// UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Explosion, FTransform(FRotator(), MuzzleLocation, FVector(0.1f)), true);
	UGameplayStatics::SpawnEmitterAttached(Explosion, this->SK_Gun, MuzzleSocketName);
}

void ACPP_BaseGun::SetOwningPawn(APawn* NewOwner)
{
	if (this->CurrentPawn != NewOwner)
	{
		SetInstigator(NewOwner);
		CurrentPawn = NewOwner;
		
		SetOwner(NewOwner);
	}
}

void ACPP_BaseGun::OnRep_BurstCounter()
{
	if (0 < BursterCounter)
	{
			// 애니메이션, 소리, 이펙트, 카메라 흔들림?
			OnFireEffect();
			// PlaySount
			// PlayAnimation
	}
	else {
		// stop.
	}
}


void ACPP_BaseGun::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACPP_BaseGun, BursterCounter);

}