// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TimeMasterPlayerController.generated.h"

class UInputMappingContext;
class ATimeMasterCharacter;
/**
 * 
 */
UCLASS()
class TIMEMASTER_API ATimeMasterPlayerController : public APlayerController
{
	GENERATED_BODY()
protected:

	//input mapping contexts for FirstPerson player
	UPROPERTY(EditAnywhere, Category = "Input|Input Mappings")
	TArray<UInputMappingContext*> DefaultMappingContexts;

	//Character class to respawn when the possessed pawn is destroyed
	UPROPERTY(EditAnywhere, Category = "Character|Rewpawn")
	TSubclassOf<ATimeMasterCharacter> CharacterClass;

	//Tag the possessed pawn and pin it as the player
	UPROPERTY(EditAnywhere, Category = "Character|")
	FName PlayerPawnTag = FName("Player 0");

protected:
	//Gameplay initializaiton
	virtual void BeginPlay() override;

	//Initialize input bindings
	virtual void SetupInputComponent() override;

	//Pawn initialization
	virtual void OnPossess(APawn* InitPawn) override;

	//called when the possesed pawn is destroyed
	UFUNCTION()
	void OnPawnDestroyed(AActor* DestroyedActor);

	//called when the bullet count on the possessed pawn is updated
	UFUNCTION()
	void OnBulletCountUpdated(int32 MagazineSize, int32 Bullets);


	//called when the possessed pawn is damaged
	UFUNCTION()
	void OnPawnDamaged(float LifePercent);
};
