// Fill out your copyright notice in the Description page of Project Settings.


#include "Heal_AnimNotify.h"
#include "RPG_Character.h"

void UHeal_AnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{

	AActor* Owner = MeshComp->GetOwner();

	ARPG_Character* RPGCharacter = Cast<ARPG_Character>(Owner);

	if (RPGCharacter)
	{

		RPGCharacter->StartHealing();

	}

}
