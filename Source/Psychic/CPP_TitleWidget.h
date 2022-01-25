// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CPP_TitleWidget.generated.h"

/**
 * 
 */
UCLASS()
class PSYCHIC_API UCPP_TitleWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta=(BindWidget))
	class UButton* Server;

	UPROPERTY(meta = (BindWidget))
	class UButton* Join;

	UPROPERTY(meta = (BindWidget))
	class UButton* Exit;

	UPROPERTY(meta = (BindWidget))
	class UEditableText* IPtext;

	UPROPERTY(meta = (BindWidget))
	class UEditableText* NameText;

protected:
	void NativeConstruct() override;

public:
	UFUNCTION()
	void OnExitCliced();

	UFUNCTION()
	void OnServerClicked();

	UFUNCTION()
	void OnJoinClicked();

	
};
