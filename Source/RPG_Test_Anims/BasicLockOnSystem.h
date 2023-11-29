// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasicLockOnSystem.generated.h"

UCLASS()
class RPG_TEST_ANIMS_API ABasicLockOnSystem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABasicLockOnSystem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UFUNCTION()
		void LockOn();

	UFUNCTION()
		void Unlock();


private:

	UPROPERTY(EditAnywhere)
		float LockOnDistance = 1000.0f;


	UPROPERTY()
		class AActor* LockedTarget = nullptr;


	UPROPERTY()
		class APlayerController* PlayerController = nullptr;




};
