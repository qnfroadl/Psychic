// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPP_BaseGun.generated.h"

class ACPP_BaseCharacter;

UCLASS()
class PSYCHIC_API ACPP_BaseGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPP_BaseGun();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

// 	UPROPERTY(EditAnywhere, BlueprintReadWrite)
// 	class UStaticMeshComponent* Gun;					
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USkeletalMeshComponent* SK_Gun;

	class UParticleSystem* Explosion;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite)
// 	class USceneComponent* Muzzle;

	/** Muzzle에서 EndLocation 까지 발사.*/
 	void OnFire(const FVector& EndLocation, float Distance = 100000.f);

	UFUNCTION(Server, Reliable)
	void CS_FireProjectile(FVector StartLocation, FVector_NetQuantizeNormal Direction);

	void OnFireEffect();

	void SetOwningPawn(ACPP_BaseCharacter* NewOwner);

// 	bool OnReload();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int CurrentAmmo;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int RemainingAmmo;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int MaxMagazine;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FName MuzzleSocketName;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FName ScopeCameraSocketName;

	ACPP_BaseCharacter* BaseCharacter;
};
