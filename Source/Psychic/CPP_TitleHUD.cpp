// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_TitleHUD.h"

#include "CPP_TitleWidget.h"

ACPP_TitleHUD::ACPP_TitleHUD()
{
	ConstructorHelpers::FClassFinder<UCPP_TitleWidget> WidgetClass (TEXT("WidgetBlueprint'/Game/Blueprints/1_Title/BP_TitleWidget.BP_TitleWidget_C'"));
	if (WidgetClass.Succeeded())
	{
		TitleWidgetClass = WidgetClass.Class;
	}
}

void ACPP_TitleHUD::BeginPlay()
{
	if (TitleWidgetClass->GetClass())
	{
		auto widget = CreateWidget(GetWorld(), TitleWidgetClass->GetClass(), FName(TEXT("TitleWidget")));
		TitleWidget = Cast<UCPP_TitleWidget>(widget);
		if (TitleWidget)
		{
			TitleWidget->AddToViewport();
		}
	}

}
