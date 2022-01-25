// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_TitlePC.h"

#include "CPP_TitleHUD.h"

ACPP_TitlePC::ACPP_TitlePC()
:APlayerController(), TitleHUD(nullptr)
{
	// ConstructorHelpers::FClassFinder<ACPP_TitleHUD>
}

void ACPP_TitlePC::BeginPlay()
{
	Super::BeginPlay();
	
	TitleHUD = GetHUD<ACPP_TitleHUD>();

	SetShowMouseCursor(true);
	SetInputMode(FInputModeUIOnly());
	
}

void ACPP_TitlePC::OnStartServer()
{
	
}

void ACPP_TitlePC::OnConnectServer(const FString& IPText)
{
	
}

void ACPP_TitlePC::OnSetName(const FString& IPText)
{

}
