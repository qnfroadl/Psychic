// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_TitleWidget.h"

#include <Components/Button.h>
#include <Components/EditableText.h>
#include <Kismet/KismetSystemLibrary.h>

#include "CPP_TitlePC.h"

void UCPP_TitleWidget::NativeConstruct()
{
	Super::NativeConstruct();

	this->Exit->OnClicked.AddDynamic(this, &UCPP_TitleWidget::OnExitCliced);
	this->Server->OnClicked.AddDynamic(this, &UCPP_TitleWidget::OnServerClicked);
}

void UCPP_TitleWidget::OnExitCliced()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, false);
}

void UCPP_TitleWidget::OnServerClicked()
{
	ACPP_TitlePC* PC = GetOwningPlayer<ACPP_TitlePC>();
	if (PC)
	{
		PC->OnSetName(this->NameText->GetText().ToString());
		PC->OnStartServer();
	}
	else 
	{
		
	}
}

void UCPP_TitleWidget::OnJoinClicked()
{
	ACPP_TitlePC* PC = GetOwningPlayer<ACPP_TitlePC>();
	if (PC)
	{
		const FString InputIPText = this->IPtext->GetText().ToString();

		// IP 검증 단계 필요

		PC->OnSetName(this->NameText->GetText().ToString());
		PC->OnConnectServer(InputIPText);
	
	}
	else 
	{

	}
}
