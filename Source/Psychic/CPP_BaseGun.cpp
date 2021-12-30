// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_BaseGun.h"

#include <Components/StaticMeshComponent.h>
#include <Components/SceneComponent.h>

// Sets default values
ACPP_BaseGun::ACPP_BaseGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->Gun = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gun"));
	this->Muzzle = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle"));

	ConstructorHelpers::FObjectFinder<UStaticMesh> SM_BASEGUN(TEXT("StaticMesh'/Game/FPS_Weapon_Bundle/Weapons/Meshes/KA74U/SM_KA74U_X.SM_KA74U_X'"));
	if (SM_BASEGUN.Succeeded())
	{
		this->Gun->SetStaticMesh(SM_BASEGUN.Object);
	}

	// ing...
	// Set CollisionPreset.
	this->Gun->SetCollisionProfileName(TEXT("CharacterMesh"));




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

