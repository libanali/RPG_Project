// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RPG_Character.h"
#include "Enemy.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEnemyDeathDelegate);


UCLASS()
class RPG_TEST_ANIMS_API AEnemy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemy();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		float EnemyHealth;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defence")
		float DefencePercentage;


	UFUNCTION(BlueprintCallable, Category = "Enemy")
	bool EnemyIsDead() const;

	bool bIsDead;

	void Death();

	void Destroyed();



	UPROPERTY(BlueprintAssignable, Category = "Events")
		FEnemyDeathDelegate OnEnemyDeathDelegate;


	//UFUNCTION(BlueprintCallable, Category = Damage)
	//	void ReceiveDamage(float DamageAmount);

	UFUNCTION(BlueprintCallable, Category = "Damage")
		void InflictDamageOnCharacter(ARPG_Character* Character);


	virtual float TakeDamage (float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;




	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
		float BaseAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
		float AttackMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
		float TotalEnemyAttack;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
