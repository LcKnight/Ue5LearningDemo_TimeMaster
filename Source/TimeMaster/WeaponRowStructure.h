#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/StaticMesh.h"
#include "WeaponRowStructure.generated.h"

class ATimeMasterWeapon;

USTRUCT(BlueprintType)
struct FWeaponRowStructure : public FTableRowBase
{
	GENERATED_BODY()

	/** Mesh to display on the pickup */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) // Changed for blueprint access
		TSoftObjectPtr<UStaticMesh> StaticMesh;

	/** Weapon class to grant on pickup */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) // Changed for blueprint access
		TSubclassOf<ATimeMasterWeapon> WeaponToSpawn;
};