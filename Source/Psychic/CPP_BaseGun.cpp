// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_BaseGun.h"

#include <Components/StaticMeshComponent.h>
#include <Components/SceneComponent.h>
#include <Kismet/GameplayStatics.h>
#include <Particles/ParticleSystem.h>
#include <Kismet/KismetMathLibrary.h>

#include "CPP_BaseCharacter.h"
#include "CPP_BaseBullet.h"


// Sets default values
ACPP_BaseGun::ACPP_BaseGun()
:Gun(nullptr), CurrentAmmo(0), RemainingAmmo(0), MaxMagazine(0), MuzzleSocketName(TEXT("Muzzle"))
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->Gun = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gun"));

	ConstructorHelpers::FObjectFinder<UStaticMesh> SM_BASEGUN(TEXT("StaticMesh'/Game/FPS_Weapon_Bundle/Weapons/Meshes/KA74U/SM_KA74U_X.SM_KA74U_X'"));
	if (SM_BASEGUN.Succeeded())
	{
		this->Gun->SetStaticMesh(SM_BASEGUN.Object);
	}

	// Set CollisionPreset.
	this->Gun->SetCollisionProfileName(TEXT("CharacterMesh"));


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
	
	FVector MuzzleLocation = this->Gun->GetSocketLocation(MuzzleSocketName);
	FRotator Direction = this->Gun->GetSocketTransform(MuzzleSocketName).Rotator();

	const float ProjectileAdjustRange = 10000.0f;

	if (BaseCharacter)
	{	
		const FVector Start = BaseCharacter->GetCameraLocation();
		const FVector End = Start + (BaseCharacter->GetCameraForward() * ProjectileAdjustRange);
		
		FHitResult HitResult;
		GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility);

		Direction = UKismetMathLibrary::FindLookAtRotation(MuzzleLocation, End);
	}

	CS_FireProjectile(MuzzleLocation, Direction.Vector());

}

void ACPP_BaseGun::CS_FireProjectile_Implementation(FVector StartLocation, FVector_NetQuantizeNormal Direction)
{
	UE_LOG(LogTemp, Warning, TEXT(__FUNCTION__));

	StartLocation = StartLocation + Direction * 30;	//testing...

	FTransform SpawnTM(Direction.Rotation(), StartLocation);
	ACPP_BaseBullet* Bullet = Cast<ACPP_BaseBullet>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, ACPP_BaseBullet::StaticClass(), SpawnTM));
	if (Bullet)
	{
		Bullet->SetInstigator(GetInstigator());
		Bullet->SetOwner(this);
		Bullet->InitVelocity(Direction);

		UGameplayStatics::FinishSpawningActor(Bullet, SpawnTM);
	}

}

void ACPP_BaseGun::OnFireEffect()
{
	FTransform MuzzleTransform = this->Gun->GetSocketTransform(MuzzleSocketName);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Explosion, MuzzleTransform, true);
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

