// Copyright Epic Games, Inc. All Rights Reserved.


#include "PsychicGameModeBase.h"

#include "CPP_BaseCharacter.h"
#include "CPP_BasePlayerState.h"
#include "CPP_BasePlayerController.h"
#include "CPP_GamePlayHUD.h"
#include "CPP_BaseGun.h"

APsychicGameModeBase::APsychicGameModeBase()
{
// 	ConstructorHelpers::FClassFinder<ACPP_BaseCharacter>CHARACTER_CLASS (TEXT("Blueprint'/Game/Blueprints/BP_BaseCharacter.BP_BaseCharacter_C'"));
// 
// 	if(CHARACTER_CLASS.Succeeded())
// 	{
// 		DefaultPawnClass = CHARACTER_CLASS.Class;
// 		PlayerStateClass = ACPP_BasePlayerState::StaticClass();
// 		PlayerControllerClass = ACPP_BasePlayerController::StaticClass();
// 		HUDClass = ACPP_GamePlayHUD::StaticClass();
// 	}
// 	else 
// 	{
		DefaultPawnClass = ACPP_BaseCharacter::StaticClass();
		PlayerStateClass = ACPP_BasePlayerState::StaticClass();
		PlayerControllerClass = ACPP_BasePlayerController::StaticClass();
		HUDClass = ACPP_GamePlayHUD::StaticClass();
	//}


	// testing...
	ConstructorHelpers::FClassFinder<ACPP_BaseGun>BP_BASEGUN(TEXT("Blueprint'/Game/Blueprints/BP_BaseGun.BP_BaseGun_C'"));
	if (BP_BASEGUN.Succeeded())
	{
		BaseGunClass = BP_BASEGUN.Class;
	}
}

void APsychicGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	// Create Default Weapon(Gun) testing...
	FActorSpawnParameters param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FTransform transform;
	auto SpawnGun = GetWorld()->SpawnActor<ACPP_BaseGun>(BaseGunClass/*ACPP_BaseGun::StaticClass()*/, transform, param);
	
	// Init gun.
	SpawnGun->SetRPM(600);
	ACPP_BaseCharacter* Player = Cast< ACPP_BaseCharacter>(NewPlayer->GetCharacter());
	if (Player)
	{
		UE_LOG(LogTemp, Log, TEXT("Player SpawnGun"));
		Player->SetGun(SpawnGun);
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("Player is nullptr"));
	}

}

