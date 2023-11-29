// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryItem.h"
#include "RPG_Character.h"
#include "InventoryComponent.generated.h"


class ARPG_Character;

USTRUCT(BlueprintType)
struct FInventoryItem

{

	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
		FName ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
		FString ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
		int32 Quantity;

};



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPG_TEST_ANIMS_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;



	ARPG_Character* OwningCharacter;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
		void AddItemToInventory(FName ItemName);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
		TArray<FInventoryItem> Inventory;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
		int32 MaxSlots;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
		bool CanAddItemToInventory();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
		int32 GetItemQuantity(FName ItemName) const;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
		void UseItem(FName ItemName);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
		void RemoveItemFromInvnetory(FName ItemName);

	UFUNCTION(BlueprintPure, Category = "Inventory")
		bool HasHealthPotion() const;




};



