// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_Character.h"
#include "Kismet/GameplayStatics.h" 
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerController.h"
#include "Curves/CurveFloat.h"
#include "Enemy.h"
#include "Animation/AnimMontage.h"
#include "Engine/DataTable.h"
#include "Blueprint/UserWidget.h"
#include "Public/Character_Attributes_By_Level.h"





// Sets default values
ARPG_Character::ARPG_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	//Capsule
	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	//Character Movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 400.0f;
	GetCharacterMovement()->AirControl = 0.1f;


	//Camera Boom
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 350.0f;
	CameraBoom->bUsePawnControlRotation = true;


	//Camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;


	//Inventory
	CharacterInventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("Character Inventory"));


	//Lock-On
	bIsSoftLockEnabled = false;
	SoftLockRange = 2300.0f;
	SoftLockAngle = 340.0f;
	SoftLockedEnemy = nullptr;
	HeightTargetingOffest = 5.0f;
	MaxHardLockRange = 2600.0f;
	FirstTargetIndex = 0;


	//Health
	MaxHealth = 60.0f;
	CurrentHealth = MaxHealth;
	bIsdead = false;
	DamageReduction = 0.3f;
	isPoisoned = false;


	//Mana
	MaxMana = 30.0f;
	CurrentMana = MaxMana;


	//Combat
	bIsCasting = false;
	Blocking = false;

	
	//Level
	CharacterLevel = 1;
	ExperienceRequired.Add(0);
	ExperienceRequired.Add(50);//Level2
	ExperienceRequired.Add(120);//Level3
	ExperienceRequired.Add(170);//Level4
	ExperienceRequired.Add(300);//Level5



	//Attack
	BaseAttack = 4.0f;
	AttackMultiplier = 3.0f;


	//Defence
	BaseDefence = 3.0f;
	DefenceMultiplier = 2.0f;
}







void ARPG_Character::MoveForward(float Axis)
{
	if (!bIsdead)

	{


		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		AddMovementInput(Direction, Axis);
	}
	

}





void ARPG_Character::MoveRight(float Axis)
{
	
	if (!bIsdead)

	{

		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(Direction, Axis);

	}
}








void ARPG_Character::ToggleHardLock()
{


	if (bIsHardLockedOn)

	{

		bIsHardLockedOn = false;
		HardLockedActor = nullptr;
		ToggleHardLockOnEffects();

	}

	else if (bIsSoftLockEnabled && SoftLockedEnemy)

	{
		bIsHardLockedOn = true;
		HardLockedActor = SoftLockedEnemy;
		ToggleHardLockOnEffects();

	}

	else

	{
		if (LockOnCandidates.Num() > 0)
		{

			HardLockedActor = LockOnCandidates[0];
			bIsSoftLockEnabled = false;

		}

	}

}





void ARPG_Character::ToggleSoftLock()
{

	TArray<AActor*> OverlappingActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName(TEXT("Enemy")), OverlappingActors);

	FVector CharacterForward = GetActorForwardVector();
	FVector CharacterLocation = GetActorLocation();

	AActor* NearestEnemy = nullptr;
	float NearestTargetDistance = SoftLockRange * SoftLockRange;

	for (AActor* Actor : OverlappingActors)
	{
		FVector TargetDirection = Actor->GetActorLocation() - CharacterLocation;
		float TargetDistance = TargetDirection.SizeSquared();
		float AngleToEnemy = FMath::Acos(FVector::DotProduct(TargetDirection.GetSafeNormal(), CharacterForward)) * (180.0f / PI);

		if (TargetDistance <= NearestTargetDistance && AngleToEnemy <= SoftLockAngle && !bIsHardLockedOn)
		{

			bIsSoftLockEnabled = true;
			NearestEnemy = Actor;
			NearestTargetDistance = TargetDistance;
			GEngine->AddOnScreenDebugMessage(1, 1.3f, FColor::Green, TEXT("Soft Lock!"));
			ToggleSoftLockOnEffects();

		}
	}

	SoftLockedEnemy = NearestEnemy;
	bIsSoftLockEnabled = SoftLockedEnemy != nullptr;


}



//Switch between Lock-On Targets

void ARPG_Character::ChangingHardLockedTargets()
{

	if (!bIsHardLockedOn && LockOnCandidates.Num() == 0)
	{
		// No targets available, exit the function
		UE_LOG(LogTemp, Warning, TEXT("ChangingHardLockedTargets: No targets available."));
		ToggleHardLockOnEffects();
		return;
	}

	int32 CurrentTargetIndex = LockOnCandidates.IndexOfByKey(HardLockedActor);

	if (CurrentTargetIndex == INDEX_NONE)
	{
		// Current target not found in the list, set to the first target
		HardLockedActor = LockOnCandidates[0];
	}
	else if (bIsHardLockedOn)
	{
		// Determine the index of the next target
		int32 NextTargetIndex = (CurrentTargetIndex + 1) % LockOnCandidates.Num();

		// Check if we've completed one full cycle
		if (NextTargetIndex == FirstTargetIndex)
		{
			// Cycle back to the second target
			NextTargetIndex = (FirstTargetIndex + 1) % LockOnCandidates.Num();
		}

		// Ensure that the next target is not destroyed
		AEnemy* NextEnemy = Cast<AEnemy>(LockOnCandidates[NextTargetIndex]);
		if (NextEnemy && !NextEnemy->IsPendingKill())
		{
			ToggleHardLockOnEffects();
			HardLockedActor = LockOnCandidates[NextTargetIndex];
			UE_LOG(LogTemp, Warning, TEXT("ChangingHardLockedTargets: Switched to target %d"), NextTargetIndex);
		}
	}
}


void ARPG_Character::TurnOffHardLock()
{

	HardLockedActor = nullptr;

	bIsHardLockedOn = false;

}





void ARPG_Character::Heal(float HealAmount)
{

	if (CurrentHealth < MaxHealth && HealAmount > 0 && !bIsCasting && IsCombatModeOn && CurrentMana > 0)

	{

		bIsCasting = true;

		CurrentHealth = FMath::Clamp(CurrentHealth + HealAmount, 0.0f, MaxHealth);

		//Subtract Mana
		CurrentMana -= 50.0f;
		CurrentMana = FMath::Clamp(CurrentMana, 0.0f, MaxMana);


		//Play AnimMontange for healing
		PlayAnimMontage(HealingAnimation);

		//Disable Movement temporarily
		ARPG_Character::GetCharacterMovement()->DisableMovement();

		//Delay the reset of bIsCasting based on animation length
		float HealingAnimationDuration = HealingAnimation->GetPlayLength();
		GetWorldTimerManager().SetTimer(CastTimer, this, &ARPG_Character::OnCastingFinshed, HealingAnimationDuration, false);

	}

}




void ARPG_Character::StartHealing()
{

	Heal(20.0f);

}



void ARPG_Character::Poisoned()
{

	isPoisoned = true;

	if (isPoisoned)
	{
		CurrentHealth -= 5;
	}

}



void ARPG_Character::PoisonedTime()
{

	GetWorldTimerManager().SetTimer(PosionedTimer, this, &ARPG_Character::Poisoned, 5.0f, true);

	GetWorldTimerManager().SetTimer(StopPoisonTimer, this, &ARPG_Character::StopPoison, 15.0f, false);

}



void ARPG_Character::StopPoison()
{
	isPoisoned = false;
	GetWorldTimerManager().ClearTimer(PosionedTimer);

}



/*void ARPG_Character::RestoreHealth(float RestorationPercentage)
{

	if (RestorationPercentage >= 0.0f && RestorationPercentage <= 1.0f)

	{

		//Calculate the amount of health to restore based on the percentage
		float HealthToRestore = MaxHealth * RestorationPercentage;

		//Increase the character's current health, but make sure it does not exceed the max health.
		CurrentHealth = FMath::Min(CurrentHealth + HealthToRestore, MaxHealth);

	}


}
*/



void ARPG_Character::IncreaseMana(float MPAmount)
{

	if (CurrentMana < MaxMana && MPAmount > 0 && !bIsCasting && IsCombatModeOn)

	{

		bIsCasting = true;

		CurrentMana = FMath::Clamp(CurrentMana + MPAmount, 0.0f, MaxMana);

		PlayAnimMontage(IncreaseManaAnimation);

		ARPG_Character::GetCharacterMovement()->DisableMovement();

		float IncreaseManaAnimationDuration = IncreaseManaAnimation->GetPlayLength();
		GetWorldTimerManager().SetTimer(CastTimer, this, &ARPG_Character::OnCastingFinshed, IncreaseManaAnimationDuration, false);

	}


}

void ARPG_Character::StartManaIncrease()
{

	IncreaseMana(20.f);

}



void ARPG_Character::TogglePauseMenu()
{

	
}






void ARPG_Character::CalculateTotalAttack()
{

	TotalAttack = BaseAttack * AttackMultiplier;

}







void ARPG_Character::CalculateTotalDefence()
{

	TotalDefence = BaseDefence * DefenceMultiplier;

}





void ARPG_Character::CalculateTotalElemetnalAttack()
{
}

void ARPG_Character::CalculateTotalElementalDefence()
{
}




void ARPG_Character::GainExperience(int32 ExpAmount)
{

	ExperiencePoints += ExpAmount;
	CheckAndTriggerLevelUp();

}

void ARPG_Character::CheckAndTriggerLevelUp()
{
	UE_LOG(LogTemp, Warning, TEXT("CharacterLevel before: %d"), CharacterLevel);

	if (CharacterLevel < ExperienceRequired.Num() && ExperiencePoints >= ExperienceRequired[CharacterLevel])

	{

		CharacterLevel++; //Increase Character Level

		FString RowName = FString::Printf(TEXT("Row_%d"), CharacterLevel);
		UE_LOG(LogTemp, Warning, TEXT("RowName: %s"), *RowName); // Print the constructed RowName for verification

		FCharacterAttributesByLevel* Attributes = CharacterAttributesTable->FindRow<FCharacterAttributesByLevel>(FName(*RowName), FString("1"), true);

		if (Attributes)
		{
			// Print some attributes for verification
			UE_LOG(LogTemp, Warning, TEXT("MaxHealth: %f"), Attributes->MaxHealth);
			UE_LOG(LogTemp, Warning, TEXT("MaxMana: %f"), Attributes->MaxMana);
			

			//Assign character attributes to the attributes in the data table
			MaxHealth = Attributes->MaxHealth;
			MaxMana = Attributes->MaxMana;
			BaseAttack = Attributes->BaseAttack;
			BaseDefence = Attributes->BaseDefence;
			BaseElementalAttack = Attributes->BaseElementalAttack;
			BaseElementalDefence = Attributes->BaseElementalDefence;

			UE_LOG(LogTemp, Warning, TEXT("Character leveled up to Level %d"), CharacterLevel);
			UE_LOG(LogTemp, Warning, TEXT("MaxHealth: %f, MaxMana: %f"), MaxHealth, MaxMana);
		}

		else

		{

			UE_LOG(LogTemp, Warning, TEXT("Attributes not found for Level %d"), CharacterLevel);

		}
	}

}

void ARPG_Character::OnBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	
}

void ARPG_Character::ButtonPressForGainingExperience()
{

	//GainExperience(10);

}




void ARPG_Character::OnCastingFinshed()
{

	bIsCasting = false;
	ARPG_Character::GetCharacterMovement()->SetMovementMode(MOVE_Walking);

}



void ARPG_Character::InflictDamageOnEnemy(AEnemy* Enemy)
{

	if (Enemy)

	{

		float AttackDamage = BaseAttack;
		CalculatedDamage = AttackDamage * (1 - Enemy->DefencePercentage);

		Enemy->TakeDamage(CalculatedDamage, FDamageEvent(), GetController(), this);


	}


}







// Called when the game starts or when spawned
void ARPG_Character::BeginPlay()
{
	Super::BeginPlay();


	TArray<AActor*> OverlappingActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName(TEXT("Enemy")), OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		LockOnCandidates.Add(Actor);
	}



	CurrentMana = FMath::Clamp(CurrentMana, 0.0f, MaxMana);

	CalculateTotalAttack();


}


void ARPG_Character::TakeDamage(float DamageAmount)

{

	if (Blocking)

	{

		DamageAmount *= DamageReduction;

		PlayAnimMontage(BlockingHitAnimation, 1.5f);

		//LaunchCharacter(FVector(0.0, -600.0f, 0.0f), false, false);

		GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Black, TEXT("blocking hit!"));

	}


	if (DamageAmount > 0)
	{

		//CurrentHealth -= DamageAmount;

		CalculatedDamage = DamageAmount / (1 + TotalDefence);
		CurrentHealth -= CalculatedDamage;

		CurrentHealth = FMath::Clamp(CurrentHealth - CalculatedDamage, 0.0f, MaxHealth);

	}


		if (CurrentHealth <= 0)
		{

			GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Black, TEXT("Death! XP"));
			//Destroy();
			bIsdead = true;

			//PlayAnimMontage(DeathAnimation, 0.6f);



			APlayerController* PlayerController = Cast<APlayerController>(GetController());

			if (PlayerController)

			{

				DisableInput(PlayerController);

			}

			CalculatedDamage = DamageAmount / (1 + TotalDefence);



		}

}



// Called every frame
void ARPG_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (bIsHardLockedOn && HardLockedActor)
	{
		float MaxRangeSquared = MaxHardLockRange * MaxHardLockRange;
		float DistanceSquared = FVector::DistSquared(GetActorLocation(), HardLockedActor->GetActorLocation());

		if (DistanceSquared > MaxRangeSquared)
		{
			bIsHardLockedOn = false;
			HardLockedActor = nullptr;
		}
	}


	if (bIsHardLockedOn)

	{

		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), HardLockedActor->GetActorLocation());
		LookAtRotation.Pitch -= HeightTargetingOffest;
		GetController()->SetControlRotation(LookAtRotation);
	}




	ToggleSoftLock();

	ToggleHardLockOnEffects();

	ToggleSoftLockOnEffects();

	

}




// Called to bind functionality to input
void ARPG_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);




	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	//PlayerInputComponent->BindAxis("RightStickX", this, &ARPG_Character::HandleRightStickX);
	//PlayerInputComponent->BindAxis("RightStickY", this, &ARPG_Character::HandleRightStickY);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("HardLockOn", IE_Pressed, this, &ARPG_Character::ToggleHardLock);
	PlayerInputComponent->BindAction("Switch Locked On Target", IE_Pressed, this, &ARPG_Character::ChangingHardLockedTargets);
	PlayerInputComponent->BindAction("Mana", IE_Pressed, this, &ARPG_Character::StartManaIncrease);
	PlayerInputComponent->BindAction("EXP", IE_Pressed, this, &ARPG_Character::ButtonPressForGainingExperience);
	PlayerInputComponent->BindAction("Poison", IE_Pressed, this, &ARPG_Character::PoisonedTime);
	

	//PlayerInputComponent->BindAction("Heal", IE_Pressed, this, &ARPG_Character::StartHealing);
	//PlayerInputComponent->BindAction("Set Nearest Target Left", IE_Pressed, this, &ARPG_Character::SelectNearestTargetLeft);
	//PlayerInputComponent->BindAction("Set Nearest Target Right", IE_Pressed, this, &ARPG_Character::SelectNearestTargetRight);
	



	PlayerInputComponent->BindAxis("MoveForward", this, &ARPG_Character::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ARPG_Character::MoveRight);


}

