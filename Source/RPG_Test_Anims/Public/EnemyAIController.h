// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "RPG_Character.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class RPG_TEST_ANIMS_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
public:

		virtual void BeginPlay() override;

		AEnemyAIController();




protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
		class UBlackboardComponent* BlackboardComponent;


	virtual void OnPossess(APawn* InPawn) override;
	
};
