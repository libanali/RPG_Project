// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenu.generated.h"

/**
 * 
 */
UCLASS()
class RPG_TEST_ANIMS_API UPauseMenu : public UUserWidget
{
	GENERATED_BODY()

public:


	//Widgets
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UButton* ResumeButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UButton* OptionsButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UButton* InventoryButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UTextBlock* ResumeButtonText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UTextBlock* OptionsButtonText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UTextBlock* InventoryButtonText;
	//Widgets


	//Functions
	UFUNCTION(BlueprintCallable)
		void ResumeClicked();

	UFUNCTION(BlueprintCallable)
		void OptionsClicked();

	UFUNCTION(BlueprintCallable)
		void InventoryClicked();

	UFUNCTION(BlueprintCallable)
		void ShowPauseMenu();

	UFUNCTION(BlueprintCallable)
		void HidePauseMenu();
	//Functions

protected:

	virtual void NativeConstruct() override;

};
