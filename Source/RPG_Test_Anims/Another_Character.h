// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFrameWork/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/UnrealMathUtility.h"
#include "Components/SkeletalMeshComponent.h"
#include "Enemy.h"
#include "Another_Character.generated.h"

UCLASS()
class RPG_TEST_ANIMS_API AAnother_Character : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAnother_Character();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
		USpringArmComponent* CameraBoom;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
		UCameraComponent* Camera;


	void MoveForward(float Axis);
	void MoveRight(float Axis);
	//void ToggleHardLockOn();
	void ToggleSoftLockOn();
	//void ToggleCameraControl();


	bool bSoftLockOnActive;
	bool bHardLockOnActive;
	bool bAutoActivateSoftLock;
	bool bSoftLockOnPending;

	bool bPlayerControlsCamera;
	bool bShouldUpdateCameraRotation;

	float SoftLockOnMaxDistance;
	float SoftLockOnInterSpeed;
	float HardLockInterpSpeed;


	float MinArmLength;
	float MaxArmLength;
	float ArmLengthInterpSpeed;


	AEnemy* GetNearestEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
