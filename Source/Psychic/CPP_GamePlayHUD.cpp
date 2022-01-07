// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_GamePlayHUD.h"

#include <Engine/Canvas.h>

#include "CPP_BaseCharacter.h"
#include "CPP_BasePlayerController.h"
#include "CPP_BaseCharacter.h"

void ACPP_GamePlayHUD::DrawHUD()
{
	Super::DrawHUD();
	
	DrawCrosshair();
	

}

void ACPP_GamePlayHUD::DrawCrosshair()
{
	ACPP_BasePlayerController* PC = Cast<ACPP_BasePlayerController>(PlayerOwner);
	if (PC)
	{
		ACPP_BaseCharacter* BaseCharacter = Cast<ACPP_BaseCharacter>(PC->GetPawn());
		if (BaseCharacter && false == BaseCharacter->IsSprint() && false == BaseCharacter->IsIronsights())
		{
			FVector2D center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

			DrawLine(center.X - 10, center.Y,	 center.X + 10, center.Y,		FColor::Red, 1.f);
			DrawLine(center.X,		center.Y-10, center.X,		center.Y + 10,  FColor::Red, 1.f);
		}
	}
}
