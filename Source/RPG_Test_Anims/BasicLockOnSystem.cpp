// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicLockOnSystem.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABasicLockOnSystem::ABasicLockOnSystem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABasicLockOnSystem::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	
}

// Called every frame
void ABasicLockOnSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABasicLockOnSystem::LockOn()
{
}

void ABasicLockOnSystem::Unlock()
{
}

