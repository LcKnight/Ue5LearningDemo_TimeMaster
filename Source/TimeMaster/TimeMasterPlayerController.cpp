// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeMasterPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "TimeMaster.h"
#include "TimeMasterCharacter.h"

void ATimeMasterPlayerController::BeginPlay()
{
	Super::BeginPlay();

}

void ATimeMasterPlayerController::SetupInputComponent()
{
	if (IsLocalPlayerController()) {
		//add the input mapping contexts
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer())) {
			for (UInputMappingContext* CurrentContext : DefaultMappingContexts) {
				Subsystem->AddMappingContext(CurrentContext, 0);
			}
		}
	}
}

void ATimeMasterPlayerController::OnPossess(APawn* InitPawn)
{
	Super::OnPossess(InitPawn);
	//subscribe to the pawn's OnDestroyed delegate
	InitPawn->OnDestroyed.AddDynamic(this, &ATimeMasterPlayerController::OnPawnDestroyed);
	if (ATimeMasterCharacter* PlayerCharacter = Cast<ATimeMasterCharacter>(InitPawn))
	{
		// add the player tag
		PlayerCharacter->Tags.Add(PlayerPawnTag);

		// subscribe to the pawn's delegates
		//Character->OnBulletCountUpdated.AddDynamic(this, &ATimeMasterCharacter::OnBulletCountUpdated);
		PlayerCharacter->OnDamaged.AddDynamic(this, &ATimeMasterPlayerController::OnPawnDamaged);

		// force update the life bar
		PlayerCharacter->OnDamaged.Broadcast(1.0f);
	}
}

void ATimeMasterPlayerController::OnPawnDestroyed(AActor* DestroyedActor)
{
	TArray<AActor*> ActorList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), ActorList);
	if (ActorList.Num() > 0)
	{
		// select a random player start
		AActor* RandomPlayerStart = ActorList[FMath::RandRange(0, ActorList.Num() - 1)];

		// spawn a character at the player start
		const FTransform SpawnTransform = RandomPlayerStart->GetActorTransform();

		if (ATimeMasterCharacter* RespawnedCharacter = GetWorld()->SpawnActor<ATimeMasterCharacter>(CharacterClass, SpawnTransform))
		{
			// possess the character
			Possess(RespawnedCharacter);
		}
	}
}

void ATimeMasterPlayerController::OnBulletCountUpdated(int32 MagazineSize, int32 Bullets)
{
}
void ATimeMasterPlayerController::OnPawnDamaged(float LifePercent)
{

}

