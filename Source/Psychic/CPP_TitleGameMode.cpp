// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_TitleGameMode.h"

#include "CPP_TitleHUD.h"
#include "CPP_TitlePC.h"

ACPP_TitleGameMode::ACPP_TitleGameMode()
{

	this->HUDClass = ACPP_TitleHUD::StaticClass();
	this->PlayerControllerClass = ACPP_TitlePC::StaticClass();

}

void ACPP_TitleGameMode::BeginPlay()
{
	
}
