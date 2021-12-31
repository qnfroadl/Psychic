// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPP_BaseGun.generated.h"

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* Gun;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneComponent* Muzzle;
// 
// 	bool OnFire(const FVector& EndLocation);
// 
// 	bool OnReload();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int CurrentAmmo;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int RemainingAmmo;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int MaxMagazine;

};
