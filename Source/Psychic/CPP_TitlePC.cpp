// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_TitlePC.h"

#include "CPP_TitleHUD.h"

ACPP_TitlePC::ACPP_TitlePC()
:APlayerController(), TitleHUD(nullptr)
{
	
}

void ACPP_TitlePC::BeginPlay()
{
	Super::BeginPlay();
	
	TitleHUD = GetHUD<ACPP_TitleHUD>();

	SetInputMode(FInputModeUIOnly());

	
}
