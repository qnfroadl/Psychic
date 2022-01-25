// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_GamePlayHUD.h"

#include <Engine/Canvas.h>

#include "CPP_BaseCharacter.h"
#include "CPP_BasePlayerController.h"
#include "CPP_BaseCharacter.h"

#include <Kismet/GameplayStatics.h>

void ACPP_GamePlayHUD::DrawHUD()
{
	Super::DrawHUD();
	
	ACPP_BaseCharacter* Character = Cast<ACPP_BaseCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Character && false == Character->IsIronsights() && false == Character->IsSprint())
	{
		DrawCrosshair();
	}

	
	

}

void ACPP_GamePlayHUD::DrawCrosshair()
{
	ACPP_BasePlayerController* PC = Cast<ACPP_BasePlayerController>(PlayerOwner);
	if (PC)
	{

		float SpreadRadius = 20.f;		// 임시 테스트 값
		const static float MinimumRadius = 2.f;
		float LineLength = 20.f;		// 임시 테스트 값

		ACPP_BaseCharacter* BaseCharacter = Cast<ACPP_BaseCharacter>(PC->GetPawn());
		if (BaseCharacter && false == BaseCharacter->IsSprint()/* && false == BaseCharacter->IsIronsights()*/)
		{
			FVector2D center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

			// Draw Top
			DrawLine(center.X, center.Y - (MinimumRadius + SpreadRadius), 
					 center.X, center.Y - (MinimumRadius + SpreadRadius) - LineLength, FColor::Red, 2.f);
	
			// Draw Left
			DrawLine(center.X - (MinimumRadius + SpreadRadius),              center.Y ,
			         center.X - (MinimumRadius + SpreadRadius) - LineLength, center.Y , FColor::Red, 2.f);

			// Draw Right
			DrawLine(center.X + (MinimumRadius + SpreadRadius), center.Y,
					 center.X + (MinimumRadius + SpreadRadius) + LineLength, center.Y, FColor::Red, 2.f);

			// Draw Bottom
			DrawLine(center.X, center.Y + (MinimumRadius + SpreadRadius),
					 center.X, center.Y + (MinimumRadius + SpreadRadius) + LineLength,  FColor::Red, 2.f);
		}
	}
}
