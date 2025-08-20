// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "Math/Vector.h"
#include "Math/Rotator.h"


#include "TimeReversalComponent.generated.h"


struct TimeInfo {
	FVector Location;
	FRotator Rotation;
	FVector LinearVelocity;
	FVector AngularVelocity;
	float DeltaTime;
	TimeInfo(FVector _Location, FRotator _Rotation, FVector _LinearVelocity, FVector _AngularVelocity, float _DeltaTime) :
		Location(_Location), Rotation(_Rotation), LinearVelocity(_LinearVelocity), AngularVelocity(_AngularVelocity){ }

};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent),Blueprintable)
class TIMEMASTER_API UTimeReversalComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTimeReversalComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	AActor* Owner;
	bool IsInit;
	bool IsTimeReversing;
	bool IsOutdated;
	float RecordTimeLength;
	TDoubleLinkedList<TimeInfo> TimeFrames;

	UFUNCTION(BlueprintCallable)
	void SetTimeReversing(bool TimeReversingState);
		
};
