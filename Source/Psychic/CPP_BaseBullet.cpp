// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_BaseBullet.h"

#include <Components/SphereComponent.h>
#include <Particles/ParticleSystemComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>
#include <Net/UnrealNetwork.h>

// Sets default values
ACPP_BaseBullet::ACPP_BaseBullet()
	:ProjectileSpeed(1000.f), bExploded(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	
	// Collision->bTraceComplexOnMove = true;
// 	Collision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

//	ECC_GameTraceChannel2 라고 쓰지말고 따로 define해서 쓰는게 더 명확.
// 	Collision->SetCollisionObjectType(ECC_GameTraceChannel2);	
// 	Collision->SetCollisionResponseToAllChannels(ECR_Ignore);
// 	Collision->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
// 	Collision->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
// 	Collision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	RootComponent = Collision;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	ConstructorHelpers::FObjectFinder<UStaticMesh>SM_AMMO (TEXT("StaticMesh'/Game/FPS_Weapon_Bundle/Weapons/Meshes/Ammunition/SM_Shell_762x39.SM_Shell_762x39'"));
	if (SM_AMMO.Succeeded())
	{
		Mesh->SetStaticMesh(SM_AMMO.Object);
		Mesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
		Mesh->SetGenerateOverlapEvents(true);
		Mesh->SetCollisionProfileName(TEXT("OverlapAll"));
	}
	Mesh->SetupAttachment(RootComponent);

	// Set ProjectileParticle
	TrailParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("TrailParticle"));
	TrailParticle->bAutoActivate = false;
	TrailParticle->bAutoDestroy = false;
	TrailParticle->SetupAttachment(GetRootComponent());

	// Set Projectile 
	Projectile = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile"));
	Projectile->SetUpdatedComponent(GetRootComponent());
	Projectile->InitialSpeed = ProjectileSpeed;
	Projectile->MaxSpeed = ProjectileSpeed;
	Projectile->bRotationFollowsVelocity = true;
	Projectile->ProjectileGravityScale = 0.f;

	// Set Replicate
	bReplicates = true;
	SetReplicatingMovement(true);
}

// Called every frame
void ACPP_BaseBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACPP_BaseBullet::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	Projectile->OnProjectileStop.AddDynamic(this, &ACPP_BaseBullet::OnImpact);
	Collision->MoveIgnoreActors.Add(GetInstigator());
	Mesh->MoveIgnoreActors.Add(GetInstigator());

	SetLifeSpan(10.f);

}

void ACPP_BaseBullet::OnImpact(const FHitResult& HitResult)
{
	UE_LOG(LogTemp, Warning, TEXT(__FUNCTION__));

	if (HasAuthority() && !bExploded)
	{
		Explode(HitResult);

		// Play Sound
// 		UAudioComponent* ProjAudioComp = FindComponentByClass<UAudioComponent>();
// 		if (ProjAudioComp && ProjAudioComp->IsPlaying())
// 		{
// 			ProjAudioComp->FadeOut(0.1f, 0.f);
// 		}

		Projectile->StopMovementImmediately();

		// 이펙트 보여질 시간 남김.
		SetLifeSpan(2.0f);
	}
}

void ACPP_BaseBullet::OnRep_Exploded()
{

}

void ACPP_BaseBullet::Explode(const FHitResult& Impact)
{

}

void ACPP_BaseBullet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACPP_BaseBullet, bExploded);

}

