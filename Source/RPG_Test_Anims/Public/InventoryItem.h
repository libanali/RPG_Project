// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include "Engine/DataTable.h"
#include "InventoryItem.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct RPG_TEST_ANIMS_API FMyInventoryItem : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

		
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	FString ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 Quantity;
	
};
