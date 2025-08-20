// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TimeMasterWeaponHolder.generated.h"


class ATimeMasterWeapon;
class UAnimMontage;


// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UTimeMasterWeaponHolder : public UInterface
{
	GENERATED_BODY()

};

/**
 * 
 */
class TIMEMASTER_API ITimeMasterWeaponHolder
{
	GENERATED_BODY()
public:
	virtual void AttachWeaponMeshes(ATimeMasterWeapon* Weapon) = 0;

	virtual void PlayFiringMontage(UAnimMontage* Montage) = 0;


	virtual void AddWeaponRecoil(float Recoil) = 0;


	virtual FVector GetWeaponTargetLocation() = 0;

	virtual void AddWeaponClass(const TSubclassOf<ATimeMasterWeapon>& WeaponCLass) = 0;
	virtual void OnWeaponActivated(ATimeMasterWeapon* Weapon) = 0;

	virtual void OnWeaponDeactivated(ATimeMasterWeapon* Weapon) = 0;
	/** Notifies the owner that the weapon cooldown has expired and it's ready to shoot again */
	virtual void OnSemiWeaponRefire() = 0;


	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
};
