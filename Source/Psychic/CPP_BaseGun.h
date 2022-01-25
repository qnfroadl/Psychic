// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPP_BaseGun.generated.h"


UENUM(BlueprintType)
enum class EGunState : uint8 
{
	Idle UMETA(DisplayName="Idle"),
	Reload UMETA(DisplayName = "Reload"),
	Fireing UMETA(DisplayName = "Fireing"),
};

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
	void StartFire();

	UFUNCTION(Server, Reliable)
	void ServerStartFire();

	void StopFire();

	UFUNCTION(Server, Reliable)
	void ServerStopFire();

	UFUNCTION(Server, Reliable)
	void ServerFireProjectile(FVector StartLocation, FVector_NetQuantizeNormal Direction);

	UFUNCTION()
	void OnFire();
	UFUNCTION()
	void OnReFire();

	void OnFireEffect();
	void OnRecoil();

	void SetOwningPawn(APawn* NewOwner);

	UFUNCTION()
	void OnRep_BurstCounter();

// 	bool OnReload();
	UFUNCTION()
	void SetRPM(uint32 _RPM);

	virtual FVector GetAdjustedAim() const;


public:
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

	APawn* CurrentPawn;

	EGunState CurrentState;

	UPROPERTY(ReplicatedUsing = OnRep_BurstCounter)
	uint32 BurstCounter;
	
	FTimerHandle OnFireHandle;


private:
	uint32 RPM;
	float BetweenFireTime;
	float LastFireTime;
	float TimerInterval;
};
