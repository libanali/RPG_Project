// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "RPG_Character.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	EnemyHealth = 30.0f;

	bIsDead = false;

	DefencePercentage = 3.0f;

	AttackMultiplier = 1.5f;

	BaseAttack = 5.0f;

}








bool AEnemy::EnemyIsDead() const
{
	return bIsDead;
}

void AEnemy::Death()
{

	if (EnemyHealth <= 0)

	{

		Destroy();

		bIsDead = true;

		ARPG_Character* Character = Cast<ARPG_Character>(GetWorld()->GetFirstPlayerController()->GetPawn());

		Character->GainExperience(40.f);

	}


}

void AEnemy::Destroyed()
{

	//Super::Destroyed();

	//ARPG_Character* Character = Cast<ARPG_Character>(GetWorld()->GetFirstPlayerController()->GetPawn());


	//if (Character != nullptr)
	//{

	//	Character->TurnOffHardLock();

	//}


}


//void AEnemy::ReceiveDamage(float DamageAmount)
//{

//	EnemyHealth -= DamageAmount;


//}

void AEnemy::InflictDamageOnCharacter(ARPG_Character* Character)
{

	if (Character)

	{

		TotalEnemyAttack = BaseAttack * AttackMultiplier;
		float DamageToInflict = TotalEnemyAttack / (1 + Character->BaseDefence);

		Character->TakeDamage(DamageToInflict);

	}

}


float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{

	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	float CalculatedDamage = DamageAmount * (1 - DefencePercentage);

	EnemyHealth -= CalculatedDamage;

	return CalculatedDamage;
}



// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Death();
}

