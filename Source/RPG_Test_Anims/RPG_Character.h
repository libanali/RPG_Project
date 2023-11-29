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
#include "Components/TimelineComponent.h"
#include "Public/Character_Attributes_By_Level.h"
#include "Engine/DataTable.h"
#include "InventoryComponent.h"
#include "RPG_Character.generated.h"


class UAnimMontage;

UCLASS()
class RPG_TEST_ANIMS_API ARPG_Character : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARPG_Character();




	//Character Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
		USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
		UCameraComponent* Camera;
	//Character Components






	//Combat Variables
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
		bool IsCombatModeOn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		bool bIsCasting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		bool CanCastElement;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void OnCastingFinshed();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		bool Blocking;

	UFUNCTION(BlueprintCallable, Category = "Combat")
		void InflictDamageOnEnemy(AEnemy* Enemy);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		float CalculatedDamage;
	//Combat Variables





	//Movement Variables/Functions
	void MoveForward(float Axis);
	void MoveRight(float Axis);
	//Movement Variables/Functions



	//Lock-On Variables
	void ToggleHardLock();
	void ToggleSoftLock();
	void ChangingHardLockedTargets();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lock On")
	float HeightTargetingOffest;

	UFUNCTION(BlueprintImplementableEvent, Category = "Lock On")
		void ToggleHardLockOnEffects();

	UFUNCTION(BlueprintImplementableEvent, Category = "Lock On")
		void ToggleSoftLockOnEffects();

	UFUNCTION(BlueprintCallable, Category = "Lock On")
		void TurnOffHardLock();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Lock On")
		bool bIsSoftLockEnabled;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Lock On")
		float SoftLockRange;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Lock On")
		float SoftLockAngle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Lock On")
		AActor* SoftLockedEnemy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lock On")
		AActor* HardLockedActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lock On")
		TArray<AActor*> LockOnCandidates;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Lock On")
		bool bIsHardLockedOn;

	float MaxHardLockRange;
	//Lock-On Variables







	//Health Variables
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
		float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
		bool bIsdead;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		float CurrentHealth;

	UFUNCTION(BlueprintCallable, Category = "Health")
		void TakeDamage(float DamageAmount);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
		float DamageReduction;

	UFUNCTION()
		void Heal(float HealAmount);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
		UAnimMontage* HealingAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
		UAnimMontage* DeathAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
		UAnimMontage* BlockingHitAnimation;

	UFUNCTION(BlueprintCallable, Category = "Health")
		void StartHealing();	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		bool isPoisoned;

	UFUNCTION(BlueprintCallable, Category = "Health")
		void Poisoned();

	UFUNCTION(BlueprintCallable, Category = "Health")
		void PoisonedTime();

	UFUNCTION(BlueprintCallable, Category = "Health")
		void StopPoison();

	FTimerHandle PosionedTimer;
	FTimerHandle StopPoisonTimer;
	//Health Variables






	//Mana Variables
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mana")
		float MaxMana;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mana")
		float CurrentMana;

	UFUNCTION(BlueprintCallable, Category = "Mana")
		void IncreaseMana(float MPAmount);

	UFUNCTION(BlueprintCallable, Category = "Mana")
		void StartManaIncrease();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
		UAnimMontage* IncreaseManaAnimation;
	//Mana Variables






	//Inventory variables
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
		class UInventoryComponent* CharacterInventory;
	//Inventory variables






	//Attack Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
		float BaseAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
		float AttackMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
		float TotalAttack;

	UFUNCTION(BlueprintCallable, Category = "Attack")
		void CalculateTotalAttack();
	//Attack Variables






	//Defence Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defence")
		float BaseDefence;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defence")
		float DefenceMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defence")
		float TotalDefence;

	UFUNCTION(BlueprintCallable, Category = "Defence")
		void CalculateTotalDefence();
	//Defence Variables






	//Elemental Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Elemental")
		float BaseElementalAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Elemental")
		float ElementalAttackMultiplier;

	UPROPERTY(BlueprintReadOnly, Category = "Elemental")
		float TotalElementalAttack;

	UFUNCTION(BlueprintCallable, Category = "Elemental")
		void CalculateTotalElemetnalAttack();


	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Elemental")
		float BaseElementalDefence;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Elemental")
		float ElementalDefenceMultiplier;

	UPROPERTY(BlueprintReadOnly, Category = "Elemental")
		float TotalElementalDefence;

	UFUNCTION(BlueprintCallable, Category = "Elemental")
		void CalculateTotalElementalDefence();
	//Elemental Variables






	//Level Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
		int32 CharacterLevel;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
		int32 ExperiencePoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
		TArray<int32> ExperienceRequired;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Level")
		UDataTable* CharacterAttributesTable;

	UFUNCTION(BlueprintCallable, Category = "Level")
		void GainExperience(int32 ExpAmount);

	UFUNCTION(BlueprintCallable, Category = "Level")
		void CheckAndTriggerLevelUp();
	//Level Variables



	//UI Variables
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<class UPauseMenu> PauseMenuWidgetClass;

	class UPauseMenu* PauseMenuWidgeInstant;

	void TogglePauseMenu();
	//UI Variables



	UFUNCTION()
		void OnBeginOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	


	void ButtonPressForGainingExperience();

private:

	int32 FirstTargetIndex;

	FTimerHandle CastTimer;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
