// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeReversalComponent.h"
#include "TimeMasterCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"

// Sets default values for this component's properties
UTimeReversalComponent::UTimeReversalComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	Owner = nullptr;
	IsInit = false;
	IsTimeReversing = false;
	IsOutdated = false;
	RecordTimeLength = 0.0f;
	// ...
}


// Called when the game starts
void UTimeReversalComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	if(!IsInit) {
		Owner = GetOwner();
		IsInit = true;
	}
	
}


// Called every frame
void UTimeReversalComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UTimeReversalComponent::SetTimeReversing(bool TimeReversingState)
{
	IsTimeReversing = TimeReversingState;

}

