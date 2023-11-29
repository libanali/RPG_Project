// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseMenu.h"

void UPauseMenu::ResumeClicked()
{

	APlayerController* PlayerController = GetOwningPlayer();

	if (PlayerController)

	{

		HidePauseMenu();

		PlayerController->SetPause(false);

	}


}

void UPauseMenu::OptionsClicked()
{



}

void UPauseMenu::InventoryClicked()
{


}


void UPauseMenu::ShowPauseMenu()
{

	SetVisibility(ESlateVisibility::Visible);

}

void UPauseMenu::HidePauseMenu()
{

	SetVisibility(ESlateVisibility::Hidden);

}

void UPauseMenu::NativeConstruct()
{
}
