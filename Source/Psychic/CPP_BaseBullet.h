// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPP_BaseBullet.generated.h"

UCLASS()
class PSYCHIC_API ACPP_BaseBullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPP_BaseBullet();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void PostInitializeComponents() override;

	UFUNCTION()
	void OnImpact(const FHitResult& HitResult);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Projectile)
	class UProjectileMovementComponent*  Projectile;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Projectile)
	class USphereComponent* Collision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Projectile)
	class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Projectile)
	class UParticleSystemComponent* TrailParticle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Projectile)
	float ProjectileSpeed;

	UPROPERTY(ReplicatedUsing = OnRep_Exploded)
	bool bExploded;

	UFUNCTION()
	void OnRep_Exploded();

	void Explode(const FHitResult& Impact);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};
