// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_BasePlayerController.h"

#include "CPP_BaseCameraManager.h"

ACPP_BasePlayerController::ACPP_BasePlayerController()
{
	PlayerCameraManagerClass = ACPP_BaseCameraManager::StaticClass();
	
}
