// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "InventoryItem.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

	MaxSlots = 4;
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	OwningCharacter = Cast<ARPG_Character>(GetOwner());
	
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInventoryComponent::AddItemToInventory(FName ItemName)
{

	if (Inventory.Num() >= MaxSlots || ItemName.IsNone())

	{

		if (Inventory.Num() >= MaxSlots)

		{

			UE_LOG(LogTemp, Warning, TEXT("Inventory is full. Cannot add more items."));


		}

		else if (ItemName.IsNone())

		{

			UE_LOG(LogTemp, Warning, TEXT("Invalid item name. Item not added to the inventory."));

		}

		return;

	}

	//Creating a new inventory item

	FInventoryItem NewItem;

	NewItem.Quantity = 1;
	NewItem.ItemName = FName(*ItemName.ToString());

	//Adding the item to the inventory

	Inventory.Add(NewItem);

}

bool UInventoryComponent::CanAddItemToInventory()
{
	if (Inventory.Num() >= MaxSlots)

	{

		return false;
	}

	return true;
}



int32 UInventoryComponent::GetItemQuantity(FName ItemName) const
{
	int32 TotalyQuantity = 0;

	for (const FInventoryItem& Item : Inventory)

	{

		if (Item.ItemName.ToString() == ItemName.ToString())

		{


			TotalyQuantity += Item.Quantity;

		}
	}


	return int32();
}

void UInventoryComponent::UseItem(FName ItemName)
{

	// Iterate through the inventory to find the item

	for (FInventoryItem& Item : Inventory)
	{

		if (Item.ItemName.ToString() == ItemName.ToString())

		{

			if (Item.ItemType == "HealthPotion")

			{

				//Increase health by 25%

				float HPRestorationPercentage = 0.25f;
				float RestoredHealth = OwningCharacter->MaxHealth * HPRestorationPercentage;

				//Ensure current health does not exceed max health

				OwningCharacter->CurrentHealth = FMath::Clamp(OwningCharacter->CurrentHealth, 0.0f, OwningCharacter->MaxHealth);
				

			}

			else if (Item.ItemType == "UltraHealthPotion")

			{
				
				//Increase health by 45%
				float HPRestorationPercentage = 0.45f;
				float RestoredHealth = OwningCharacter->MaxHealth * HPRestorationPercentage;

				//Ensure current health does not exceed max health
				OwningCharacter->CurrentHealth = FMath::Clamp(OwningCharacter->CurrentHealth, 0.0f, OwningCharacter->MaxHealth);

				
			}

			else if (Item.ItemType == "ManaPotion")

			{
				//Increase mana by 25%
				float MPRestorationPerecentage = 0.25f;
				float RestoredMP = OwningCharacter->MaxMana * MPRestorationPerecentage;

				//Ensure current mana does not exceed max mana
				OwningCharacter->CurrentMana = FMath::Clamp(OwningCharacter->CurrentMana, 0.0f, OwningCharacter->MaxMana);

			}

			else if (Item.ItemType == "UltraManaPotion")

			{

				//Increase mana by 45%
				float MPRestorationPercentage = 0.45f;
				float RestoredMP = OwningCharacter->MaxMana * MPRestorationPercentage;

				//Ensure current mana does not exceed max mana
				OwningCharacter->CurrentMana = FMath::Clamp(OwningCharacter->CurrentMana, 0.0f, OwningCharacter->MaxMana);

			}



			//Decrement item quantity
			Item.Quantity--;


			//Check if item quantity is equal or below 0. If it returns true, then remove item from inventory.
			if (Item.Quantity <= 0)

			{

				RemoveItemFromInvnetory(ItemName);

			}


			break;

		}
	}


}




bool UInventoryComponent::HasHealthPotion() const
{
	for (const FInventoryItem& Item : Inventory)
	{

		if (Item.ItemName == "HealthPotion")

		{

			return true;

		}

	}

	return false;
}



void UInventoryComponent::RemoveItemFromInvnetory(FName ItemName)
{

	for (int32 Index = 0; Index < Inventory.Num(); Index++)


	{

		if (Inventory[Index].ItemName.ToString() == ItemName.ToString())
		{

			Inventory.RemoveAt(Index);
			return;

		}

	}


}

