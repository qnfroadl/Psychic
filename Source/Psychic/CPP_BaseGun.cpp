// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_BaseGun.h"

#include <Components/StaticMeshComponent.h>
#include <Components/SceneComponent.h>
#include <Kismet/GameplayStatics.h>
#include <Particles/ParticleSystem.h>


// Sets default values
ACPP_BaseGun::ACPP_BaseGun()
:Gun(nullptr), CurrentAmmo(0), RemainingAmmo(0), MaxMagazine(0)
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

bool ACPP_BaseGun::OnFire(const FVector& EndLocation, float Distance)
{

	// FVector SocketLocation = this->Gun->GetSocketLocation(TEXT("Muzzle"));
	FTransform SocketTransform = this->Gun->GetSocketTransform(TEXT("Muzzle"));

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Explosion, SocketTransform, true);
	
	return true;
}

